StockSense Pro
RFID-Based Smart Inventory Management System with Security & Usage Analytics

StockSense Pro is a smart, embedded inventory management system that automates stock tracking, enhances warehouse security, and provides real-time usage analytics. Built around the ESP32 microcontroller, it combines RFID-based product identification, password-protected access control, low-stock alerts, and activity logging into a compact, low-cost solution.

Originally prototyped on Arduino Uno, the system was upgraded to ESP32 to overcome memory and performance limitations.

🚀 Features
🔐 Secure Access – Admin and staff login with 4-digit password authentication

🆔 RFID Product Identification – Assign unique RFID tags to products

📦 Automated Stock Management – Auto-updates stock-in/out with timestamps

📊 Real-Time Analytics – Track total sold, remaining stock, and best-selling products

🖥️ OLED Display Interface – Intuitive 128x64 user menu

⏰ Date/Time Logging – RTC DS3231 module records all inventory activities

🚨 Low-Stock Alerts – Buzzer + LED warnings

📈 Customer Favorite Detection – Identifies top-selling products

🔁 Scalable Design – Ready for future IoT integration

🧰 Components Used
Component	Quantity	Purpose
ESP32 Dev Board	1	Main controller
RC522 RFID Module	1	RFID card reading
RFID Cards/Tags	4+	Product identification
OLED Display (128x64)	1	User interface
RTC DS3231 Module	1	Date & time tracking
4x4 Matrix Keypad	1	Password & menu input
Active Buzzer	1	Audio alerts
Yellow LED	1	Visual warning
Breadboard & Jumpers	-	Circuit connections
USB Power Supply	1	System power
🛠️ Software & Libraries
Library / Tool	Purpose
Arduino IDE	Development environment
MFRC522	RFID communication
RTClib	RTC module control
Adafruit SSD1306	OLED display control
Adafruit GFX	Graphics support
Keypad Library	Matrix keypad input
Wire / SPI	I2C & SPI communication
⚙️ Working Principle
Initialization – ESP32 boots up, initializes all modules, and displays the login menu on OLED.

Authentication – User selects Admin or Staff mode and enters a 4-digit password.

RFID Scanning – After login, RFID tags are scanned to identify products.

Stock Update – Stock quantity and sold counters update automatically with timestamp.

Analytics – System generates total stock, sold count, favorite product, and low-stock alerts.

🔒 Security Features
Role-based access (Admin / Staff)

Password authentication

Unknown RFID tag detection

Audio + visual alerts for unauthorized access

📉 Limitations & Future Scope
Current Limitations
No persistent storage (data lost on power failure)

Limited RFID read range

No cloud or internet connectivity

No mobile app support

Future Improvements
☁️ Cloud database (Firebase / AWS)

📱 Mobile app control

📊 Wi-Fi dashboard

🔐 Fingerprint authentication

📧 Email / SMS low-stock alerts

🤖 AI-based demand prediction

🏢 Applications
Retail shops and smart stores

Warehouses and logistics

Laboratories and libraries

Hospitals and educational institutions

👨‍💻 Authors
Developed as an embedded systems project to demonstrate smart, secure, and scalable inventory management using RFID and ESP32.

📜 License
This project is licensed under the MIT License – feel free to use, modify, and distribute.

⭐ Acknowledgments
Arduino & ESP32 communities

Library contributors (MFRC522, RTClib, Adafruit)

Note: This project was successfully migrated from Arduino Uno to ESP32 for improved memory, speed, and future scalability.
