import logging
import signal
import sys

import paho.mqtt.client as mqtt

from config import (
    LOG_LEVEL,
    MQTT_BROKER,
    MQTT_CLIENT_ID,
    MQTT_PORT,
    MQTT_TOPIC,
)
from parser import parse_payload
from writer import InfluxWriter

logging.basicConfig(
    level=getattr(logging, LOG_LEVEL, logging.INFO),
    format="%(asctime)s [%(levelname)s] %(message)s",
)

logger = logging.getLogger(__name__)
writer = InfluxWriter()


def on_connect(client, userdata, flags, reason_code, properties=None):
    logger.info("Conectado a MQTT con código: %s", reason_code)
    client.subscribe(MQTT_TOPIC)
    logger.info("Suscrito a topic: %s", MQTT_TOPIC)


def on_message(client, userdata, msg):
    try:
        data = parse_payload(msg.payload)
        writer.write_telemetry(data)
        logger.info(
            "Dato escrito en InfluxDB | node_id=%s zone_id=%s temp=%.2f hum=%.2f gas=%d",
            data["node_id"],
            data["zone_id"],
            data["temperature_c"],
            data["humidity_pct"],
            data["gas_detected"],
        )
    except Exception as exc:
        logger.exception("Error procesando mensaje en topic %s: %s", msg.topic, exc)


def shutdown(client):
    logger.info("Cerrando servicio...")
    try:
        client.loop_stop()
        client.disconnect()
    finally:
        writer.close()


def handle_signal(signum, frame):
    raise KeyboardInterrupt


def main():
    signal.signal(signal.SIGINT, handle_signal)
    signal.signal(signal.SIGTERM, handle_signal)

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id=MQTT_CLIENT_ID)
    client.on_connect = on_connect
    client.on_message = on_message

    logger.info("Conectando a broker MQTT %s:%s", MQTT_BROKER, MQTT_PORT)
    client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    client.loop_start()

    try:
        signal.pause()
    except KeyboardInterrupt:
        shutdown(client)
        sys.exit(0)


if __name__ == "__main__":
    main()