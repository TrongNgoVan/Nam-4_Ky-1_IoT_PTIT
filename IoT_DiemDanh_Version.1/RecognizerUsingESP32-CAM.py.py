

#  Ver 2 nhận diện qua ESP32 -CAM và gửi thông báo về máy tính.

import cv2
import urllib.request
import numpy as np
import sqlite3

# Địa chỉ IP của ESP32-CAM (thay bằng IP của bạn)
url = 'http://192.168.43.181/cam-mid.jpg'

# Khởi tạo bộ phát hiện khuôn mặt
faceDetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Khởi tạo bộ nhận diện khuôn mặt
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('recognizer/trainner.yml')

fontface = cv2.FONT_HERSHEY_SIMPLEX
fontscale = 1
fontcolor = (0, 255, 0)
fontcolor1 = (0, 0, 255)

# Hàm lấy thông tin người dùng qua ID
def getProfile(id):
    conn = sqlite3.connect("FaceBaseNew.db")
    cursor = conn.execute("SELECT * FROM People WHERE ID=" + str(id))
    profile = None
    for row in cursor:
        profile = row
    conn.close()
    return profile

while True:
    try:
        # Đọc ảnh từ ESP32-CAM
        img_resp = urllib.request.urlopen(url)
        img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        img = cv2.imdecode(img_np, -1)

        if img is None:
            continue

        # Lật ảnh cho đỡ bị ngược (nếu cần, tùy hướng camera)
        img = cv2.flip(img, 1)

        # Chuyển ảnh về xám
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Phát hiện các khuôn mặt trong ảnh
        faces = faceDetect.detectMultiScale(gray, 1.3, 5)

        # Lặp qua các khuôn mặt nhận được để nhận diện
        for (x, y, w, h) in faces:
            # Vẽ hình chữ nhật quanh khuôn mặt
            cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

            # Nhận diện khuôn mặt
            id, dist = recognizer.predict(gray[y:y + h, x:x + w])
#  ESP 32- CAM chất lượng thấp nên có thể tăng độ sai số khi phát hiện ra

            if dist <= 50:
                profile = getProfile(id)
                if profile is not None:
                    message = f"Sinh viên: {profile[1]} đã điểm danh"
                    print(message)

                    # Hiển thị thông báo trên màn hình
                    cv2.putText(img, message, (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

                    # Dừng hiển thị sau 2 giây
                    cv2.imshow('Face', img)
                    cv2.waitKey(2000)  # Chờ 2 giây để hiển thị thông báo
                    break

        # Hiển thị hình ảnh từ ESP32-CAM
        cv2.imshow("ESP32-CAM", img)

        # Thoát nếu nhấn phím 'q'
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

    except Exception as e:
        print("Error: ", str(e))

cv2.destroyAllWindows()

