from flask import Flask, request, jsonify, render_template, url_for
import sqlite3
from datetime import datetime, timezone


app = Flask(__name__)
DB = 'plants.db'

# Connects to the SQLite database
def get_conn():
    return sqlite3.connect(DB, check_same_thread=False)

# Initializes the database and creates the sensor_data table if it doesn't exist
def init_db():
    with get_conn() as conn:
        conn.execute("""
        CREATE TABLE IF NOT EXISTS sensor_data (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ts TEXT NOT NULL,
        device TEXT NOT NULL,
        light INTEGER,
        temp REAL,
        humidity REAL,
        basil INTEGER,
        spring_onion INTEGER,
        rosemary INTEGER
        )
        """)

# =======================================
# ========== Main Endpoints =============
# =======================================

@app.route('/sensor', methods=['POST'])
def receive_sensor_data():
    data = request.get_json()

    ts = datetime.now(timezone.utc).isoformat()
    device = data.get('device', 'unknown')

    light = data.get('light')
    temp = data.get('temp')
    humidity = data.get('humidity')

    soil = data.get('soil_moisture', {})
    basil = soil.get('basil')
    spring_onion = soil.get('spring_onion')
    rosemary = soil.get('rosemary')

    with get_conn() as conn:
        conn.execute("""
        INSERT INTO sensor_data 
        (ts, device, light, temp, humidity, basil, spring_onion, rosemary)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        """, (ts, device, light, temp, humidity, basil, spring_onion, rosemary))
    
    return jsonify({"status": "success", "ts": ts})

@app.route('/')
def index():
    return render_template('index.html')

# =======================================
# ========== Testing Endpoints ==========
# =======================================

# Test endpoint to get the latest sensor data
@app.route("/latest")
def latest():
    with get_conn() as conn:
        row = conn.execute("""
            SELECT ts, device, light, temp, humidity, basil, spring_onion, rosemary
            FROM sensor_data
            ORDER BY id DESC
            LIMIT 1
        """).fetchone()

    if row is None:
        return jsonify({"status": "empty"})

    keys = ["ts","device","light","temp","humidity","basil","spring_onion","rosemary"]
    return jsonify(dict(zip(keys, row)))

# Test endpoint to delete the latest sensor data
@app.route("/delete_latest", methods=["POST"])
def delete_latest():
    with get_conn() as conn:
        conn.execute("""
        DELETE FROM sensor_data
        WHERE id = (
            SELECT id FROM sensor_data
            ORDER BY id DESC
            LIMIT 1
        )
    """)
    conn.commit()
    conn.close()

    return jsonify({"status": "deleted latest"})

# Run the app    
if __name__ == "__main__":
    init_db()
    app.run(host="0.0.0.0", port=5000, debug=True)
