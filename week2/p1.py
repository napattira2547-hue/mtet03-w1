import tkinter as tk
from tkinter import font

# --- ฟังก์ชันการทำงาน ---
def press_key(key):
    """ฟังก์ชันเมื่อกดปุ่มตัวเลขหรือเครื่องหมาย"""
    current = entry_var.get()
    if current == "Error":
        entry_var.set(key)
    else:
        entry_var.set(current + str(key))

def clear():
    """ฟังก์ชันล้างหน้าจอ (C)"""
    entry_var.set("")

def delete():
    """ฟังก์ชันลบทีละตัวอักษร (⌫)"""
    current = entry_var.get()
    if current == "Error":
        entry_var.set("")
    else:
        entry_var.set(current[:-1])

def calculate():
    """ฟังก์ชันคำนวณผลลัพธ์ (=)"""
    try:
        expression = entry_var.get()
        # เปลี่ยนเครื่องหมายเพื่อให้ python คำนวณได้
        expression = expression.replace('×', '*').replace('÷', '/')
        result = eval(expression)
        
        # ถ้ารันผลลัพธ์แล้วเป็นทศนิยม .0 ให้ตัดออกเป็นจำนวนเต็ม
        if isinstance(result, float) and result.is_integer():
            result = int(result)
            
        entry_var.set(result)
    except Exception:
        entry_var.set("Error")

# --- สร้างหน้าต่างโปรแกรม ---
root = tk.Tk()
root.title("Rainbow Calculator")
root.geometry("360x520")
root.configure(bg="#f7f9fa")  # พื้นหลังสีขาวนวลสะอาดตา
root.resizable(False, False)  # ล็อกขนาดหน้าต่าง

# ตั้งค่าฟอนต์
app_font = font.Font(family="Helvetica", size=16, weight="bold")
display_font = font.Font(family="Helvetica", size=30, weight="bold")

# ตัวแปรเก็บค่าหน้าจอ
entry_var = tk.StringVar()

# --- หน้าจอแสดงผล ---
display_frame = tk.Frame(root, bg="#f7f9fa", padx=12, pady=15)
display_frame.pack(expand=True, fill="both")

# กรอบหน้าจอสีรุ้งพาสเทล (ชมพู-ม่วง)
display = tk.Entry(
    display_frame, 
    textvariable=entry_var, 
    font=display_font, 
    justify="right", 
    bd=0, 
    bg="#ffffff",      
    fg="#4a4a4a",      
    insertbackground="#ff6b81",
    highlightthickness=3,
    highlightbackground="#f8a5c2",  # ขอบสีชมพูพาสเทล
    highlightcolor="#9c88ff"        # ขอบเปลี่ยนเป็นสีม่วงเมื่อคลิก
)
display.pack(expand=True, fill="both", ipady=12)

# --- โซนปุ่มกดสีรุ้ง (Rainbow Grid) ---
button_frame = tk.Frame(root, bg="#f7f9fa", padx=12, pady=12)
button_frame.pack(expand=True, fill="both")

# ออกแบบปุ่มโดยไล่สีตามแถว (Row-by-Row Rainbow Pastel)
# โครงสร้าง: (ข้อความ, แถว, คอลัมน์, สีปุ่ม, สีตัวอักษร, ฟังก์ชัน)
buttons = [
    # แถวที่ 0: โทนแดง/ส้มพาสเทล
    ('C', 0, 0, '#ff6b6b', '#ffffff', clear),
    ('⌫', 0, 1, '#ff8787', '#ffffff', delete),
    ('÷', 0, 2, '#fca5a5', '#ffffff', lambda: press_key('÷')),
    ('×', 0, 3, '#ffd166', '#ffffff', lambda: press_key('×')),
    
    # แถวที่ 1: โทนเหลือง/เขียวพาสเทล
    ('7', 1, 0, '#ffe066', '#4a4a4a', lambda: press_key(7)),
    ('8', 1, 1, '#d8f3dc', '#4a4a4a', lambda: press_key(8)),
    ('9', 1, 2, '#b7e4c7', '#4a4a4a', lambda: press_key(9)),
    ('-', 1, 3, '#95d5b2', '#ffffff', lambda: press_key('-')),
    
    # แถวที่ 2: โทนเขียวมิ้นท์/ฟ้าพาสเทล
    ('4', 2, 0, '#74c69d', '#ffffff', lambda: press_key(4)),
    ('5', 2, 1, '#a8dadc', '#4a4a4a', lambda: press_key(5)),
    ('6', 2, 2, '#457b9d', '#ffffff', lambda: press_key(6)),
    ('+', 2, 3, '#a2d2ff', '#ffffff', lambda: press_key('+')),
    
    # แถวที่ 3: โทนน้ำเงิน/ม่วงพาสเทล
    ('1', 3, 0, '#bde0fe', '#4a4a4a', lambda: press_key(1)),
    ('2', 3, 1, '#ffafcc', '#4a4a4a', lambda: press_key(2)),
    ('3', 3, 2, '#ffc8dd', '#4a4a4a', lambda: press_key(3)),
    ('=', 3, 4, '#cdb4db', '#ffffff', calculate), # ปุ่มผลลัพธ์สีม่วงหวานๆ
    
    # แถวที่ 4: โทนชมพูพาสเทล
    ('0', 4, 0, '#f8a5c2', '#ffffff', lambda: press_key(0)),
    ('.', 4, 2, '#f7d794', '#4a4a4a', lambda: press_key('.')),
]

# ตั้งค่าการขยายตัวของ Grid ให้ปุ่มขยายเต็มหน้าต่าง
for i in range(5):
    button_frame.rowconfigure(i, weight=1, pad=6)
for j in range(4):
    button_frame.columnconfigure(j, weight=1, pad=6)

# สร้างปุ่มลงบน Grid พร้อมกำหนดสีรุ้งตามที่ตั้งไว้
for (text, row, col, bg_color, fg_color, func) in buttons:
    # เคสพิเศษ: ปุ่ม "=" ควบสองแถวด้านขวา
    if text == '=':
        btn = tk.Button(
            button_frame, text=text, font=app_font, bg=bg_color, fg=fg_color,
            activebackground='#b39bc8', activeforeground='#ffffff', bd=0, 
            relief="flat", cursor="hand2"
        )
        btn.configure(command=func)
        btn.grid(row=row, column=3, rowspan=2, sticky="nsew", padx=4, pady=4)
        
    # เคสพิเศษ: ปุ่ม "0" ควบสองคอลัมน์ด้านซ้ายล่าง
    elif text == '0':
        btn = tk.Button(
            button_frame, text=text, font=app_font, bg=bg_color, fg=fg_color,
            activebackground='#f5cdde', activeforeground='#ffffff', bd=0, 
            relief="flat", cursor="hand2"
        )
        btn.configure(command=func)
        btn.grid(row=row, column=col, columnspan=2, sticky="nsew", padx=4, pady=4)
        
    # ปุ่มตัวเลขและเครื่องหมายทั่วไป
    else:
        btn = tk.Button(
            button_frame, text=text, font=app_font, bg=bg_color, fg=fg_color,
            activebackground='#e2e2e2', activeforeground=fg_color, bd=0, 
            relief="flat", cursor="hand2"
        )
        btn.configure(command=func)
        btn.grid(row=row, column=col, sticky="nsew", padx=4, pady=4)

# รันโปรแกรม
root.mainloop()