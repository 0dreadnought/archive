/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************

  * @brief  Main program
  * @param  None
  * @retval None
  */

#include "main.h"

//declaration for the GPIO pins for the LED

#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED1_PIN                         GPIO_PIN_5
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define USER_BUTTON_PIN			 GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT		 GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISSABLE()

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LED2_Init(void);
void LED2_On(void);
void LED2_Off(void);
void LED2_DeInit(void);
void LED2_Toggle(void);

void LED1_Init(void);
void LED1_On(void);
void LED1_Off(void);
void LED1_DeInit(void);
void LED1_Toggle(void);


int main(void)
{
/* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 80 MHz */
  SystemClock_Config();

  LED2_Init();
  LED1_Init();
  User_PB_Init();

  LED2_On();
  LED1_Off();


    while (1){
    
    /* p.e. 1
    
    if(User_PB_STATE()==0){
    while(User_PB_STATE()==0){}
    LED2_Toggle();
    }*/
    
    /* p.e. 2
    if(User_PB_STATE()==0){
    while(User_PB_STATE()==0){}
    LED2_Toggle();
    LED1_Toggle();
    }
    */
    
    // credit exercise    
      LED2_Off();
      if(User_PB_STATE()==0){
        HAL_Delay(50); //debouncing
        while(User_PB_STATE()==0){
      LED1_On();
      }
      LED1_Off();
      } 
    
    
    }  
      

}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/*
Inititalise the LED2 GPIO port
*/

void LED2_Init(void)
{

   GPIO_InitTypeDef  gpio_init_structure;
  
  LED2_GPIO_CLK_ENABLE();
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin   = LED2_PIN;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_structure);
}

void LED1_Init(void)
{

   GPIO_InitTypeDef  gpio_init_structure;
  
  LED1_GPIO_CLK_ENABLE();
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin   = LED1_PIN;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_structure);
}

/*

deinit the GPIO for LED2

*/
void LED2_DeInit()
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = LED2_PIN;
  
  /* Turn off LED */
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_DeInit(LED2_GPIO_PORT, gpio_init_structure.Pin);
}

void LED1_DeInit()
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = LED1_PIN;
  
  /* Turn off LED */
  HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_DeInit(LED1_GPIO_PORT, gpio_init_structure.Pin);
}


/*

Turn LED2 on

*/
void LED2_On(void)
{
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
}

void LED1_On(void)
{
  HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

/* 
turn LED2 off
*/

void LED2_Off(void)
{
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
}

void LED2_Toggle(void)
{
  HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
}


void LED1_Off(void)
{
  HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void LED1_Toggle(void)
{
  HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
}



void User_PB_Init()
{
GPIO_InitTypeDef gpio_init_structure;

USER_BUTTON_GPIO_CLK_ENABLE();

gpio_init_structure.Pin = USER_BUTTON_PIN;
gpio_init_structure.Mode = GPIO_MODE_INPUT;
gpio_init_structure.Pull = GPIO_PULLUP;
gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(USER_BUTTON_GPIO_PORT,&gpio_init_structure);

}

int User_PB_STATE()
{

	return HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);

}
