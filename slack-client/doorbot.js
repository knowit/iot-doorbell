const mqtt = require('mqtt')
const { IncomingWebhook } = require('@slack/client')
const settings = require('./settings')

const webhook = new IncomingWebhook(settings.slackUrl)
const client = mqtt.connect('mqtt://' + settings.mqttBroker)

console.log('Connecting to MQTT broker ' + settings.mqttBroker)

client.on('message', (topic, message) => {
  console.log('Incoming message')
  if(topic === settings.mqttTopic) {
    console.log('on topic ' + settings.mqttTopic)
    console.log('Message: ' + message.toString())
    webhook.send('TEST: Det ringer på! :door:', function(err, res) {
      if (err) {
          console.log('Error:', err)
      } else {
          console.log('Message sent to Slack: ', res)
      }
  })    
  }
})

client.on('connect', () => {
  console.log('Connected')
  client.subscribe(settings.mqttTopic)
  console.log('Subscribed to topic ' + settings.mqttTopic)
  console.log('Ready and waiting for messages!')
})


