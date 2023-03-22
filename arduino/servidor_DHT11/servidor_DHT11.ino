#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>
#include "DHTesp.h"
#define DHTpin 15

DHTesp dht;

const char* ssid = "dr-rojano";
const char* password = "78928-tw";
int ban = 0;

AsyncWebServer server(80);

StaticJsonDocument<250> json;
char buffer[250];

void setup(){
  Serial.begin(115200);

  conectarse();
  dht.setup(DHTpin, DHTesp::DHT11);
  pinMode(2, OUTPUT);

  server.on("/estado", HTTP_GET, [](AsyncWebServerRequest *request){
    if(dht.getStatusString() == "OK"){
      createJSON("message", "Conectado con el Sensor DHT11");
      request->send(200, "application/json", buffer);
      ban = 1;
    }else{
      createJSON("message", "No se pudo conectar con el sensor DHT11");
      request->send(200, "application/json", buffer);
      digitalWrite(2, LOW);
    }
  });

  server.on("/humedad", HTTP_GET, [](AsyncWebServerRequest *request){
    float humedad = dht.getHumidity();
    ban = 0;

    if(isnan(humedad)) fallido(request);
    else{
      createJSON("humedad", String(humedad));
      request->send(200, "application/json", buffer);
      digitalWrite(2, HIGH);
    }
  });

  server.on("/temperatura/celsius", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperaturaC = dht.getTemperature();
    ban = 0;

    if(isnan(temperaturaC)) fallido(request);
    else{
      createJSON("temperaturaC", String(temperaturaC));
      request->send(200, "application/json", buffer);
      digitalWrite(2, HIGH);
    }
  });

  server.on("/temperatura/fahrenheit", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperaturaF = dht.toFahrenheit(dht.getTemperature());
    ban = 0;

    if(isnan(temperaturaF)) fallido(request);
    else{
      createJSON("temperaturaF", String(temperaturaF));
      request->send(200, "application/json", buffer);
      digitalWrite(2, HIGH);
    }
  });

  server.on("/indice/celsius", HTTP_GET, [](AsyncWebServerRequest *request){
    float iCC = dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false);
    ban = 0;

    if(isnan(iCC)) fallido(request);
    else{
      createJSON("indiceC", String(iCC));
      request->send(200, "application/json", buffer);
      digitalWrite(2, HIGH);
    }
  });

  server.on("/indice/fahrenheit", HTTP_GET, [](AsyncWebServerRequest *request){
    float iCF = dht.computeHeatIndex(dht.toFahrenheit(dht.getTemperature()), dht.getHumidity(), true);
    ban = 0;

    if(isnan(iCF)) fallido(request);
    else{
      createJSON("indiceF", String(iCF));
      request->send(200, "application/json", buffer);
      digitalWrite(2, HIGH);
    }
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop(){
  delay(dht.getMinimumSamplingPeriod());

  if(ban == 1){
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
  }
}
  
void conectarse(){
  Serial.print("Conectando a...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void fallido(AsyncWebServerRequest *request){
  createJSON("message", "¡No se pudo leer el sensor DHT11!");
  request->send(200, "application/json", buffer);
  digitalWrite(2, LOW);
}

void createJSON(String clave, String valor){
  json.clear();
  json[clave] = valor;
  serializeJson(json, buffer);
}