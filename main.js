var express = require("express");
var app = express();
var path = require("path");
var server = require("http").createServer(app);
var io = require("socket.io")(server);
var port = process.env.PORT || 8200;
var net = require("net");

const objects = new Map();

server.listen(port, () => {
  console.log("Server listening at port %d", port);
});
app.use(express.static(path.join(__dirname, "public")));

io.on("connection", (socket) => {
  console.log(`user connect with id: ${socket.id}`);
  socket.emit("user_connected", { data: socket.id });
  socket.on("disconnect", () => {
    console.log(`user disconnect with id: ${socket.id}`);
  });

  var client = net.createConnection({ port: 9002 }, () => {
    console.log("connected to server!");
    client.write("world!\r\n");
  });

  client.on("data", (data) => {
    const strData = data.toString();
    const parsed = JSON.parse(strData);
    objects.set(parsed.id, { lat: parsed.x_pos, lon: parsed.y_pos });
  });

  client.on("end", () => {
    console.log("disconnected from server");
  });

  client.on("error", (e) => {
    console.log(e);
  });

  setTimeout(() => {
    async function sendData() {
      console.log(objects);
      socket.emit("tcp_data", Array.from(objects));
    }
    setInterval(sendData, 500);
  }, 3000);
});
