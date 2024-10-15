import sqlite3

# Kết nối tới cơ sở dữ liệu SQLite
conn = sqlite3.connect("FaceBaseNew.db")

# Tạo con trỏ (cursor) để thao tác với cơ sở dữ liệu
cursor = conn.cursor()

# Xác nhận thay đổi trong cơ sở dữ liệu
conn.commit()

# Lấy dữ liệu còn lại từ bảng để kiểm tra
cursor.execute("SELECT * FROM People")
rows = cursor.fetchall()

# In ra những dòng còn lại
for row in rows:
    print(f"ID: {row[0]}, Name: {row[1]}")

# Đóng kết nối với cơ sở dữ liệu
conn.close()
