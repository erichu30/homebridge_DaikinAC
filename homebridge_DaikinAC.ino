#include <ESP8266WiFi.h>        // library for wifi
#include <ESP8266WebServer.h>   // library for web server
#include <DYIRDaikin.h>

#define SSID   "YOURSSID"
#define PASSWORD  "YOURPASSWORD"
#define DYIRDAIKIN_SOFT_IR
DYIRDaikin irdaikin;
int isOn;

// declare object and port
ESP8266WebServer server(80);   

String cmd1,cmd2;

void wificonfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  // you may configure your own network there
  WiFi.config(IPAddress(192,168,0,5),   // IP address
              IPAddress(192,168,0,1),     // gateway
              IPAddress(255,255,255,0));  // netmask
    
  while (WiFi.status() != WL_CONNECTED){
     Serial.println("Connecting Wifi...");
     delay(500);
  }
      
  Serial.println("Wifi connected at: ");
  Serial.println(WiFi.localIP());
  Serial.println("-----------------------------------------------------");

}

// define function when requesting http://ip/index.html
void rootRouter()
{
  server.send(200, "text/html",  "A = <b>" + cmd1 + "</b><br>"+"B = <b>" + cmd2 + "</b><br>");
}

// define initial status of irdaikin
void irdaikinInit(int pin)
{
  irdaikin.begin(pin);
  irdaikin.on();
  irdaikin.setSwing_off();
  irdaikin.setMode(1); //0:FAN, 1:COOL, 2:DRY
  irdaikin.setFan(1); //FAN speed: 0:speed1 1:speed2 2:speed3 3:speed4 4:speed5 5:Auto 6:moon
  irdaikin.setTemp(25); //18-33
}

void acOn()
{
  if(isOn!=0){
    isOn = 0;
    irdaikin.on();
    irdaikin.setFan(1);
    Serial.println("Turn On");
    irdaikin.sendCommand();
    Serial.println("Execute Command!");
  }
  Serial.flush();
}

void acOff()
{
  if(isOn==0){
    isOn = 1;
    irdaikin.off();
    Serial.println("Turn Off");
    irdaikin.sendCommand();
    Serial.println("Execute Command!");
  }
  Serial.flush();
}

void setup() {
  Serial.begin(115200);
  wificonfig();
    
  // handling request
  server.on("/index.html", rootRouter); 
  server.on("/", rootRouter);
    
  // handling request with arguments
  // as format such as http://192.168.0.108/cmd?A=On&B=2 in url
  server.on("/cmd", []() {
    //initial value
    cmd1="";
    cmd2="";

    cmd1=server.arg("A"); //key name
    cmd2=server.arg("B"); //key name
    
    Serial.print(cmd1);
    Serial.print(",");
    Serial.println(cmd2);
    if(cmd1=="On"){
        acOn();
      }
    if(cmd1=="Off"){
        acOff();
      }

    Serial.flush();
        server.send(200, "text/html", "OK" );
  }); 

  // handling when unable to find the path
  server.onNotFound([](){   
    server.send(404, "text/plain", "File NOT found!");
  });
  
  server.begin();

  delay(1000);
  irdaikinInit(4);
  //----everything is ok and to execute send command-----
  irdaikin.sendCommand();
  isOn = 0;
  Serial.println("IRremote Start\n");
}

void loop() {
  server.handleClient();  // handle clients connections
}
