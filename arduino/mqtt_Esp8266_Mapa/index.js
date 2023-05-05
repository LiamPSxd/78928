// MQTT
client = new Paho.MQTT.Client("192.168.246.237", 9001, "Liam");

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect({ onSuccess:onConnect });

function onConnect(){
  console.log("onConnect");
  client.subscribe("/data");
  message = new Paho.MQTT.Message("Hello");
  message.destinationName = "World";
}

function onConnectionLost(responseObject){
  if(responseObject.errorCode !== 0)
    console.log("onConnectionLost:" + responseObject.errorMessage);
}

function onMessageArrived(message){
    var data = JSON.parse(message.payloadString);

    document.getElementById("id").innerHTML = data.id;
    document.getElementById("latitud").innerHTML = data.latitud;
    document.getElementById("longitud").innerHTML = data.longitud;
    document.getElementById("humedad").innerHTML = data.humedad;
    document.getElementById("tempC").innerHTML = data.temperaturaC;
    document.getElementById("tempF").innerHTML = data.temperaturaF;
    document.getElementById("iCC").innerHTML = data.iCC;
    document.getElementById("iCF").innerHTML = data.iCF;

    enviar();
}

// LEAFLETJS
var map = L.map('map').setView([19.546440, -96.904226], 13);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);

var marker = new L.Marker([0, 0]).addTo(map);

const enviar = () => {
    const id = document.getElementById("id").value;
    const humedad = document.getElementById("humedad").value;

    marker.setLatLng(L.latLng(
      document.getElementById("latitud").value,
      document.getElementById("longitud").value
    ));
    marker.bindPopup(`<b>${id}</b><br><strong>Humedad</strong>${humedad}`).openPopup();
};