from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

from config import INFLUXDB_URL, INFLUXDB_TOKEN, INFLUXDB_ORG, INFLUXDB_BUCKET


class InfluxWriter:
    def __init__(self) -> None:
        self.client = InfluxDBClient(
            url=INFLUXDB_URL,
            token=INFLUXDB_TOKEN,
            org=INFLUXDB_ORG,
        )
        self.write_api = self.client.write_api(write_options=SYNCHRONOUS)

    def write_telemetry(self, data: dict) -> None:
        point = (
            Point("environment")
            .tag("node_id", data["node_id"])
            .tag("zone_id", data["zone_id"])
            .field("temperature_c", data["temperature_c"])
            .field("humidity_pct", data["humidity_pct"])
            .field("gas_detected", data["gas_detected"])
            .time(data["timestamp"], WritePrecision.S)
        )

        self.write_api.write(
            bucket=INFLUXDB_BUCKET,
            org=INFLUXDB_ORG,
            record=point,
        )

    def close(self) -> None:
        self.client.close()