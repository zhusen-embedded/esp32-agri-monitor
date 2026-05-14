# Raspberry Pi USB CDC Sender

This folder contains the Raspberry Pi side script for sending inference results to the ESP32 over USB Serial/JTAG.

## Protocol

Send one JSON object per line, UTF-8 encoded, terminated by `\n`.

Example:

{"type":"infer","label":"tomato","conf":0.93}

The ESP32 receiver currently stores the latest non-empty line and logs it.

## Files

- `usb_cdc_sender.py` - simple sender script using `pyserial`
- `requirements.txt` - Python dependency list

## Install

```bash
pip3 install -r requirements.txt
```

## Send one result

```bash
python3 usb_cdc_sender.py --port /dev/ttyACM0 --label tomato --conf 0.93 --once
```

## Send repeatedly

```bash
python3 usb_cdc_sender.py --port /dev/ttyACM0 --label tomato --conf 0.93 --interval 1.0
```

On Windows, replace the port with something like `COM5`.
