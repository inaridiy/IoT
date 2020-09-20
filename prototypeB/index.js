const mosca = require('mosca')
var express = require('express');
var app = express();
const server = new mosca.Server()
var serverhtml = require('http').createServer(app);

app.get("/", function (request, response)
{
    response.sendFile(__dirname + '/index.html');
});
serverhtml.listen(3000);
var io = require('socket.io').listen(serverhtml);
server.on('ready', () => console.log('server started'))

server.on('clientConnected', client =>
    console.log(`client connected: ${client.id}`))

server.on('published', (packet) =>
    console.log(`published: ${JSON.stringify(packet)}`))
const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://localhost')

client.on('connect', () =>
{

})

client.on('message', (topic, msg) =>
{
    console.log(`topic: ${topic}, msg: ${msg}`)

})
io.on('connection', function (socket)
{
    socket.on('red', function (msg)
    {
        console.log("red=" + msg);
        client.publish('esp32/red', msg);
    });
    socket.on('blue', function (msg)
    {
        console.log("blue=" + msg);
        client.publish('esp32/blue', msg);
    });
    socket.on('green', function (msg)
    {
        console.log("green=" + msg);
        client.publish('esp32/green', msg);
    });
});