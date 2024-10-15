import sqlite3

# Kết nối tới cơ sở dữ liệu SQLite
conn = sqlite3.connect("FaceBaseNew.db")

# Tạo con trỏ (cursor) để thao tác với cơ sở dữ liệu
cursor = conn.cursor()

# Xóa bản ghi có ID = 1
cursor.execute("DELETE FROM People WHERE ID = ?", (1,))


# Xóa bản ghi có ID = 'BDCCB21DCCN726'
cursor.execute("DELETE FROM People WHERE ID = ?", ('2',))
cursor.execute("DELETE FROM People WHERE ID = ?", ('3',))
cursor.execute("DELETE FROM People WHERE ID = ?", ('5',))
cursor.execute("DELETE FROM People WHERE ID = ?", ('7',))
cursor.execute("DELETE FROM People WHERE ID = ?", ('726',))

# Xác nhận thay đổi trong cơ sở dữ liệu
conn.commit()

conn.close()