const API_URL = "http://192.168.0.105";
const contenido = document.getElementById('contenido');

function cargarContenido(ruta){
    fetch(ruta)
    .then(respuesta => respuesta.text())
    .then(data => {contenido.innerHTML = data})
    .catch(error => console.error(error));

    cargarContenidoDelContenido(ruta);
}

function cargarContenidoDelContenido(ruta){
    switch(ruta){
        case "estado.html":
            fetch(`${API_URL}/estado`)
            .then(respuesta => respuesta.json())
            .then(data => setInfo("estadoText", data.message))
            .catch(error => console.log(error));
            break;
        case "humedad.html":
            fetch(`${API_URL}/humedad`)
            .then(respuesta => respuesta.json())
            .then(data => setInfo("humedadText", data.humedad))
            .catch(error => console.log(error));
            break;
        case "temperatura.html":
            fetch(`${API_URL}/temperatura/celsius`)
            .then(respuesta => respuesta.json())
            .then(data => setInfo("tempC", data.temperaturaC))
            .catch(error => console.log(error));    
        
            fetch(`${API_URL}/temperatura/fahrenheit`)
            .then(respuesta => respuesta.json())
            .then(data => setInfo("tempF", data.temperaturaF))
            .catch(error => console.log(error));
            break;
    }
}

function setInfo(idDoc, valor){
    document.getElementById(idDoc).innerHTML = valor;
}

function indiceF(){
    fetch(`${API_URL}/indice/fahrenheit`)
    .then(respuesta => respuesta.json())
    .then(data => setInfo("indiceText", data.indiceF))
    .catch(error => console.log(error));
}

function indiceC(){
    fetch(`${API_URL}/indice/celsius`)
    .then(respuesta => respuesta.json())
    .then(data => setInfo("indiceText", data.indiceC))
    .catch(error => console.log(error));
}

function manejarCambioURL(){
    const ruta = location.hash.substr(1) || '/';
    cargarContenido(`${ruta}.html`);
}

cargarContenido("estado.html");

window.addEventListener('hashchange', manejarCambioURL);