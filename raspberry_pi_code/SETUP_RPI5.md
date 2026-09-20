# Raspberry Pi 5 推理脚本

在 **Raspberry Pi 5（1GB, 无桌面系统）** 上运行实时摄像头推理，通过 USB 发送结果到 ESP32。

## 环境

- **OS**: Raspberry Pi OS Lite (64-bit)
- **Python**: 3.11+
- **摄像头**: 任何支持 V4L2 的 USB 或 CSI 摄像头
- **推理框架**: NCNN (轻量级，适合树莓派)

## 安装步骤

### 1. 系统更新

```bash
sudo apt update && sudo apt upgrade
sudo apt install -y python3-pip python3-dev libatlas-base-dev libjasper-dev libtiff5 libjasper1 libharfbuzz0b libwebp6 libtiff6 libopenjp2-7 libopenjp2-tools python3-opencv
```

### 2. 安装 Python 依赖

```bash
cd ~/lvgl_temp_gui/raspberry_pi_code
pip3 install -r requirements-rpi5.txt
```

### 3. 安装 NCNN

```bash
# 方案 A: 使用预编译轮子（如果可用）
pip3 install ncnn

# 方案 B: 从源码编译（需要约 30-60 分钟）
git clone https://github.com/Tencent/ncnn.git
cd ncnn
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DNCNN_PYTHON=ON ..
make -j4
sudo make install
cd ../python && python3 setup.py install
```

### 4. 验证摄像头

```bash
# 列出可用摄像头
ls -la /dev/video*

# 测试摄像头（使用 cv2）
python3 -c "import cv2; cap = cv2.VideoCapture(0); print(f'Camera ready: {cap.get(cv2.CAP_PROP_FRAME_WIDTH)}x{cap.get(cv2.CAP_PROP_FRAME_HEIGHT)}')"
```

### 5. 连接 ESP32

在 `/dev/ttyACM0` 上使用 USB-CDC 连接 ESP32-S3（自动枚举）。

## 使用方法

### 基础运行（NCNN 模型）

```bash
python3 infer_and_send.py \
  --model best_ncnn_model/model.ncnn \
  --camera /dev/video0 \
  --port /dev/ttyACM0 \
  --baud 115200 \
  --interval 1.0 \
  --conf 0.25
```

### 参数说明

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `--model` | `best_ncnn_model/model.ncnn` | NCNN 模型路径（不含扩展名） |
| `--camera` | `/dev/cam0` | 摄像头路径或索引 |
| `--width` | `640` | 摄像头宽度 |
| `--height` | `480` | 摄像头高度 |
| `--port` | `/dev/ttyACM0` | USB 串口 |
| `--baud` | `115200` | 波特率 |
| `--interval` | `1.0` | 推理间隔（秒） |
| `--conf` | `0.25` | 置信度阈值 |

### 示例

发送番茄叶片诊断：
```bash
python3 infer_and_send.py \
  --model crops/yolov8m_leaf_disease.ncnn \
  --camera /dev/video0 \
  --interval 0.5
```

## 输出格式

每次推理后通过 USB 发送 JSON：

```json
{
  "type": "infer",
  "label": "番茄早疫病叶",
  "conf": 0.87,
  "ts": 1715769600
}
```

## 性能优化

### 1. 降低推理频率（节省 CPU）

```bash
python3 infer_and_send.py --interval 2.0  # 2 秒/次
```

### 2. 使用更小的模型

```bash
python3 infer_and_send.py --model crops/yolov8n.ncnn  # Nano 模型
```

### 3. 降低摄像头分辨率

```bash
python3 infer_and_send.py --width 480 --height 360
```

### 4. 运行时监控 CPU/内存

```bash
watch -n 1 'ps aux | grep infer_and_send.py'
top -p $(pgrep -f infer_and_send.py)
```

## 故障排除

### 摄像头无法打开

```bash
# 检查摄像头权限
ls -la /dev/video*
sudo usermod -a -G video $USER
# 重新登录或 sudo python3 infer_and_send.py

# 使用 v4l2-ctl 诊断
sudo apt install v4l2-utils
v4l2-ctl -l | grep camera
```

### USB 串口无法找到

```bash
# 查看已连接的设备
ls -la /dev/ttyACM*

# 检查权限
sudo usermod -a -G dialout $USER
```

### NCNN 导入失败

```bash
# 确认 NCNN Python 绑定已安装
python3 -c "import ncnn; print(ncnn.__version__)"

# 重新安装
pip3 install --upgrade ncnn
```

## 开机自启

创建 systemd 服务：

```bash
sudo nano /etc/systemd/system/rpi-infer.service
```

```ini
[Unit]
Description=Raspberry Pi Camera Inference
After=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/lvgl_temp_gui/raspberry_pi_code
ExecStart=/usr/bin/python3 /home/pi/lvgl_temp_gui/raspberry_pi_code/infer_and_send.py
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable rpi-infer.service
sudo systemctl start rpi-infer.service

# 查看日志
sudo journalctl -u rpi-infer.service -f
```

## 参考

- [NCNN 文档](https://github.com/Tencent/ncnn)
- [OpenCV Python](https://opencv-python-tutroals.readthedocs.io/)
- [Raspberry Pi 摄像头](https://www.raspberrypi.com/documentation/accessories/camera.html)
