var net = require('net');
var os=require('os');

var HOST = '10.105.128.210';
var SENSORPORT = 8080;
var APPPORT = 8000;

var networkInterfaces=os.networkInterfaces();
var count = 1;

global.globalString = "shared string";

// Listen to SENSORPORT
console.log('Server listening on ' + HOST + ':' + SENSORPORT);
net.createServer(function(sock) {
  console.log('SENSOR CONNECTED: ' + sock.remoteAddress + ':' + sock.remotePort);
  sock.on('data', function(data) {
    globalString = data.toString('utf8');
    console.log('SENSOR DATA ' + sock.remoteAddress + ': ' + data);
    console.log(globalString);
    sock.write('You said "' + data + '"');
  });
  sock.on('close', function(data) {
    console.log('SENSOR CLOSED: ' + sock.remoteAddress +' '+ sock.remotePort);
  });
}).listen(SENSORPORT, HOST);

// callback for app socket
// reference:
// http://helloraspberrypi.blogspot.com/2014/03/create-tcp-server-using-nodejs-with-net.html
function callback_server_connection(socket){
    var remoteAddress = socket.remoteAddress;
    var remotePort = socket.remotePort;
    socket.setNoDelay(true);
    console.log("connected: ", remoteAddress, " : ", remotePort);

    var msg = 'Hello ' + remoteAddress + ' : ' +  remotePort + '\r\n'
        + "You are #" + count + ' ' + globalString + '\r\n';
    count++;

    socket.end(msg);

    socket.on('data', function (data) {
        console.log(data.toString());
    });

    socket.on('end', function () {
        console.log("ended: ", remoteAddress, " : ", remotePort);
    });
}

console.log("http://android-er.blogspot.com/");
console.log("http://helloraspberrypi.blogspot.com/");

console.log("node.js net server is waiting:");
for (var interface in networkInterfaces) {

    networkInterfaces[interface].forEach(function(details){

        if ((details.family=='IPv4') && !details.internal) {
            console.log(interface, details.address);
        }
    });
}

console.log("APPPORT: ", APPPORT);

// listen to APPPORT:
var netServer = net.createServer(callback_server_connection);
netServer.listen(APPPORT);
