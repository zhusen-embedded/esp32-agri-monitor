#!/usr/bin/env python3
"""
诊断树莓派摄像头问题的脚本
"""

import os
import subprocess
import sys

def run_cmd(cmd):
    """运行命令并返回输出"""
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=5)
        return result.stdout + result.stderr
    except Exception as e:
        return f"Error: {e}"

print("=" * 60)
print("🔍 Raspberry Pi 摄像头诊断")
print("=" * 60)

# 1. 检查设备文件
print("\n1️⃣  检查 V4L2 设备:")
result = run_cmd("ls -la /dev/video* 2>/dev/null || echo '❌ 没有找到 /dev/video*'")
print(result if result.strip() else "❌ 没有找到 /dev/video*")

# 2. 检查 libcamera（树莓派 5 推荐）
print("\n2️⃣  检查 libcamera 摄像头:")
result = run_cmd("libcamera-hello --list-cameras 2>&1")
print(result if result.strip() else "❌ libcamera-hello 不可用或无摄像头")

# 3. 检查 /boot/firmware/config.txt 配置
print("\n3️⃣  检查 /boot/firmware/config.txt 摄像头配置:")
if os.path.exists("/boot/firmware/config.txt"):
    result = run_cmd("grep -E 'camera|dtoverlay' /boot/firmware/config.txt || echo '❌ 未配置摄像头'")
    print(result if result.strip() else "❌ 未配置摄像头")
elif os.path.exists("/boot/config.txt"):
    result = run_cmd("grep -E 'camera|dtoverlay' /boot/config.txt || echo '❌ 未配置摄像头'")
    print(result if result.strip() else "❌ 未配置摄像头")
else:
    print("❌ /boot 配置文件不存在")

# 4. 检查摄像头权限
print("\n4️⃣  检查摄像头权限:")
result = run_cmd("id | grep -q video && echo '✅ 用户在 video 组' || echo '❌ 用户不在 video 组'")
print(result.strip())

# 5. 尝试 picamera2
print("\n5️⃣  检查 picamera2:")
try:
    from picamera2 import Picamera2
    print("✅ picamera2 已安装")
    try:
        cam = Picamera2()
        print(f"✅ picamera2 可以打开摄像头")
        print(f"   摄像头型号: {cam.camera_properties}")
        cam.close()
    except Exception as e:
        print(f"❌ picamera2 无法打开摄像头: {e}")
except ImportError:
    print("❌ picamera2 未安装")
    print("   安装: sudo apt install -y python3-picamera2")

# 6. 尝试 OpenCV
print("\n6️⃣  检查 OpenCV:")
try:
    import cv2
    print("✅ OpenCV 已安装")
    cap = cv2.VideoCapture(0)
    if cap.isOpened():
        print("✅ OpenCV 可以打开 /dev/video0")
        cap.release()
    else:
        print("❌ OpenCV 无法打开 /dev/video0")
except Exception as e:
    print(f"❌ OpenCV 错误: {e}")

# 7. 检查 GPU 内存分配
print("\n7️⃣  检查 GPU 内存分配:")
if os.path.exists("/boot/firmware/config.txt"):
    result = run_cmd("grep gpu_mem /boot/firmware/config.txt || echo 'gpu_mem 未配置'")
    print(result.strip())
elif os.path.exists("/boot/config.txt"):
    result = run_cmd("grep gpu_mem /boot/config.txt || echo 'gpu_mem 未配置'")
    print(result.strip())

# 8. 建议
print("\n" + "=" * 60)
print("💡 建议:")
print("=" * 60)
print("""
如果摄像头没有出现在 /dev/video* 中：

1️⃣  确保摄像头已连接：
   - FPC 连接器插入相机槽位（CSI）
   - 拉出连接器的黑色卡扣，插入 FPC，推回卡扣
   
2️⃣  启用摄像头（如果是 Raspberry Pi 5）：
   sudo raspi-config
   → Interface Options → Camera → Enable
   → 重启
   
3️⃣  如果使用 picamera2（推荐）：
   sudo apt install -y python3-picamera2 libcamera-tools
   libcamera-hello  # 测试摄像头
   
4️⃣  给用户添加 video 组权限：
   sudo usermod -a -G video $USER
   # 重新登录或运行: newgrp video

5️⃣  如果仍无法工作，尝试调整 /boot/firmware/config.txt：
   [all]
   dtoverlay=vc4-kms-v3d,cma-256
   camera_auto_detect=1
   display_auto_detect=1
""")
