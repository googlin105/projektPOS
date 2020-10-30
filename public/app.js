const lat = 0;
const lon = 0;
const objects = new Map();
let doOnce = 1;

let mymap = L.map("mapid").setView([lat, lon], 2);
const marker = [L.marker([lat, lon]).addTo(mymap)];

L.tileLayer(
  "https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}",
  {
    attribution:
      'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, <a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
    maxZoom: 18,
    id: "mapbox/streets-v11",
    tileSize: 512,
    zoomOffset: -1,
    accessToken:
      "pk.eyJ1Ijoic3RlZWV3IiwiYSI6ImNrZ203NW5ubTEwNjIydW5hdHF2am9ndWkifQ.4hltR0t-0nik0FZIz9Nlcw",
  }
).addTo(mymap);
L.marker([lat, lon]).addTo(mymap);

document.getElementById("latitude").textContent = lat;
document.getElementById("longitude").textContent = lon;

var socket = io();

socket.on("connect", () => {
  console.log("user connected");
  socket.on("tcp_data", (data) => {
    const objects = new Map(data);
    console.log(objects);
    console.log(objects.size);

    if (doOnce) {
      for (j = 1; j <= objects.size; j++) {
        let values = objects.get(j);
        console.log(`id: ${j}` + values.lat);
        marker.push(L.marker([values.lat, values.lon]).addTo(mymap));
      }
      doOnce = 0;
    }
    console.log(marker);

    for (j = 1; j <= objects.size; j++) {
      let values = objects.get(j);
      const latlng = L.latLng(values.lat, values.lon);
      console.log(latlng);
      marker[j].setLatLng(latlng);
    }

    // async function getPosition() {
    //   const location = data.get(1);
    //   // const lat = parsed.x_pos;
    //   // const lon = parsed.y_pos;
    //   // const latlng = L.latLng(lat, lon);
    //   // marker.setLatLng(latlng);
    //   console.log(location);
    //   document.getElementById("latitude").textContent = lat;
    //   document.getElementById("longitude").textContent = lon;
    // }

    // getPosition(data);
    // setInterval(getPosition, 2000);
  });
});
