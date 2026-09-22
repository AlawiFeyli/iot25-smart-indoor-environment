import json
import logging
import paho.mqtt.client as mqtt

latest_value = None
message_count = 0

MQTT_HOST = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "iot25/indoor/temperature"

logger = logging.getLogger("mqtt_handler")

def on_connect(client, userdata, flags, rc):
    logger.info(f"MQTT connected with result code {rc}")
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    global latest_value, message_count

    try:
        payload = json.loads(msg.payload.decode())
        latest_value = payload
        message_count += 1
        logger.info(f"Received MQTT message: {payload}")

    except Exception as e:
        logger.error(f"Invalid JSON received: {e}")

def start_mqtt():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(MQTT_HOST, MQTT_PORT, 60)
    client.loop_start()

    return client
