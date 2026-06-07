📦 StockSense Pro
RFID-Based Smart Inventory Management System with Security & Usage Analytics

🧠 Overview
StockSense Pro is an embedded smart inventory management system that automates stock monitoring and warehouse security using RFID technology. Built on the ESP32 microcontroller, it combines RFID-based product identification, password-protected access control, real-time stock tracking, low-stock alerts, and usage analytics into a single compact solution.

Designed for retail shops, warehouses, laboratories, and small businesses — StockSense Pro eliminates manual counting errors and brings smart automation to everyday inventory management.


✨ Features

🔐 Role-Based Access Control — Separate Admin and Staff login modes with 4-digit keypad authentication
📡 RFID Product Identification — Instant product detection via RC522 RFID module
📊 Inventory Analytics — Best-selling product detection, total sold/remaining stock reports
⚠️ Low-Stock Alerts — Buzzer and LED warnings when stock drops below threshold
🕒 Date & Time Logging — All stock-in/out events timestamped via DS3231 RTC module
🖥️ OLED Display UI — Real-time inventory information on a 128×64 OLED screen
🔒 Security System — Unknown RFID detection and access denial for unauthorized users
➕ Auto Stock-In — Add stock directly through the keypad menu


🛠️ Hardware Components
ComponentQtyPurposeESP32 Development Board1Main controllerRC522 RFID Module1RFID card readingRFID Cards/Tags4+Product identificationOLED Display (128×64)1System UIRTC DS3231 Module1Date & time logging4×4 Matrix Keypad1Password & menu inputActive Buzzer1Audio alertsYellow LED1Visual warningBreadboard + Jumper Wires—Prototyping

📚 Libraries Used
LibraryPurposeMFRC522RFID communicationRTClibRTC date/time managementAdafruit SSD1306OLED display controlAdafruit GFXGraphics supportKeypadKeypad input handlingWireI2C communicationSPISPI communication

⚙️ How It Works
Power On → System Init → Login Screen → RFID Scan → Stock Update → Analytics

Initialization — All modules start up; OLED shows startup screen
Authentication — User selects Admin or Staff login and enters 4-digit password
RFID Scanning — Product RFID card is scanned and matched to inventory
Stock Management — Quantity updates, sold counter increments, timestamp recorded
Analytics — View total stock, sold items, best-selling product, and low-stock alerts

Registered Products (Demo)
ProductRFID TagT-ShirtRFID B1PantRFID B3TracksuitRFID B4ShoeWhite RFID

🔒 Security

Password authentication with wrong-password buzzer alert
Separate Admin / Staff privilege levels
Unknown RFID card detection with audio-visual warnings
Access denial for unregistered users


⚠️ Current Limitations

Data does not persist after power loss (no permanent storage)
Limited RFID scan range
No cloud/internet connectivity
No mobile app support


🚀 Future Improvements

 Cloud database integration (Firebase / AWS)
 Wi-Fi inventory dashboard
 Mobile app control
 Fingerprint authentication
 Email/SMS low-stock alerts
 Barcode scanner support
 AI-based demand prediction
 Automatic report generation


📌 Why ESP32 over Arduino Uno?
The project was initially built on Arduino Uno but migrated to ESP32 due to:

Insufficient SRAM and flash storage on Uno
Difficulty running RFID + OLED + RTC libraries simultaneously
Need for better multitasking and future IoT scalability

ESP32 resolved all of these constraints and enabled stable, high-performance operation.

👤 Author
Md. Atif Absar | Safoan Ahmed Sany — CSE Undergrad, KUET

📄 License
This project is for educational purposes. Feel free to fork, improve, and build upon it.
