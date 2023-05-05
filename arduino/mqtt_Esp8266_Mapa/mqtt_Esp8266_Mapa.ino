#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

#define DHTpin 15
DHTesp dht;

const char* ssid = "CatBenderson";
const char* password = "ranita987";
const char* mqtt_server = "192.168.246.237";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
float value = 19.55;
int id = 0;

DynamicJsonDocument json(1024);
char respuesta[250];

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
  for(int i=0; i<length; i++){
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

    String clientId = "Blanket";
    clientId += String(random(0xffff), HEX);

    if(client.connect(clientId.c_str())){
      Serial.println("connected");

      client.publish("fei/cc1/temperatura", "temperatura");
      client.subscribe("inTopic");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void createJSON(int id){
  json.clear();
  json["id"] = id;
  json["humedad"] = dht.getHumidity();
  json["temperaturaC"] = dht.getTemperature();
  json["temperaturaF"] = dht.toFahrenheit(dht.getTemperature());
  json["iCC"] = dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false);
  json["iCF"] = dht.computeHeatIndex(dht.toFahrenheit(dht.getTemperature()), dht.getHumidity(), true);
  json["latitud"] = value;
  json["longitud"] = -96.904226;
  serializeJson(json, respuesta);
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
  if(now - lastMsg > 7000){
    lastMsg = now;
    value = value + .000015;

    createJSON(id);
    client.publish("/data", respuesta);
  }
}