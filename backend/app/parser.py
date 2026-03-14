import json
from typing import Any, Dict


REQUIRED_FIELDS = {
    "node_id",
    "zone_id",
    "timestamp",
    "temperature_c",
    "humidity_pct",
    "gas_detected",
}


def parse_payload(payload: bytes) -> Dict[str, Any]:
    data = json.loads(payload.decode("utf-8"))

    missing = REQUIRED_FIELDS - data.keys()
    if missing:
        raise ValueError(f"Faltan campos obligatorios: {sorted(missing)}")

    return {
        "node_id": str(data["node_id"]),
        "zone_id": str(data["zone_id"]),
        "timestamp": int(data["timestamp"]),
        "temperature_c": float(data["temperature_c"]),
        "humidity_pct": float(data["humidity_pct"]),
        "gas_detected": int(data["gas_detected"]),
    }