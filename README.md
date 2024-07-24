# smart-cane-2024

# 4 Wire Parts

# 5 Install Board Manager
1. Add this “https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json” to File > Preferences > Additional Board Manager URL

2. Open Board Manager (Tools > Board > Board Manager) and install esp32 library

# 6 Install Library
1. Go to this github and download library as zip file

2. Install library (Sketch > Include library > Add zip file)

# 7 Upload code to ESP32
“Attention : this is the most tricky part“
1. copy below code to your arduino IDE and change wifi_ssid, wifi_pass of yours.
#include 
#include 
#include 
 
const char* WIFI_SSID = "<strong><em>YOUR WIFI NAME</em></strong>";
const char* WIFI_PASS = "<strong><em>YOUR WIFI PASSWORD</em></strong>";
 
WebServer server(80);
 
 
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);
void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame-&amp;gt;getWidth(), frame-&amp;gt;getHeight(),
                static_cast(frame-&amp;gt;size()));
 
  server.setContentLength(frame-&amp;gt;size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame-&amp;gt;writeTo(client);
}
 
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}
 
 
void  setup(){
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
 
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
 
  server.begin();
}
 
void loop()
{
  server.handleClient();
}
 
2. connect I01 and GND of ESP32-CAM before uploading code

3. connect module with computer and check your FTDI’s port

4. set your board as ESP32 Wrover Module

5. set your port of your FTDI

6. set your frequency to 80hz

7. set your partition scheme like below

8. set upload speed like below

9. code upload done

10. disconnect I01 and GND of ESP32-CAM before uploading code

11. push reset button to run your code

12. check your serial monitor 115200 band

# 8 Run Object detection code
1. copy below python code
import cv2 #opencv
import urllib.request #to open and read URL
import numpy as np
 
#OBJECT CLASSIFICATION PROGRAM FOR VIDEO IN IP ADDRESS
 
url = 'http://172.12.3.195/cam-hi.jpg'
#url = 'http://192.168.1.6/'
winName = 'ESP32 CAMERA'
cv2.namedWindow(winName,cv2.WINDOW_AUTOSIZE)
#scale_percent = 80 # percent of original size    #for image processing
 
classNames = &#091;]
classFile = 'coco.names'
with open(classFile,'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')
 
configPath = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = 'frozen_inference_graph.pb'
 
net = cv2.dnn_DetectionModel(weightsPath,configPath)
net.setInputSize(320,320)
#net.setInputSize(480,480)
net.setInputScale(1.0/127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)
 
while(1):
    imgResponse = urllib.request.urlopen (url) # here open the URL
    imgNp = np.array(bytearray(imgResponse.read()),dtype=np.uint8)
    img = cv2.imdecode (imgNp,-1) #decodificamos
 
    img = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE) # vertical
    #img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) #black and white
 
    
 
    classIds, confs, bbox = net.detect(img,confThreshold=0.5)
    print(classIds,bbox)
 
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            cv2.rectangle(img,box,color=(0,255,0),thickness = 3) #mostramos en rectangulo lo que se encuentra
            cv2.putText(img, classNames&#091;classId-1], (box&#091;0]+10,box&#091;1]+30), cv2.FONT_HERSHEY_COMPLEX, 1, (0,255,0),2)
 
 
    cv2.imshow(winName,img) #  show the picture
 
    #wait for ESC to be pressed to end the program
    tecla = cv2.waitKey(5) &amp;amp; 0xFF
    if tecla == 27:
        break
cv2.destroyAllWindows()
2. download below three file and locate them within same folder with your python code
ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt

frozen_inference_graph.pb

coco.names


3. change code url using current serial monitor url
