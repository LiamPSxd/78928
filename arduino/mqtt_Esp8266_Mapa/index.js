// MQTT
const cliente = "Liam";
// client = new Paho.MQTT.Client("192.168.246.77", 9001, cliente);
client = new Paho.MQTT.Client("ip172-18-0-53-ch5v53qe69v000f953ig@direct.labs.play-with-docker.com", 9001, cliente);

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect({onSuccess:onConnect});

function onConnect() {
  console.log("onConnect");
  client.subscribe("World");
  message = new Paho.MQTT.Message("Hello");
  message.destinationName = "World";
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

function onMessageArrived(message) {
    enviar(message.payloadString);
    console.log("onMessageArrived:"+message.payloadString);
}

function sendMessage(){
    client.subscribe("data");
    message = new Paho.MQTT.Message(`${cliente}: ${document.getElementById("mensaje").value}`);
    message.destinationName = "data";
    client.send(message);

    document.getElementById("mensaje").value = "";
}

// LEAFLETJS
var map = L.map('map').setView([19.5324, -96.91589], 13);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);

var marker = null;

const enviar = (message) => {
    const lat = document.getElementById("lat").value;
    const lon = document.getElementById("lon").value;
    const mensaje = document.getElementById("mensaje").value;

    marker = L.marker([message.latitud, message.longitud]).addTo(map);
    marker.bindPopup(`<b>${message.id}</b><br><strong>Humedad</strong>${humedad}`).openPopup();

    resetear();
};

const resetear = () => {
    document.getElementById("lat").value = "";
    document.getElementById("lon").value = "";
    document.getElementById("mensaje").value = "";
};