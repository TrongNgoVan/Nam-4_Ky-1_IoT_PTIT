
# Ver 1 nhận diện qua cam Máy tính, nhận diện và gửi thông báo về máy tính.
import cv2
import numpy as np
import sqlite3
import time  # Thêm thư viện time để sử dụng hàm sleep

# Khởi tạo bộ phát hiện khuôn mặt
faceDetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Khởi tạo bộ nhận diện khuôn mặt
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('recognizer/trainner.yml')

# Hàm lấy thông tin người dùng qua ID
def getProfile(id):
    conn = sqlite3.connect("FaceBaseNew.db")
    cursor = conn.execute("SELECT * FROM People WHERE ID=" + str(id))
    profile = None
    for row in cursor:
        profile = row
    conn.close()
    return profile

# Khởi tạo camera
cam = cv2.VideoCapture(0)

while True:
    # Đọc ảnh từ camera
    ret, img = cam.read()

    # Lật ảnh cho đỡ bị ngược
    img = cv2.flip(img, 1)

    # Vẽ khung chữ nhật để định vị vùng người dùng đưa mặt vào
    centerH = img.shape[0] // 2
    centerW = img.shape[1] // 2
    sizeboxW = 300
    sizeboxH = 400
    cv2.rectangle(img, (centerW - sizeboxW // 2, centerH - sizeboxH // 2),
                  (centerW + sizeboxW // 2, centerH + sizeboxH // 2), (255, 255, 255), 5)

    # Chuyển ảnh về xám
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Phát hiện các khuôn mặt trong ảnh camera
    faces = faceDetect.detectMultiScale(gray, 1.3, 5)

    # Lặp qua các khuôn mặt nhận được để hiện thông tin
    for (x, y, w, h) in faces:
        # Vẽ hình chữ nhật quanh mặt
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Nhận diện khuôn mặt, trả ra 2 tham số id: mã nhân viên và dist (độ sai khác)
        id, dist = recognizer.predict(gray[y:y + h, x:x + w])

        profile = None

        # Nếu độ sai khác <= 35 thì lấy profile
        if dist <= 35:
            profile = getProfile(id)
            if profile is not None:
                message = f"Sinh viên: {profile[1]} đã điểm danh"
                print(message)
                
                # Hiển thị thông báo trên màn hình
                cv2.putText(img, message, (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                cv2.imshow('Face', img)
                
              
        # Hiển thị thông tin tên người hoặc Unknown nếu không tìm thấy
        if profile is not None:
            cv2.putText(img, "Name: " + str(profile[1]), (x, y + h + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        else:
            cv2.putText(img, "Name: Unknown", (x, y + h + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    # Hiển thị video
    cv2.imshow('Face', img)

    # Nếu nhấn q thì thoát
    if cv2.waitKey(1) == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()