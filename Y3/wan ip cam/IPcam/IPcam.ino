#include <WiFiManager.h>

#include "esp_camera.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include "esp_wpa2.h"
#include <Preferences.h>


#define FPS 25

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

void startCameraServer();
void setupLedFlash(int pin);

Preferences prefs;

String device_name;
String device_pass;

void sendFrameHttp() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) return;

  HTTPClient http;
  http.setReuse(false);
  http.begin("http://130.61.122.213:5000/upload/?name="+ device_name + "&pass="+device_pass);
  http.addHeader("Content-Type", "image/jpeg");

  int httpResponseCode = http.POST(fb->buf, fb->len);

  esp_camera_fb_return(fb);
  Serial.printf("Upload response: %d\n", httpResponseCode);

  http.end();
  delay(1000/FPS);
}

bool connect_Wifi_Enterprise(String SSID, String name, String pass){
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);

  esp_wifi_sta_wpa2_ent_disable();
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)name.c_str(), name.length());
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)name.c_str(), name.length());
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)pass.c_str(), pass.length());
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(SSID.c_str());

  int count = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(count == 20){
      return false;
    }
    count++;
  }
  return true;
}

bool connect_Wifi_Personal(String SSID, String pass){

  WiFi.begin(SSID.c_str(), pass.c_str());

  int count = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(count == 20){
      return false;
    }
    count++;
  }
  return true;
}


void setup() {
  Serial.begin(115200);

  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

////////////////////
//     WIFI       //
////////////////////

  prefs.begin("wifi-creds", false);
  bool saved = prefs.getBool("saved", false);

  bool connected = false;

  if(saved){
    bool is_Eprise = prefs.getBool("eprise", false);
    
    device_name = prefs.getString("device_name","default");
    device_pass = prefs.getString("device_pass","default");

    if(is_Eprise){
      connected = connect_Wifi_Enterprise(prefs.getString("ssid"), prefs.getString("name"), prefs.getString("pass"));
    }
    else{
      connected = connect_Wifi_Personal(prefs.getString("ssid"), prefs.getString("pass"));
    }
  }
  if(!connected || !saved){
    WiFiManager wm;
    wm.setEnableConfigPortal(true);
    wm.setBreakAfterConfig(true);
    WiFiManagerParameter username_par("username", "Wifi Username", "", 64);
    
    WiFiManagerParameter devicename_par("devicename", "Device name", "", 64);
    WiFiManagerParameter devicepass_par("devicepass", "Stream password", "", 64);
    
    wm.addParameter(&username_par);
    wm.addParameter(&devicename_par);
    wm.addParameter(&devicepass_par);

    if (!wm.startConfigPortal("ESP32_Setup", "setup123")) {
      Serial.println("Config portal timeout.");
    }

    String wifi_ssid = wm.getWiFiSSID();
    String wifi_name = username_par.getValue();
    String wifi_pass = wm.getWiFiPass();
    device_name = devicename_par.getValue();
    device_pass = devicepass_par.getValue();

    prefs.putBool("saved", true);
    prefs.putString("ssid", wifi_ssid);
    prefs.putString("name", wifi_name);

    if(wifi_name.length()>0){
      prefs.putBool("eprise", true);
    }
    else{
      prefs.putBool("eprise", false);
    }
    prefs.putString("pass", wifi_pass);

    if(device_name.length()>0){
      prefs.putString("device_name", device_name);
    }
    if(device_pass.length()>0){
      prefs.putString("device_pass", device_pass);
    }

    Serial.println("Got credentials:");
    Serial.println("SSID: " + wifi_ssid);
    Serial.println("Username: " + wifi_name);
    Serial.println("Password: " + wifi_pass);

    Serial.println("Device name: " + device_name);
    Serial.println("Stream password: " + device_pass);

    ESP.restart();
  }

  WiFi.setSleep(false);

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Ready! \n");


}

void loop() {

  sendFrameHttp();

}
