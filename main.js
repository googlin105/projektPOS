var express = require("express");
var app = express();
var path = require("path");
var server = require("http").createServer(app);
var io = require("socket.io")(server);
var port = process.env.PORT || 8200;
var net = require("net");

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

  var client = net.createConnection({ port: 9001 }, () => {
    console.log("connected to server!");
    client.write("world!\r\n");
  });

  client.on("data", (data) => {
    console.log(data.toString());
    socket.emit("tcp_data", data.toString());
  });
  client.on("end", () => {
    console.log("disconnected from server");
  });
  client.on("error", (e) => {
    console.log(e);
  });
});
