#!/usr/bin/env python3
"""Send Raspberry Pi inference results to ESP32 over USB Serial/JTAG.

Protocol:
- One JSON object per line
- UTF-8 encoded
- Line terminated with '\n'

Example payload:
{"type":"infer","label":"tomato","conf":0.93}
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from typing import Any, Dict

import serial


def build_message(label: str, confidence: float, extra: Dict[str, Any] | None = None) -> str:
    message: Dict[str, Any] = {
        "type": "infer",
        "label": label,
        "conf": round(float(confidence), 4),
        "ts": int(time.time()),
    }
    if extra:
        message.update(extra)
    return json.dumps(message, ensure_ascii=False, separators=(",", ":"))


def main() -> int:
    parser = argparse.ArgumentParser(description="Send inference results to ESP32 over USB CDC")
    parser.add_argument("--port", required=True, help="Serial port, for example /dev/ttyACM0 or COM5")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate for the serial port")
    parser.add_argument("--label", default="unknown", help="Inference label")
    parser.add_argument("--conf", type=float, default=0.0, help="Confidence value")
    parser.add_argument("--once", action="store_true", help="Send one message and exit")
    parser.add_argument("--interval", type=float, default=1.0, help="Send interval in seconds when not using --once")
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
    except serial.SerialException as exc:
        print(f"Failed to open serial port: {exc}", file=sys.stderr)
        return 1

    try:
        while True:
            payload = build_message(args.label, args.conf)
            ser.write((payload + "\n").encode("utf-8"))
            ser.flush()
            print(payload)
            if args.once:
                break
            time.sleep(args.interval)
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
