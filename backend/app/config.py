import os
from dotenv import load_dotenv

load_dotenv()

MQTT_BROKER = os.getenv("MQTT_BROKER", "broker.hivemq.com")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))
MQTT_TOPIC = os.getenv("MQTT_TOPIC", "tfm/ambiental/+/telemetria")
MQTT_CLIENT_ID = os.getenv("MQTT_CLIENT_ID", "tfm-backend-ingestor")

INFLUXDB_URL = os.getenv("INFLUXDB_URL", "http://localhost:8086")
INFLUXDB_TOKEN = os.getenv("INFLUXDB_TOKEN", "")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "tfm-org")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "ambiental")

LOG_LEVEL = os.getenv("LOG_LEVEL", "INFO").upper()