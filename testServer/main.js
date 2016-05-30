var http = require('http');

const PORT=8080;
var stats = {};

function outputStats(response) {
  response.end(JSON.stringify(stats));
}

function addPathToStats(path) {
  console.log(stats)
  if (stats[path]){
    stats[path] += 1;
  } else {
    stats[path] = 1;
  }
}

function handleRequest(request, response){
  if(request.url == "/stats"){
    outputStats(response);
    return;
  }
    console.log('Recieved request: ' + request.url);
    addPathToStats(request.url);
    response.end('1 success');
}

var server = http.createServer(handleRequest);

const bindAddress = "0.0.0.0";
server.listen(PORT, bindAddress, function(){
    console.log("Server listening on: http://%s:%s", bindAddress, PORT);
});
