from mqtt_handler import message_count, latest_value

def get_status():
    return {
        "messages_received": message_count,
        "latest_value": latest_value
    }
