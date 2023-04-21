const cliente = "Liam";
// client = new Paho.MQTT.Client("192.168.246.77", 9001, cliente);
client = new Paho.MQTT.Client("172.18.0.91", 9001, cliente);

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
    var div = document.getElementById("chat");
    div.innerHTML = div.innerHTML + `<p>${message.payloadString}</p>`;
    console.log("onMessageArrived:"+message.payloadString);
}

function sendMessage(){
    client.subscribe("World");
    message = new Paho.MQTT.Message(`${cliente}: ${document.getElementById("mensaje").value}`);
    message.destinationName = "World";
    client.send(message);

    document.getElementById("mensaje").value = "";
}