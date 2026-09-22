import logging
from mqtt_handler import start_mqtt
from api import app

logging.basicConfig(level=logging.INFO)

if __name__ == "__main__":
    start_mqtt()
    app.run(host="0.0.0.0", port=8443, debug=True)
