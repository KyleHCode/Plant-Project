from flask import Flask, request, jsonify, render_template, url_for
import sqlite3
from datetime import datetime, timezone
import db
import handlers

app = Flask(__name__)

# =======================================
# ========== Main Endpoints =============
# =======================================

@app.route('/sensor', methods=['POST'])
def sensor():
    return handlers.receive_sensor_data()

@app.route('/')
def index():
    return render_template('index.html')

# =======================================
# ========== Testing Endpoints ==========
# =======================================

# Test endpoint to get the latest sensor data
@app.route("/latest")
def latest():
    return handlers.latest()
# Test endpoint to delete the latest sensor data
@app.route("/delete_latest", methods=["POST"])
def delete_latest():
    return handlers.delete_latest()

# Run the app    
if __name__ == "__main__":
    db.init_db()
    app.run(host="0.0.0.0", port=5000, debug=False)
