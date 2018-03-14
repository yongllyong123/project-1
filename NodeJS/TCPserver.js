var net = require('net');
var os=require('os');

var HOST = '10.105.128.210';
var SENSORPORT = 8080;
var APPPORT = 8000;

var networkInterfaces=os.networkInterfaces();
var count = 1;

global.globalString1 = "shared string";
global.globalString2 = "shared string";

// TO-DO: automatically add addresses to an abritrarily large table
global.arduino1Address = "10.106.9.208";
global.arduino2Address = "10.106.19.249";

global.A1Sensor1CloseEnough = 0;
global.A1Sensor2CloseEnough = 0;
global.A2Sensor1CloseEnough = 0;
global.A2Sensor2CloseEnough = 0;

global.A1_timesince = 0;
global.A2_timesince = 0;
global.A1_timeprev = 0;
global.A2_timeprev = 0;

global.emptyLot = 0;
global.count = 0;

global.distThresh = 50; // 50 cm distance threshold

function vehicle_detect() {
if(emptyLot == 1) { //parking spot is not used
   if(A1Sensor2CloseEnough && A2Sensor1CloseEnough) {// close enough to both sensors
     count++; // increment counter
     if(count > 5) { // threshold depends on Arduino read frequency
       count = 0; // reset counter before changing state
       emptyLot = 0; // parking spot in use
     } else {
      emptyLot = 1; // parking spot empty
     }
   } else {
     count = 0; // still empty, so don't change count
     emptyLot = 1; // still empty
   }
 } else { // parking spot in use
   if ((!A1Sensor2CloseEnough) || (!A2Sensor1CloseEnough)) { // not close enough to at least one sensor
     count++;
     if(count > 5) { // threshold depends on Arduino read frequency
       count = 0;
       emptyLot = 1;
     } else {
       emptyLot = 0;
     }
   } else {
     count = 0;
     emptyLot = 0;
   }
 }
}

// Listen to SENSORPORT
console.log('Server listening on ' + HOST + ':' + SENSORPORT);
var start = new Date();
A1_timeprev = start;
A2_timeprev = start;
console.log('Start time: %d',start);

net.createServer(function(sock) {
  console.log('SENSOR CONNECTED: ' + sock.remoteAddress + ':' + sock.remotePort);
  sock.on('data', function(data) {
    console.log('SENSOR DATA ' + sock.remoteAddress + ': ' + data);
    if (sock.remoteAddress==arduino1Address) {
      console.log("You are Arduino 1");
      globalString1 = data.toString('utf8');
      A1_timesince = new Date() - A1_timeprev;
      A1_timeprev = A1_timesince;
      console.log(globalString1);
      A1Sensor1CloseEnough = data[0] - 48; // ASCII offset: 0 (char) is 48 (dec)
      A1Sensor2CloseEnough = data[2] - 48; // there is a space between, so data[1] encodes the space.
      // TO-DO: do something useful with A1_timesince;
    }
    else if (sock.remoteAddress==arduino2Address) {
      console.log("You are Arduino 2");
      globalString2 = data.toString('utf8');
      A2_timesince = new Date() - A2_timeprev;
      A2_timeprev = A2_timesince;
      console.log(globalString2);
      A2Sensor1CloseEnough = data[0] - 48; // ASCII offset: 0 (char) is 48 (dec)
      A2Sensor2CloseEnough = data[2] - 48; // there is a space between, so data[1] encodes the space.
      // TO-DO: do something useful with A2_timesince;
    }
    else {
      console.log("Roger Daltrey wants to know 'whoooooooo are you?'");
      // data is not stored
    }

    console.log("A1S1: " + A1Sensor1CloseEnough + ", A1S2: " + A1Sensor2CloseEnough + ".");
    console.log("A2S1: " + A2Sensor1CloseEnough + ", A2S2: " + A2Sensor2CloseEnough + ".");

    vehicle_detect();
    console.log("Empty: " + emptyLot + ", count: " + count + ".");

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
        + "A1S1: " + A1Sensor1CloseEnough + ", A1S2: " + A1Sensor2CloseEnough + "." + '\r\n'
        + "A2S1: " + A2Sensor1CloseEnough + ", A2S2: " + A2Sensor2CloseEnough + "." + '\r\n'
        + "Empty: " + emptyLot + ", count: " + count + "." + '\r\n';

    socket.end(msg);

    socket.on('data', function (data) {
        console.log(data.toString());
    });

    socket.on('end', function () {
        console.log("ended: ", remoteAddress, " : ", remotePort);
    });
}

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
