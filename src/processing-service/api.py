from flask import Flask, jsonify, request
import mqtt_handler
import os

app = Flask(__name__)

API_KEY = os.getenv("API_KEY", "my-secret-key")

@app.route("/api/v1/temperature", methods=["GET"])
def get_temperature():

    key = request.headers.get("X-API-Key")

    if key != API_KEY:
        return jsonify({"error": "unauthorized"}), 401

    if mqtt_handler.latest_value is None:
        return jsonify({"error": "no data"}), 404

    return jsonify(mqtt_handler.latest_value), 200