#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

#define DHTpin 15
DHTesp dht;

const char* ssid = "OPPO A54";
const char* password = "ranita321";
const char* mqtt_server = "https://ip172-18-0-53-ch5v53qe69v000f953ig@direct.labs.play-with-docker.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int id = 0;

StaticJsonDocument<250> json;
char buffer[250];

void setup_wifi(){
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if((char)payload[0] == '1'){
    digitalWrite(2, LOW);
  }else{
    digitalWrite(2, HIGH);
  }
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if(client.connect(clientId.c_str())){
      float humedad = dht.getHumidity();
      float temperaturaC = dht.getTemperature();
      float temperaturaF = dht.toFahrenheit(dht.getTemperature());
      float iCC = dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false);
      float iCF = dht.computeHeatIndex(dht.toFahrenheit(dht.getTemperature()), dht.getHumidity(), true);

      Serial.println("connected");

      createJSON(String(id), String(humedad), String(temperaturaC), String(temperaturaF), String(iCC), String(iCF));
      client.publish("data", buffer);
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void createJSON(String id, String humedad, String temperaturaC, String temperaturaF, String iCC, String iCF){
  json.clear();
  json["id"] = id;
  json["humedad"] = humedad;
  json["temperaturaC"] = temperaturaC;
  json["temperaturaF"] = temperaturaF;
  json["iCC"] = iCC;
  json["iCF"] = iCF;
  json["latitud"] = 19.59;
  json["longitud"] = -96.94;
  serializeJson(json, buffer);
}

void setup(){
  dht.setup(DHTpin, DHTesp::DHT11);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if(now - lastMsg > 2000){
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}