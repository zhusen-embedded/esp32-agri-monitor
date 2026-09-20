# 智能农业监测与病害识别终端

基于 **ESP32-S3** 与 **树莓派 5** 的农业环境监测 + 边缘 AI 病害识别系统。

ESP32 端负责环境数据采集、本地触摸交互与执行器控制；树莓派端负责摄像头图像推理，结果通过 USB CDC 回传 ESP32 显示，构成「**感知 → 识别 → 决策 → 执行**」的闭环。

---

## 系统架构

```
┌─────────────────────────────┐         ┌──────────────────────────────┐
│        树莓派 5 (RPi5)       │         │         ESP32-S3             │
│                             │         │                              │
│  picamera2 / OpenCV         │         │  ┌────────────────────────┐  │
│        ↓                    │  USB    │  │  LVGL9 UI (多级页面栈)  │  │
│  640x480 @15fps             │  CDC    │  │  ILI9341 + FT6x36      │  │
│        ↓                    │ ──────► │  └────────────────────────┘  │
│  NCNN 推理 (320x320)         │  JSON   │  ┌────────────────────────┐  │
│        ↓                    │  一行    │  │ BH1750 光照 (I2C)      │  │
│  病害类别 + 置信度            │  一帧    │  │ RS485 土壤多参数 (UART1)│  │
│                             │         │  └────────────────────────┘  │
└─────────────────────────────┘         │  ┌────────────────────────┐  │
                                        │  │ 继电器: 水泵 / 补光灯   │  │
                    ┌───────────────┐   │  └────────────────────────┘  │
                    │  MQTT Broker  │◄──┤  ┌────────────────────────┐  │
                    │  (TLS)        │   │  │ BLE 配网 + Wi-Fi 重连   │  │
                    └───────────────┘   │  └────────────────────────┘  │
                                        └──────────────────────────────┘
```

---

## 硬件组成

| 模块 | 型号 | 接口 | 引脚 / 地址 |
|---|---|---|---|
| 主控 | ESP32-S3 | — | — |
| 显示屏 | ILI9341 | SPI | SCLK=GPIO11, MOSI=GPIO12, CS=GPIO10, DC=GPIO13, RST=GPIO9, BL=GPIO5 |
| 触摸 | FT6x36 | I2C | SCL=GPIO4, SDA=GPIO5, 地址 `0x38` |
| 光照 | BH1750 | I2C | SCL=GPIO4, SDA=GPIO5, 地址 `0x23` |
| 土壤多参数 | 兆泰盛五插针（RS485） | UART1 | TX=GPIO17, RX=GPIO18 |
| 水泵继电器 | — | GPIO | GPIO15 |
| 补光灯继电器 | — | GPIO | GPIO16 |
| 调试串口 | — | UART0 | TX0 / RX0 |
| 边缘推理 | 树莓派 5 + 摄像头 | USB CDC | `/dev/ttyACM0` |

> 引脚分配与 I2C 地址详见 [`docs/hardware_modules_table.csv`](docs/hardware_modules_table.csv)
> 土壤传感器手册：[`兆泰盛五插针土壤多参数传感器.pdf`](兆泰盛五插针土壤多参数传感器.pdf)

---

## 功能

- **环境监测**：土壤多参数（RS485 工业传感器）+ 光照强度（BH1750）
- **本地交互**：LVGL9 触摸界面，多级页面栈支持返回上一级
- **网络连接**：BLE 配网（首次配置）+ Wi-Fi 断线自动重连 + MQTT over TLS 上报
- **边缘 AI 识别**：树莓派摄像头 + NCNN 推理，识别 40+ 类作物病害与健康叶
- **执行器控制**：继电器驱动水泵与补光灯，支持手动开关
- **OTA 预留**：分区表已配置 `otadata` 槽位

---

## 关键技术点

### 1. USB CDC 通信协议（树莓派 → ESP32）

- 传输格式：**一行一个 JSON 对象**，UTF-8 编码，`\n` 结尾
- ESP32 接收端按**括号深度**解析，只保留最近一个合法 JSON
- 树莓派侧带**断线重试**（重试间隔 3s），串口拔出后可自动恢复

```json
{"type":"infer","label":"番茄早疫病叶","conf":0.87,"ts":1715769600}
```

### 2. RS485 土壤多参数传感器

工业级 RS485 传感器通过 UART1 读取，包含查询帧构造与响应帧校验。

### 3. LVGL 触摸驱动移植

自写 `esp_lcd_touch_ft6x36` 组件对接 ESP-IDF 的 LCD 触摸接口，配合 LVGL9 使用。

### 4. 树莓派推理流水线

- 摄像头采集：`picamera2` / OpenCV 双通道，640×480 @15fps
- 推理：NCNN，输入 320×320，置信度阈值 0.25，推理间隔 1.0s
- 模型：`raspberry_pi_code/best_ncnn_model/`。原始为 YOLO 模型，因**树莓派板端内存受限**，转换为 NCNN 格式后部署
- 识别类别覆盖玉米、水稻、辣椒、甘蓝、草莓、马铃薯、番茄、小麦等作物的病害与健康叶

### 5. 页面栈式导航

用栈保存页面指针实现「返回上一级」，避免 LVGL 页面动态变化导致的指针失效问题。

---

## 目录结构

```
lvgl_temp_gui/
├── main/
│   ├── main.c                    # 主程序入口、任务初始化
│   ├── ble_sitting_wifi/         # BLE 配网模块
│   ├── uart_echo_wifi_ble/       # 串口 / Wi-Fi / BLE 通信
│   ├── esp_lcd_touch_ft6x36/     # FT6x36 触摸驱动组件（自写）
│   └── ui/                       # LVGL 界面（generated + custom + events）
├── raspberry_pi_code/            # 树莓派端代码
│   ├── infer_and_send.py         # 主脚本：摄像头推理 + USB CDC 发送
│   ├── usb_cdc_sender.py         # 测试用发送脚本
│   ├── diagnose_camera.py        # 摄像头诊断工具
│   ├── fix_camera.sh             # 摄像头自动修复脚本
│   ├── CAMERA_TROUBLESHOOT.md    # 摄像头排障手册
│   ├── SETUP_RPI5.md             # 树莓派 5 环境配置
│   └── best_ncnn_model/          # NCNN 模型文件
├── docs/
│   └── hardware_modules_table.csv  # 硬件模块 / 引脚 / I2C 地址表
├── partitions.csv                # 分区表（已预留 OTA 槽位）
└── sdkconfig
```

---

## 编译与烧录（ESP32 端）

```bash
# 需要 ESP-IDF v5.x
idf.py set-target esp32s3
idf.py menuconfig        # 配置 Wi-Fi / MQTT 参数
idf.py build
idf.py -p <PORT> flash monitor
```

## 运行（树莓派端）

```bash
# 1. 摄像头未识别时先诊断
python3 diagnose_camera.py
chmod +x fix_camera.sh && ./fix_camera.sh

# 2. 安装依赖
pip3 install -r requirements-rpi5.txt

# 3. 运行推理并回传
python3 infer_and_send.py \
  --model ./best_ncnn_model \
  --port /dev/ttyACM0 \
  --interval 1.0
```

无树莓派时可用 `usb_cdc_sender.py` 发送测试数据（Windows 下端口写成 `COM5` 之类）。

---

## 踩坑记录

### 摄像头识别失败

树莓派 5 的摄像头链路与旧版差异较大，遇到过设备不识别、驱动加载异常等情况。
完整的排查步骤、诊断脚本与自动修复方案见 [`raspberry_pi_code/CAMERA_TROUBLESHOOT.md`](raspberry_pi_code/CAMERA_TROUBLESHOOT.md)。

### 其他

- 详见 git 提交历史，每次提交都记录了当时的功能状态与遗留问题

---

## 开发历程

项目从 2025 年 9 月启动，历经多轮重构（早期版本存在架构问题，已废弃重写），最终版本持续迭代至 2026 年 6 月。
完整演进过程见 `git log`。

---

## TODO

- [ ] 补充硬件原理图（`docs/hardware_design.md` 待完善）
- [ ] 补充运行截图（LVGL 界面 / 串口数据 / 推理输出）
- [ ] 实现 OTA 远程升级（分区表已预留）
- [ ] 补充自动控制策略（当前为手动控制水泵与补光灯）
