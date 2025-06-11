import sqlite3
from datetime import datetime


def connect(database_name):
    global conn
    conn = sqlite3.connect(database_name)


def create_database():
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS button_stats (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            Right INTEGER DEFAULT 0,
            Down INTEGER DEFAULT 0,
            Up INTEGER DEFAULT 0,
            Left INTEGER DEFAULT 0,
            Square INTEGER DEFAULT 0,
            Cross INTEGER DEFAULT 0,
            Circle INTEGER DEFAULT 0,
            Triangle INTEGER DEFAULT 0,
            L1 INTEGER DEFAULT 0,
            R1 INTEGER DEFAULT 0,
            L3 INTEGER DEFAULT 0,
            R3 INTEGER DEFAULT 0,
            Share INTEGER DEFAULT 0,
            Options INTEGER DEFAULT 0,
            PS INTEGER DEFAULT 0,
            Touchpad INTEGER DEFAULT 0
        )
    ''')
    cursor.execute("INSERT INTO button_stats DEFAULT VALUES")
    conn.commit()


def update_buttons(buttons: dict):
    cursor = conn.cursor()

    for button, value in buttons.items():
        cursor.execute(f'''
                UPDATE button_stats
                SET {button} = {value},
                    updated_at = ?
                WHERE id = (SELECT MAX(id) FROM button_stats)
            ''', (datetime.now(),))

    conn.commit()
