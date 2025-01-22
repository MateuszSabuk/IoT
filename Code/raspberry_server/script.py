
import time
import mysql.connector
from mysql.connector import Error, MySQLConnection
import paho.mqtt.client as mqtt
import json
from datetime import datetime

# Database configuration
db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'password',
    'database': 'IOT'
}

connection: MySQLConnection  = None

# MQTT configuration
mqtt_broker = "192.168.1.50"
mqtt_port = 1883
mqtt_topics = ["sensor/plant/coffea/+"]

def insert_sensor_data(table_name, sensor_value, timestamp):
    try:
        if connection.is_connected():
            cursor = connection.cursor()
            query = f"""
            INSERT INTO {table_name} (SensorValue, Timestamp)
            VALUES (%s, %s)
            """
            cursor.execute(query, (sensor_value, timestamp))
            connection.commit()
    except Error as e:
        print(f"Error: {e}")
    finally:
        if connection.is_connected():
            cursor.close()

def on_message(client, userdata, msg):
    try:
        # Decode message payload (assuming it's JSON format)
        message_payload = json.loads(msg.payload.decode())
        sensor_value = message_payload
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        # Determine the appropriate table based on the topic
        if msg.topic == "sensor/plant/coffea/humidity":
            insert_sensor_data("Humidity", sensor_value, timestamp)
        elif msg.topic == "sensor/plant/coffea/luminosity":
            insert_sensor_data("Luminosity", sensor_value, timestamp)
        elif msg.topic == "sensor/plant/coffea/pressure":
            insert_sensor_data("Pressure", sensor_value, timestamp)
        elif msg.topic == "sensor/plant/coffea/temperature":
            insert_sensor_data("Temperature", sensor_value, timestamp)
        else:
            print("Unknown topic")
    except Exception as e:
        print(f"Error processing message: {e}")

def on_connect(client, userdata, flags, rc):
    print("Connected to broker")
    for topic in mqtt_topics:
        client.subscribe(topic)
        print(f"Subscribed to {topic}")

def mqtt_client():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()

def connect_to_server():
    global connection
    try:
        connection = mysql.connector.connect(**db_config)
    except:
        pass
    while connection == None or not connection.is_connected():
        time.sleep(5)
        try:
            connection = mysql.connector.connect(**db_config)
        except:
            pass
    print("Connected to mysql server")

if __name__ == "__main__":
    time.sleep(30)
    connect_to_server()
    # Start MQTT client
    mqtt_client()

    while True:
        time.sleep(10)

    connection.close()
