import paho.mqtt.client as mqtt
from kafka import KafkaProducer

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("testok")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    producer = KafkaProducer(bootstrap_servers=['localhost:9092'])
    
    future = producer.send('yui', msg.payload)
    

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.0.5", 1883, 60)
client.username_pw_set(username="raakesh",password="helloworld")


# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
