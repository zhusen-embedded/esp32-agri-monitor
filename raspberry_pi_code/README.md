# Raspberry Pi USB CDC Sender

This folder contains the Raspberry Pi side script for sending inference results to the ESP32 over USB Serial/JTAG.

## Files

- `infer_and_send.py` - **Main**: Raspberry Pi 5 camera inference with NCNN + USB CDC (supports picamera2 & OpenCV)
- `usb_cdc_sender.py` - Simple USB CDC sender for testing
- `diagnose_camera.py` - 📷 Camera diagnostic tool
- `fix_camera.sh` - 🔧 Automatic camera setup script
- `CAMERA_TROUBLESHOOT.md` - 🆘 Complete camera troubleshooting guide
- `requirements-rpi5.txt` - Python dependencies for RPi 5
- `requirements.txt` - Minimal dependencies for basic sender

## 🚀 Quick Start

### If camera is NOT detected:

```bash
# 1. Run diagnostic
python3 diagnose_camera.py

# 2. Run auto-fix
chmod +x fix_camera.sh
./fix_camera.sh

# 3. See detailed guide
cat CAMERA_TROUBLESHOOT.md
```

### If camera IS detected:

```bash
# Install dependencies
pip3 install -r requirements-rpi5.txt

# Run inference
python3 infer_and_send.py \
  --model /path/to/model.ncnn \
  --port /dev/ttyACM0 \
  --interval 1.0
```

## Protocol

Send one JSON object per line, UTF-8 encoded, terminated by `\n`.

Example:

```json
{"type":"infer","label":"番茄早疫病叶","conf":0.87,"ts":1715769600}
```

The ESP32 receiver stores the latest valid JSON object (parsed by bracket depth) and displays it on the "AI建议" tab.

## Test without Raspberry Pi

Use `usb_cdc_sender.py` to send test messages:

```bash
# Install
pip3 install -r requirements.txt

# Send once
python3 usb_cdc_sender.py --port /dev/ttyACM0 --label tomato --conf 0.93 --once

# Send repeatedly
python3 usb_cdc_sender.py --port /dev/ttyACM0 --label tomato --conf 0.93 --interval 1.0
```

On Windows, replace the port with something like `COM5`.
