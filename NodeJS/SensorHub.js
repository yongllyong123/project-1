var request = require("request");

function intervalFunc() {
  request({
    // uri: "http://10.106.19.249",
    uri: "http://10.106.9.208",
  }, function(error, response, body) {
    console.log(body);
  });
}

setInterval(intervalFunc, 1000);
