var map = L.map('map').setView([19.5324, -96.91589], 13);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);

var marker = null;

const enviar = () => {
    const lat = document.getElementById("lat").value;
    const lon = document.getElementById("lon").value;
    const mensaje = document.getElementById("mensaje").value;

    marker = L.marker([lat, lon]).addTo(map);
    marker.bindPopup(`<b>¡Mensaje!</b><br>${mensaje}`).openPopup();
    map.remove();

    resetear();
};

const resetear = () => {
    document.getElementById("lat").value = "";
    document.getElementById("lon").value = "";
    document.getElementById("mensaje").value = "";
};