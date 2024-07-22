from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Allow CORS for all origins

# Sample data
data = {
    "temperature": 20.0,
    "fan_speed": "medium",
    "ac_status": "Off"
}

@app.route('/temperature', methods=['GET'])
def get_temperature():
    return jsonify({"current_temperature": data["temperature"]})

@app.route('/update_temperature', methods=['POST'])
def update_temperature():
    content = request.json
    data["temperature"] = content.get("temperature", data["temperature"])
    return jsonify({"status": "success", "message": "Temperature updated"})

@app.route('/fan_speed', methods=['GET'])
def get_fan_speed():
    return jsonify({"fan_speed": data["fan_speed"]})

@app.route('/set_fan_speed', methods=['POST'])
def set_fan_speed():
    content = request.json
    data["fan_speed"] = content.get("fan_speed", data["fan_speed"])
    return jsonify({"status": "success", "message": "Fan speed set"})

@app.route('/ac_status', methods=['GET'])
def get_ac_status():
    return jsonify({"ac_status": data["ac_status"]})

@app.route('/toggle_ac', methods=['POST'])
def toggle_ac():
    data["ac_status"] = "On" if data["ac_status"] == "Off" else "Off"
    return jsonify({"status": "success", "ac_status": data["ac_status"]})

if __name__ == '__main__':
    app.run(debug=True)
