import cv2
import urllib.request
import numpy as np
# http://192.168.43.181/cam-lo.jpg
# Địa chỉ IP của ESP32-CAM
url = 'http://192.168.43.181/cam-lo.jpg' 
while(1):
    img = urllib.request.urlopen(url)
    img_np= np.array(bytearray(img.read()), dtype=np.uint8)
    frame=cv2.imdecode(img_np,-1)
    print(frame)
    cv2.imshow("ESP32_CAM_Trong",frame)
    if cv2.waitKey(10) & 0xFF==ord('q'):
        frame.release()
        cv2.destroyAllWindows()
        break

