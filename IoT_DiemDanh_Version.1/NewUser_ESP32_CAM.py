

import cv2
import urllib.request
import numpy as np
import sqlite3

# Địa chỉ IP của ESP32-CAM
url = 'http://192.168.43.181/cam-lo.jpg'

detector = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Hàm cập nhật tên và ID vào CSDL
def insertOrUpdate(id, name):
    conn = sqlite3.connect("FaceBaseNew.db")
    cursor = conn.execute('SELECT * FROM People WHERE ID=' + str(id))
    isRecordExist = 0
    for row in cursor:
        isRecordExist = 1
        break

    if isRecordExist == 1:
        cmd = "UPDATE People SET Name='" + str(name) + "' WHERE ID=" + str(id)
    else:
        cmd = "INSERT INTO People(ID, Name) VALUES(" + str(id) + ", '" + str(name) + "')"

    conn.execute(cmd)
    conn.commit()
    conn.close()

id = input('Nhập mã sinh viên:')
name = input('Nhập tên sinh viên:')
print("Bắt đầu chụp ảnh sinh viên, nhấn q để thoát!")

insertOrUpdate(id, name)

sampleNum = 0

while True:
    try:
        # Đọc hình ảnh từ ESP32-CAM
        img_resp = urllib.request.urlopen(url)
        img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        img = cv2.imdecode(img_np, -1)

        if img is None:
            continue

        # Lật ảnh nếu cần
        img = cv2.flip(img, 1)

        # Vẽ khung giữa màn hình để người dùng đưa mặt vào khu vực này
        centerH = img.shape[0] // 2
        centerW = img.shape[1] // 2
        sizeboxW = 300
        sizeboxH = 400
        cv2.rectangle(img, (centerW - sizeboxW // 2, centerH - sizeboxH // 2),
                      (centerW + sizeboxW // 2, centerH + sizeboxH // 2), (255, 255, 255), 5)

        # Đưa ảnh về ảnh xám
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Nhận diện khuôn mặt
        faces = detector.detectMultiScale(gray, 1.3, 5)
        for (x, y, w, h) in faces:
            # Vẽ hình chữ nhật quanh khuôn mặt
            cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
            sampleNum += 1

            # Lưu hình ảnh khuôn mặt vào thư mục dataSet
            cv2.imwrite("dataSet/User." + id + '.' + str(sampleNum) + ".jpg", gray[y:y + h, x:x + w])

        # Hiển thị hình ảnh với khung và khuôn mặt
        cv2.imshow('ESP32-CAM', img)

        # Nhấn 'q' để thoát hoặc thu thập đủ 300 ảnh
        if cv2.waitKey(100) & 0xFF == ord('q'):
            break
        elif sampleNum >= 300:
            break

    except Exception as e:
        print("Error: ", str(e))

# Giải phóng bộ nhớ
cv2.destroyAllWindows()
