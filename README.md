📦 StockSense Pro
RFID-Based Smart Inventory Management System with Security & Usage Analytics

StockSense Pro is an embedded-system-driven smart inventory management solution designed to automate stock tracking, enhance security, and provide real-time usage analytics for retail and warehouse environments.

Originally developed on the Arduino Uno, the system was later migrated to the ESP32 platform to overcome memory constraints and enable future IoT scalability.

It replaces traditional manual inventory methods with an automated RFID-based tracking system, secure role-based authentication, and a real-time OLED dashboard.

🚀 Key Features
📡 Smart Inventory Tracking
RFID-based instant product identification
Automatic stock-in / stock-out updates
Real-time inventory level monitoring
🔐 Secure Role-Based Access
Separate Admin and Staff login systems
4-digit password authentication via keypad
Unauthorized access detection with buzzer & LED alerts
⏱️ Real-Time Logging
DS3231 RTC integration for accurate timestamps
Every transaction logged with date & time metadata
📊 Intelligent Analytics Dashboard
Tracks best-selling products (“Customer Favorites”)
Displays stock remaining and total sales count
Low-stock warning system
🖥️ Interactive OLED UI
Multi-layer menu system on 128x64 OLED display
Navigation via 4x4 matrix keypad
Clean and responsive embedded interface
🔊 Alerts & Indicators
Active buzzer for security breaches
LED indicators for system warnings and low stock
🧠 System Architecture
🏗️ Initialization Phase
ESP32 initializes SPI & I2C communication buses
RFID reader (RC522) and RTC module are activated
OLED splash screen followed by secure login interface
🔐 Authentication Phase
User selects role: Admin / Staff
Enters 4-digit PIN via keypad
Access granted or denied based on validation
🎛️ Operation Phase
Users access dashboards and stock controls
RFID scan triggers product recognition and updates inventory
📈 Data Logging Phase
Each transaction recorded with timestamp
Stock levels updated dynamically
Alerts triggered for low-stock conditions
🛠️ Hardware Components
ESP32 Development Board – Main controller & IoT-ready processor
RC522 RFID Module – Product identification via SPI
DS3231 RTC Module – Real-time clock tracking
OLED Display (128x64) – UI dashboard
4x4 Matrix Keypad – User input & navigation
RFID Tags/Cards – Product identifiers
Active Buzzer – Security alerts
LED Indicator – Status notifications
Breadboard & Jumper Wires – Circuit prototyping
💻 Software Stack
Arduino IDE (Development Environment)
MFRC522 – RFID communication
RTClib – Time and date handling
Adafruit SSD1306 & GFX – OLED UI rendering
Keypad Library – Matrix keypad input handling
SPI & Wire – Core communication protocols
⚙️ Why ESP32?

The system was migrated from Arduino Uno to ESP32 due to:

🚀 Higher processing power and clock speed
💾 Increased SRAM and flash memory
🔄 Stable multitasking for OLED, RFID, and keypad operations
🌐 Built-in Wi-Fi & Bluetooth for future IoT expansion
⚠️ Current Limitations
Data is not persistently stored (resets on power loss)
No cloud or remote dashboard integration yet
RFID requires close-range scanning
🔮 Future Improvements
☁️ Cloud integration (Firebase / AWS IoT)
📡 Web-based live dashboard
🔐 Fingerprint-based authentication
📲 SMS / Email alert system for low stock
🤖 AI-based demand prediction system
🏢 Applications

StockSense Pro is suitable for:

Retail stores & micro-warehouses
Medical and laboratory inventory systems
Educational institutions (libraries & supply rooms)
📄 License

This project is licensed under the MIT License, allowing free use, modification, and distribution.
