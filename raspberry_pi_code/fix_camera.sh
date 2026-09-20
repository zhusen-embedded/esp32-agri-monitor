#!/bin/bash
# Raspberry Pi 5 摄像头快速修复脚本

echo "🔧 Raspberry Pi 5 摄像头问题诊断和修复"
echo "=========================================="

# 1. 检查是否以 root 运行某些操作
if [ "$EUID" -ne 0 ]; then
    echo "⚠️  某些操作需要 sudo 权限"
fi

# 2. 安装必要的库
echo -e "\n📦 安装摄像头库..."
sudo apt update
sudo apt install -y python3-picamera2 libcamera-tools v4l2-utils

# 3. 添加用户到 video 组
echo -e "\n👤 配置用户权限..."
sudo usermod -a -G video $USER
echo "✅ 用户已添加到 video 组（需要重新登录生效）"

# 4. 测试 picamera2
echo -e "\n🎥 测试 picamera2..."
python3 -c "from picamera2 import Picamera2; print('✅ picamera2 可以导入')" 2>/dev/null || echo "❌ picamera2 导入失败"

# 5. 列出摄像头
echo -e "\n📷 列出可用摄像头:"
libcamera-hello --list-cameras

echo -e "\n💡 下一步:"
echo "1. 重新登录或运行: newgrp video"
echo "2. 测试推理脚本: python3 infer_and_send.py --model model.ncnn --no-usb --verbose"
