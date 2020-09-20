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

const topic = "esp32"

client.on('connect', () =>
{
    client.subscribe("esp32temp")
    client.subscribe("esp32hum")
    client.subscribe("esp32pres")
})
let temp = [], hum = [], pres = [];
let weather = [temp, hum, pres]
client.on('message', (topic, msg) =>
{
    console.log(`topic: ${topic}, msg: ${msg}`)
    msg = Number(msg)
    console.log(topic)
    if (topic == "esp32temp")
    {
        console.log("gettemp");
        temp.push(msg);

    } else if (topic == "esp32hum")
    {
        console.log("gethum");
        hum.push(msg);

    } else
    {
        console.log("getpres");
        pres.push(msg);

    }

})
io.on('connection', function (socket)
{
    socket.on('weather', function (msg)
    {
        io.emit('weather', weather)
    });
});