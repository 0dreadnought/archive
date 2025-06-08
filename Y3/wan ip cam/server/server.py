from flask import Flask, request, send_file, jsonify , Response, render_template, stream_with_context

import time
import os

app = Flask(__name__, template_folder = '../dns')
UPLOAD_FOLDER = 'uploads'

os.makedirs(UPLOAD_FOLDER, exist_ok=True)

# In-memory last image path
last_image_path = os.path.join(UPLOAD_FOLDER, 'device123.jpg')

@app.route('/')
def index():
    return render_template(os.path.join("",'index.html'))

@app.route('/api/')
def indexx():
    return "ESP32 CAM server is OK"

@app.route('/upload/', methods=['POST'])
def upload():
    device_name = request.args.get('name')
    device_pass = request.args.get('pass')
    filepath = os.path.join(UPLOAD_FOLDER,device_name + device_pass + ".jpg")
    with open(filepath, 'wb') as f:
        f.write(request.data)
    return "OK", 200

@app.route('/api/camera')
def get_image():
    filepath = os.path.join(UPLOAD_FOLDER,request.args.get('name') + request.args.get('pass') + ".jpg")
    if os.path.exists(filepath):
        return send_file(filepath, mimetype= 'image/gif')
    return "Not Found", 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)