let lat = 54;
let lon = 18;

var mymap = L.map("mapid").setView([lat, lon], 13);
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
var marker = L.marker([lat, lon]).addTo(mymap);

document.getElementById("latitude").textContent = lat;
document.getElementById("longitude").textContent = lon;

var socket = io();
// socket.on("user_connected", (data) => {
//   console.log(data);
// });

let test = {
  id: 1,
  x_pos: 2,
};
console.table(test);

socket.on("connect", () => {
  console.log("user connected");
  socket.on("tcp_data", (data) => {
    console.log(JSON.parse(data));
  });
});
