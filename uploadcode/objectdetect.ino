#include <WiFi.h> // library to connect to Wifi networks
#include <WebServer.h> // library to create a web server on the ESP32
#include <WifiCam.hpp>

// define GPIO pins for the camera
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27
#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22
 
const char* WIFI_SSID = "SHIVANI-PC 1254";
const char* WIFI_PASS = "a0387A=9";

// store the inital resolution setting for the camera
esp32cam::Resolution initialResolution;

// create a web server object listening on port 80
WebServer server(80);

// finds and stores resolutions for low, medium, and high quality images
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

// captures an image from the camera and serves it as a JPEG file over the web server
void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(), static_cast<int>(frame->size()));
  
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}
 
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(esp32cam::Resolution::find(320, 240))) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(esp32cam::Resolution::find(800, 600))) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(esp32cam::Resolution::find(350, 530))) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

/* defines URL endpoints for different resolution images
   associates them with their respective handler functions */
void addRequestHandlers() {
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
}
 
void setup(){
  Serial.begin(115200);
  Serial.println();

  Serial.println("Check 1");
  WiFi.persistent(false); // disables persistence WiFi settings
  WiFi.mode(WIFI_STA); // sets the WiFi mode to station mode
  Serial.println("Check 2");
  WiFi.begin(WIFI_SSID, WIFI_PASS); // starts WiFi connection with SSID and password
  Serial.println("Check 3");

  /* checks for successful connection
  /  if not, it restarts the ESP32 */
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi failure %d\n", WiFi.status());
    delay(5000);
    ESP.restart();
  }

  Serial.println("WiFi connected");
  delay(1000);

  {
    using namespace esp32cam; // prefix everything with esp32cam::

    initialResolution = Resolution::find(1024, 768); // set initalResolution to a Resolution object 1024 by 768

    Config cfg; // create Config object to hold camera settings
    cfg.setPins(pins::AiThinker); // sets pin configuration of cfg to match the AiThinker pin configuration
    cfg.setResolution(initialResolution); // set resolution of cfg to 1024x768 pixels
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg); // initializes camera with cfg settings
    if (!ok) {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
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
