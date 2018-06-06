const fs = require('fs')
const mqtt = require('mqtt')

const CFG = require('./config.json');

const client = mqtt.connect('mqtt://' + CFG.mqttIp);

client.on('connect', function () {
  client.subscribe('#');
});

const rules = [];
const store = {};

fs.readdir('rules', (err, items) => {
  for (var i=0; i<items.length; i++) {
    rules.push(require(`rules/${items[i]}`));
  }
});

client.on('message', function (topic, message) {
  const [location, room, devId, action] = topic.split('/');
  const m = JSON.parse(message);
  rules.forEach((r) => {
    r(store, location, room, devId, action, m);
  });
});