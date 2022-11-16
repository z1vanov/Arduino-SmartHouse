#include <IRremote.h>

#include <UIPEthernet.h>

IRsend mySender;

int temt6000Pin = A0;
int value = 0;

int ThermistorPin = A2;
int Vo;
float R1 = 10500;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

String html = "<!DOCTYPE html> <html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title></head><style>.cent { margin: auto;width: 60%;border: 3px solid grey;padding: 10px;}table {font-family: arial, sans-serif;border-collapse: collapse;width: 100%;}td,th {border: 1px solid #dddddd;text-align: left;padding: 8px;}tr:nth-child(even) {background-color: #dddddd;}</style><body><div class=\"cent\"><h2>Arduino Controls</h2><table><tr><th>Light Controls</th><th>Temperature Controls</th></tr><tr><td><label for=\"light\">Choose luminosity treshold:</label><select name=\"light\" id=\"light\" form=\"controlz\"><option value=\"30\">Low</option><option value=\"60\">Medium</option><option value=\"90\">High</option></select></td><td><label for=\"temp\">Choose temperature treshold:</label><select name=\"temp\" id=\"temp\" form=\"controlz\"><option value=\"22\">22</option><option value=\"23\">23</option><option value=\"24\">24</option><option value=\"25\">25</option><option value=\"26\">26</option><option value=\"27\">27</option><option value=\"28\">28</option><option value=\"29\">29</option><option value=\"30\">30</option></select></td></tr></table></br><form action=\"http://192.168.0.177\" id=\"controlz\"><input type=\"submit\" value=\"Submit\"></form></div></body></html>";

//трябва да се опрости html-a. ще се махне цсс, ще се махне таблицата, ще се махне и селекта. трябва да остане само формата и две текстови полета.
// стойностите ще се въвеждат като текст и ще се валидират от кода.

int lightStatus = 30;
int tempStatus = 20;

String buffer = "";
String buffer2 = "";
char c;
bool on_off = false;


byte mac[] = { 0xAE, 0xBD, 0xBE, 0xDF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);
EthernetServer server(80);

void setup() {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(ThermistorPin, INPUT);
  pinMode(temt6000Pin, INPUT);
  Serial.begin(9600);

  Serial.println("Ethernet WebServer Example");
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found");
    while (!0) {
      delay(5);
    }
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop()
{

  value = analogRead(temt6000Pin);
  if (value <= lightStatus) {
    digitalWrite(2, LOW); //turn on

  }
  if (value > lightStatus) {
    digitalWrite(2, HIGH); //turn off

  }
  Serial.print("Light ");
  Serial.println(value);

  if ( T <= tempStatus && !on_off) {
    for (int i = 0; i < 3; i++) {
      mySender.sendNEC(0x20DF10EF, 32);
      delay(40);
    }
    on_off=true;
  }
  else if(T >= (tempStatus+3) && on_off)
  {
      for (int i = 0; i < 3; i++) {
        mySender.sendNEC(0x20DF10EF, 32);
        delay(40);
      }
      on_off=false;
  }
  Serial.print("Temp ");
  Serial.println(T);
  
  
  temperature();
  
  buffer = "";
  buffer2 = "";
  ether2();
  delay(1000);
}

void temperature()
{
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;
  delay(1);
}
void ether2 () {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        buffer += c;
        buffer2 += c;
        if (c == '\n' && currentLineIsBlank) {
          client.println(html);
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;

        } else if (c != '\r') {
          if (buffer.indexOf("?light=30") >= 0)
          {
            lightStatus = 30;
            Serial.println(" Light is set at 30");
            buffer = "";
          }
          else if (buffer.indexOf("?light=60") >= 0)
          {
            lightStatus = 60;
            Serial.println("Light is set at 60");
            buffer = "";
          } else if (buffer.indexOf("?light=90") >= 0)
          {
            lightStatus = 90;
            Serial.println("Light is set at 90");
            buffer = "";
          }
          if (buffer2.indexOf("temp=22") >= 0)
          {
            tempStatus = 22;
            Serial.println("Temp is set at 22");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=23") >= 0)
          {
            tempStatus = 23;
            Serial.println("Temp is set at 23");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=24") >= 0)
          {
            tempStatus = 24;
            Serial.println("Temp is set at 24");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=25") >= 0)
          {
            tempStatus = 25;
            Serial.println("Temp is set at 25");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=26") >= 0)
          {
            tempStatus = 26;
            Serial.println("Temp is set at 26");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=27") >= 0)
          {
            tempStatus = 27;
            Serial.println("Temp is set at 27");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=28") >= 0)
          {
            tempStatus = 28;
            Serial.println("Temp is set at 28");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=29") >= 0)
          {
            tempStatus = 29;
            Serial.println("Temp is set at 29");
            buffer2 = "";
          }
          else if (buffer2.indexOf("temp=30") >= 0)
          {
            tempStatus = 30;
            Serial.println("Temp is set at 30");
            buffer2 = "";
          }
          currentLineIsBlank = false;
        }
      }
    }
    delay(100);
    buffer = "";
    buffer2 = "";
    client.stop();
    Serial.println("client disconnected");
  }
}
