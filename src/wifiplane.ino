//**************************************************
// WiFi Controlled Tiny Airplane
// ESP8266 Firmware ino file
// By Ravi Butani
// Rajkot INDIA
// Instructables page: https://www.instructables.com/id/WIFI-CONTROLLED-RC-PLANE/
//***************************************************
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define P_ID 1
#define ST_LED  2
#define L_MOTOR 5
#define R_MOTOR 4
#define DC_RSSI 1500  // Time in mS for send RSSI
#define DC_RX   900   // Time in mS for tx inactivity 200 old problem of motor stopping flickring
ADC_MODE(ADC_VCC);
unsigned int l_speed = 0;
unsigned int r_speed = 0;

unsigned long premillis_rssi = 0;
unsigned long premillis_rx   = 0;

int status = WL_IDLE_STATUS;
char ssid[] = "wifiplane";   //  your network SSID (name)
char pass[] = "wifiplane1234";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
IPAddress remotIp;
unsigned int localPort = 6000;      // local port to listen on
unsigned int remotPort = 2390;      // local port to talk on
char  packetBuffer[10]; //buffer to hold incoming packet
char  replyBuffer[]={P_ID,0x01,0x01,0x00}; // a string to send back
WiFiUDP Udp;

// the setup function runs once when you press reset or power the board
void setup() {
  WiFi.mode(WIFI_STA);
  //WiFi.setOutputPower(2.5);
  analogWriteRange(255);
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(L_MOTOR,0);
  analogWrite(R_MOTOR,0);
  pinMode(ST_LED, OUTPUT);
  digitalWrite(ST_LED,HIGH);
  //Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(ST_LED,LOW);
    delay(60);
    digitalWrite(ST_LED,HIGH);
    delay(1000);
    //Serial.print(".");
  }
  remotIp=WiFi.localIP();
  remotIp[3] = 255;
  Udp.begin(localPort);
}

// the loop function runs over and over again forever
void loop() {
  delay(5);
  if(WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(ST_LED,LOW);
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize) 
    {
      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 10);
      if (len > 2) 
      {
        if(packetBuffer[0] == P_ID)
        {
          l_speed = (unsigned int)packetBuffer[1]*2-2;
          r_speed = (unsigned int)packetBuffer[2]*2-2;
          //Serial.print(l_speed);
          //Serial.print(" \t");
          //Serial.println(r_speed);
          analogWrite(L_MOTOR,l_speed);
          analogWrite(R_MOTOR,r_speed);
          premillis_rx = millis();
        }
      }
      
    }
    if(millis()-premillis_rssi > DC_RSSI)
    {
       premillis_rssi = millis();
       long rssi = abs(WiFi.RSSI());
       float vcc = (((float)ESP.getVcc()/(float)1024.0)+0.75f)*10;
       replyBuffer[1] = (unsigned char)rssi;
       replyBuffer[2] = (unsigned char)vcc;
       
       Udp.beginPacket(remotIp, remotPort);
       Udp.write(replyBuffer);
       Udp.endPacket();
     }
     if(millis()-premillis_rx > DC_RX)
     {
       analogWrite(L_MOTOR,0);
       analogWrite(R_MOTOR,0);
       //Serial.println("nodata");
     }
  }
  else
  {
    digitalWrite(ST_LED,LOW);
    delay(60);
    digitalWrite(ST_LED,HIGH);
    delay(1000);
    analogWrite(L_MOTOR,0);
    analogWrite(R_MOTOR,0);
    digitalWrite(ST_LED,HIGH);
  }
}
