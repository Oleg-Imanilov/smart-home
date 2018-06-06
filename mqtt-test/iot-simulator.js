const mqtt = require('mqtt')
const CFG = require('./config.json');
const client = mqtt.connect('mqtt://' + CFG.mqttIp);

const cfg = {
    id: 123,
    inp: {b1:'trigger', b2:'trigger', t1: 'int'},
    out: {l1:'bool', d1: 'int'}
};
const state = {};

client.on('connect', function () {
  client.subscribe(`+/+/${cfg.id}/#`);
});

client.on('message', function (topic, message) {
  const [location, room, devId, action] = topic.split('/');
  switch(action) {
      case 'update':
      const m = JSON.parse(message);
      for(const k in m) {
          state[k] = m[k];
      }
      break;
      case 'info':

      break;
  }

});