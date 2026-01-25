import db
import sqlite3
from datetime import timezone, datetime
from flask import jsonify, request

def receive_sensor_data():
    data = request.get_json(silent=True) or {}

    ts = datetime.now(timezone.utc).isoformat()
    device = data.get('device', 'unknown')

    light = data.get('light')
    temp = data.get('temp')
    humidity = data.get('humidity')

    soil = data.get('soil_moisture', {})
    basil = soil.get('basil')
    spring_onion = soil.get('spring_onion')
    rosemary = soil.get('rosemary')

    with db.get_conn() as conn:
        conn.execute("""
        INSERT INTO sensor_data 
        (ts, device, light, temp, humidity, basil, spring_onion, rosemary)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        """, (ts, device, light, temp, humidity, basil, spring_onion, rosemary))
        conn.commit()
    
    return jsonify({"status": "success", "ts": ts})

def latest():
    with db.get_conn() as conn:
        row = conn.execute("""
            SELECT *
            FROM sensor_data
            ORDER BY id DESC
            LIMIT 1
        """).fetchone()

    if row is None:
        return jsonify({"status": "empty"})

    return jsonify(dict(row))

def delete_latest():
    with db.get_conn() as conn:
        conn.execute("""
        DELETE FROM sensor_data
        WHERE id = (
            SELECT id FROM sensor_data
            ORDER BY id DESC
            LIMIT 1
        )
    """)
    conn.commit()

    return jsonify({"status": "deleted latest"})