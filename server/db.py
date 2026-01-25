import sqlite3
from server.config import DB

# Connects to the SQLite database
def get_conn():
    conn = sqlite3.connect(DB, timeout=10)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA journal_mode=WAL;")
    return conn

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
        conn.execute("""CREATE INDEX IF NOT EXISTS idx_sensor_ts ON sensor_data(ts)""")
        conn.commit()