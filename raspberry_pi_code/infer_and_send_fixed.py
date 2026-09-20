import os, time, subprocess, cv2, numpy as np, json, signal, sys, select

try:
    import ncnn
except:
    print("❌ NCNN 未安装")
    sys.exit(1)

try:
    import serial
except:
    serial = None

MODEL_PARAM = "model.ncnn.param"
MODEL_BIN = "model.ncnn.bin"
IMG_SIZE = 320
FIFO_PATH = "/tmp/cam.mjpeg"
WIDTH, HEIGHT, FPS = 640, 480, 15
INFER_INTERVAL = 1.0
CONF_THRES = 0.25
USB_PORT = os.getenv("USB_PORT", "/dev/ttyACM0")
USB_BAUD = int(os.getenv("USB_BAUD", "115200"))
USB_RETRY_INTERVAL = 3.0

CLASS_NAMES = ["玉米灰斑病", "玉米叶枯病", "玉米锈病", "水稻白叶枯病", "水稻褐斑病", "水稻健康叶",
    "水稻稻瘟病", "水稻纹枯病", "辣椒炭疽病", "辣椒健康", "辣椒卷叶病", "辣椒叶斑病",
    "辣椒白粉虱", "辣椒黄化卷叶病毒", "甘蓝链格孢叶斑病", "甘蓝黑腐病", "甘蓝霜霉病",
    "甘蓝枯萎病", "甘蓝健康", "甘蓝白粉病", "甘蓝菌核病", "草莓不可食果",
    "马铃薯黑痣病", "马铃薯黑胫病", "马铃薯普通疮痂病", "马铃薯干腐病", "马铃薯早疫病叶",
    "马铃薯健康", "马铃薯健康叶", "马铃薯晚疫病叶", "草莓叶斑病",
    "番茄炭疽病果", "番茄细菌性斑点病叶", "番茄脐腐病", "番茄早疫病叶", "番茄健康果",
    "番茄健康叶", "番茄晚疫病叶", "番茄叶霉病", "番茄花叶病毒叶", "番茄斑点病叶",
    "番茄病毒斑", "番茄二斑叶螨叶", "番茄黄化卷叶病毒", "小麦白粉病", "小麦叶枯病",
    "小麦茎锈病", "小麦黄矮病"]

CLASS_NAMES_EN = [
    "corn_gray_leaf_spot", "corn_leaf_blight", "corn_rust", "rice_bacterial_leaf_blight", "rice_brown_spot", "rice_healthy_leaf",
    "rice_blast", "rice_sheath_blight", "pepper_anthracnose", "pepper_healthy", "pepper_leaf_curl", "pepper_leaf_spot",
    "pepper_whitefly", "pepper_yellow_leaf_curl_virus", "cabbage_alternaria_leaf_spot", "cabbage_black_rot", "cabbage_downy_mildew",
    "cabbage_wilt", "cabbage_healthy", "cabbage_powdery_mildew", "cabbage_sclerotinia", "strawberry_unmarketable_fruit",
    "potato_black_scurf", "potato_blackleg", "potato_common_scab", "potato_dry_rot", "potato_early_blight_leaf",
    "potato_healthy", "potato_healthy_leaf", "potato_late_blight_leaf", "strawberry_leaf_spot",
    "tomato_fruit_anthracnose", "tomato_bacterial_spot_leaf", "tomato_blossom_end_rot", "tomato_early_blight_leaf", "tomato_healthy_fruit",
    "tomato_healthy_leaf", "tomato_late_blight_leaf", "tomato_leaf_mold", "tomato_mosaic_virus_leaf", "tomato_septoria_leaf_spot",
    "tomato_virus_spot", "tomato_two_spotted_spider_mite_leaf", "tomato_yellow_leaf_curl_virus", "wheat_powdery_mildew", "wheat_leaf_blight",
    "wheat_stem_rust", "wheat_yellow_dwarf"
]

CLASS_EN_MAP = dict(zip(CLASS_NAMES, CLASS_NAMES_EN))

net = None
rpicam_proc = None
ser = None
frame_count = 0
last_serial_retry = 0.0


def try_open_serial(force=False):
    global ser, last_serial_retry
    if serial is None:
        return False
    if ser is not None:
        return True

    now = time.time()
    if (not force) and (now - last_serial_retry < USB_RETRY_INTERVAL):
        return False
    last_serial_retry = now

    try:
        ser = serial.Serial(USB_PORT, USB_BAUD, timeout=1)
        print(f"✅ USB 已连接: {USB_PORT} @ {USB_BAUD}")
        return True
    except Exception as e:
        print(f"⚠️  USB 未就绪: {e}")
        ser = None
        return False


def close_serial():
    global ser
    if ser is not None:
        try:
            ser.close()
        except Exception:
            pass
        ser = None

def send_result(label, confidence):
    if not try_open_serial():
        return
    label_en = CLASS_EN_MAP.get(label, label)
    msg = {"type": label_en, "label": label_en, "conf": round(float(confidence), 4), "ts": int(time.time())}
    try:
        line = json.dumps(msg, ensure_ascii=False, separators=(",", ":"))
        ser.write((line + "\n").encode("utf-8"))
        ser.flush()
        print(f"✅ USB 发送: {line}")
    except Exception as e:
        print(f"❌ USB 发送失败: {e}")
        close_serial()

def cleanup_resources():
    """强制清理所有资源"""
    global rpicam_proc, ser
    print("\n🧹 强制清理资源...")

    # 杀死 rpicam 进程
    subprocess.run(["pkill", "-9", "-f", "rpicam-vid"], stderr=subprocess.DEVNULL)

    # 删除 FIFO
    if os.path.exists(FIFO_PATH):
        try:
            os.remove(FIFO_PATH)
            print("✅ 已删除 FIFO 文件")
        except:
            pass

    # 关闭 USB
    if ser:
        close_serial()
        print("✅ USB 已关闭")

    print(f"✅ 完成 (总共处理 {frame_count} 帧)")

def start_rpicam():
    print("🎥 清理旧进程...")
    subprocess.run(["pkill", "-9", "-f", "rpicam-vid"], stderr=subprocess.DEVNULL)
    time.sleep(0.5)

    if os.path.exists(FIFO_PATH):
        try:
            os.remove(FIFO_PATH)
        except:
            pass

    print("🎥 启动 rpicam-vid...")
    try:
        os.mkfifo(FIFO_PATH)
    except FileExistsError:
        pass

    cmd = ["rpicam-vid", "--codec", "mjpeg", "--inline", "--nopreview", "--width", str(WIDTH),
           "--height", str(HEIGHT), "--framerate", str(FPS), "-t", "0", "-o", FIFO_PATH]

    try:
        rpicam_proc = subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        print(f"✅ rpicam-vid 已启动 (PID: {rpicam_proc.pid})")
        time.sleep(2)  # 增加等待时间
        return True
    except Exception as e:
        print(f"❌ 启动 rpicam-vid 失败: {e}")
        return False

def letterbox(im, new_shape=320, color=(114, 114, 114)):
    h, w = im.shape[:2]
    r = min(new_shape / h, new_shape / w)
    nh, nw = int(round(h * r)), int(round(w * r))
    im_resized = cv2.resize(im, (nw, nh), interpolation=cv2.INTER_LINEAR)
    canvas = np.full((new_shape, new_shape, 3), color, dtype=np.uint8)
    top = (new_shape - nh) // 2
    left = (new_shape - nw) // 2
    canvas[top:top+nh, left:left+nw] = im_resized
    return canvas, r, left, top

def nms(boxes, scores, iou_thres=0.45):
    idxs = scores.argsort()[::-1]
    keep = []
    while len(idxs) > 0:
        i = idxs[0]
        keep.append(i)
        if len(idxs) == 1: break
        xx1 = np.maximum(boxes[i, 0], boxes[idxs[1:], 0])
        yy1 = np.maximum(boxes[i, 1], boxes[idxs[1:], 1])
        xx2 = np.minimum(boxes[i, 2], boxes[idxs[1:], 2])
        yy2 = np.minimum(boxes[i, 3], boxes[idxs[1:], 3])
        w = np.maximum(0, xx2 - xx1)
        h = np.maximum(0, yy2 - yy1)
        inter = w * h
        area_i = (boxes[i, 2] - boxes[i, 0]) * (boxes[i, 3] - boxes[i, 1])
        area_j = (boxes[idxs[1:], 2] - boxes[idxs[1:], 0]) * (boxes[idxs[1:], 3] - boxes[idxs[1:], 1])
        iou = inter / (area_i + area_j - inter + 1e-6)
        idxs = idxs[1:][iou < iou_thres]
    return keep

def infer_frame(frame):
    try:
        img, r, dx, dy = letterbox(frame, IMG_SIZE)
        img = img.astype(np.float32) / 255.0
        img = img.transpose(2, 0, 1)
        ex = net.create_extractor()
        ex.input("in0", ncnn.Mat(img))
        _, out = ex.extract("out0")
        out = np.array(out)
        if out.shape[0] < 5: return "unknown", 0.0
        boxes = out[:4, :]
        class_scores = out[4:, :]
        class_id = np.argmax(class_scores, axis=0)
        scores = class_scores[class_id, np.arange(class_scores.shape[1])]
        keep = scores > 0.25
        if not np.any(keep): return "unknown", 0.0
        boxes = boxes[:, keep].T
        scores = scores[keep]
        class_id = class_id[keep]
        xyxy = np.zeros_like(boxes)
        xyxy[:, 0] = boxes[:, 0] - boxes[:, 2] / 2
        xyxy[:, 1] = boxes[:, 1] - boxes[:, 3] / 2
        xyxy[:, 2] = boxes[:, 0] + boxes[:, 2] / 2
        xyxy[:, 3] = boxes[:, 1] + boxes[:, 3] / 2
        keep_idx = nms(xyxy, scores, 0.45)
        if len(keep_idx) == 0: return "unknown", 0.0
        scores = scores[keep_idx]
        class_id = class_id[keep_idx]
        best = int(np.argmax(scores))
        best_score = float(scores[best])
        best_cls = int(class_id[best])
        disease = CLASS_NAMES[best_cls] if best_cls < len(CLASS_NAMES) else f"class_{best_cls}"
        return disease, best_score
    except Exception as e:
        print(f"❌ 推理异常: {e}")
        return "error", 0.0

def capture_loop():
    global frame_count
    print("📖 打开 FIFO 流...")
    print("   (如果卡住，说明 rpicam-vid 没有输出，按 Ctrl+C 重试)")

    # 使用非阻塞打开
    try:
        # 打开 FIFO 的读端，带超时
        cap = cv2.VideoCapture(FIFO_PATH, cv2.CAP_FFMPEG)

        # 设置读超时
        cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        # 尝试读取第一帧（有超时）
        for attempt in range(10):
            ret, frame = cap.read()
            if ret and frame is not None:
                print(f"✅ FIFO 流已打开\n▶️  推理中 (按 Ctrl+C 停止)...\n")
                break
            print(f"   等待数据 ({attempt+1}/10)...")
            time.sleep(0.5)
        else:
            print("❌ FIFO 流超时，rpicam-vid 可能没有输出")
            return
    except Exception as e:
        print(f"❌ 打开 FIFO 失败: {e}")
        return

    last_infer = 0
    error_count = 0
    try:
        while True:
            ret, frame = cap.read()
            if not ret or frame is None:
                error_count += 1
                if error_count % 100 == 0: print(f"⚠️  读取帧失败 ({error_count}x)")
                time.sleep(0.01)
                continue
            error_count = 0
            now = time.time()
            if now - last_infer < INFER_INTERVAL: continue
            last_infer = now
            disease, conf = infer_frame(frame)
            print(f"[{frame_count}] 推理: {disease} ({conf:.3f})")
            if disease == "error":
                pass
            elif disease == "unknown" and conf < CONF_THRES:
                send_result("none", 0)
            elif disease != "unknown":
                send_result(disease, conf)
            frame_count += 1
    except KeyboardInterrupt:
        print("\n\n⏹️  用户停止")
    finally:
        cap.release()

def main():
    global net, ser
    print("="*60)
    print("🚀 NCNN YOLOv8 推理 + USB CDC (修复版)")
    print("="*60 + "\n")
    print("📦 加载 NCNN 模型...")
    try:
        net = ncnn.Net()
        net.load_param(MODEL_PARAM)
        net.load_model(MODEL_BIN)
        print("✅ 模型已加载\n")
    except Exception as e:
        print(f"❌ 模型加载失败: {e}")
        return 1
    print("🔌 连接 USB...")
    if serial:
        try_open_serial(force=True)
        print("")

    signal.signal(signal.SIGINT, lambda s,f: (cleanup_resources(), sys.exit(0)))

    if not start_rpicam(): return 1

    try:
        capture_loop()
    except Exception as e:
        print(f"❌ 异常: {e}")
        return 1
    finally:
        cleanup_resources()
    return 0

if __name__ == "__main__":
    sys.exit(main())