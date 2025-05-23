#include <IRremote.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

#define PIN_RECEIVER 12   // Signal Pin of IR receiver

int Relay = 15; //Realy Pin 

//variaveis do tempo
long timezone = -4;
byte daysavetime = 1;

//variaveis do wifi
const char* ssid = "Wokwi-GUEST";
const char* senha = "";
const char* topic = "";//<- colocar o topic 

//const String url = "";//<-link do HTTP nao sei se irei utilizar
//TTPClient client;//configurao do http

//mqtt
const char* mqtt_broker = "";//<-link do broker mqtt
const int mqtt_port = ;//<-link da porta do broker utilizado

//instancias necessarias para mqtt
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

IRrecv receiver(PIN_RECEIVER);

void setup() {
  Serial.begin(115200);

  receiver.enableIRIn(); //config do sensor ir
  pinMode(Relay, OUTPUT); //config do relay

  //config do wifi
  WiFi.begin(ssid, senha);
  Serial.println("Conectando a rede WiFi");
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }

  Serial.println();
  Serial.println("Conectado");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //configuracao mqtt
  mqttclient.setServer(mqtt_broker, mqtt_port);
  Serial.println("Conectando ao broker MQTT");
  while (!mqttclient.connected()){
    if(mqttclient.connect("ESP32Client8787afd")){
      Serial.println("Conectado ao broker MQTT");
    } else {
      Serial.println("Falha na conexão, rc=");
      delay(2000);
    }
  }
  //config do tempo
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

}

void loop() {

//Receive the IR Code and print in Serial monitor
  if (receiver.decode()) {
    Serial.println(receiver.decodedIRData.command);
  }

  //Turn on
  if (receiver.decodedIRData.command == 162){ 
    digitalWrite(Relay, HIGH);
  }//Turn off
  else if (receiver.decodedIRData.command == 2){ 
    digitalWrite(Relay, LOW);
  }
  
  receiver.resume();  // Receive the next value

}

//methods

//tempo
String getHour(){
  struct tm tmstruct ; 

  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct);

  String hour = (String(tmstruct.tm_hour) + ":" + String(tmstruct.tm_min) + ":" + String(tmstruct.tm_sec));

  return hour;
}