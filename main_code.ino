#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ═════════════════════════ PIN DEFINITIONS ═════════════════════════

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// RFID
#define SS_PIN 5
#define RST_PIN 4

// ALERTS
#define RED_LED 15
#define BUZZER 2

// ═════════════════════════ OBJECTS ════════════════════════════════

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MFRC522 rfid(SS_PIN, RST_PIN);
RTC_DS3231 rtc;

// ═════════════════════════ KEYPAD ════════════════════════════════

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {12, 13, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ═════════════════════════ PASSWORDS ═════════════════════════════

const String ADMIN_PASSWORD = "9999";
const String STAFF_PASSWORD = "7777";

bool isAdmin = false;
bool isStaff = false;

String enteredPassword = "";
char loginMode = 0;

// ═════════════════════════ PRODUCTS ══════════════════════════════

#define NUM_PRODUCTS 4

const char* productNames[NUM_PRODUCTS] = {
  "T-SHIRT",
  "PANT",
  "TRACKSUIT",
  "SHOE"
};

// RFID CARD UIDS
byte rfidUIDs[NUM_PRODUCTS][4] = {
  {0x78, 0x11, 0x9F, 0xBA},
  {0xA5, 0xBA, 0x1C, 0xAA},
  {0xC5, 0xD7, 0xE9, 0xAA},
  {0x87, 0x48, 0xD6, 0x05}
};

int stock[NUM_PRODUCTS]     = {4, 4, 4, 4};
int soldCount[NUM_PRODUCTS] = {0, 0, 0, 0};

// ═════════════════════════ HELPERS ═══════════════════════════════

void beepOK() {
  tone(BUZZER, 1800, 150);
}

void beepError() {
  tone(BUZZER, 500, 400);
}

void lowStockBuzz() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 1000, 200);
    delay(300);
    digitalWrite(RED_LED, LOW);
    delay(300);
  }
}

String twoDigit(int n) {
  return (n < 10 ? "0" : "") + String(n);
}

String getTimeStr() {
  DateTime now = rtc.now();
  return twoDigit(now.hour()) + ":" +
         twoDigit(now.minute()) + ":" +
         twoDigit(now.second());
}

String getDateStr() {
  DateTime now = rtc.now();
  return String(now.year()) + "-" +
         twoDigit(now.month()) + "-" +
         twoDigit(now.day());
}

int identifyCard() {
  if (rfid.uid.size != 4) return -1;
  for (int p = 0; p < NUM_PRODUCTS; p++) {
    bool match = true;
    for (int b = 0; b < 4; b++) {
      if (rfid.uid.uidByte[b] != rfidUIDs[p][b]) {
        match = false;
        break;
      }
    }
    if (match) return p;
  }
  return -1;
}

int getBestseller() {
  int best = -1;
  int maxSold = 0;
  for (int i = 0; i < NUM_PRODUCTS; i++) {
    if (soldCount[i] > maxSold) {
      maxSold = soldCount[i];
      best = i;
    }
  }
  return best;
}

// ═════════════════════════ OLED FUNCTIONS ════════════════════════

void oledClear() {
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void oledShow(
  const char* line1,
  const char* line2 = "",
  const char* line3 = "",
  const char* line4 = ""
) {
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);  display.println(line1);
  display.setCursor(0, 20); display.println(line2);
  display.setCursor(0, 35); display.println(line3);
  display.setCursor(0, 50); display.println(line4);
  display.display();
}

// ═════════════════════════ LOGIN MENU ════════════════════════════

void showLoginMenu() {
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("STOCKSENSE PRO");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 22);
  display.println("A = ADMIN LOGIN");
  display.setCursor(0, 38);
  display.println("B = STAFF LOGIN");
  display.display();
  Serial.println("[MENU] Login menu shown.");
}

// ═════════════════════════ ADMIN MENU ════════════════════════════

void showAdminMenu() {
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ADMIN DASHBOARD");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 16); display.println("1.More Stock");
  display.setCursor(0, 26); display.println("2.View Stocks");
  display.setCursor(0, 36); display.println("3.Customer Fav");
  display.setCursor(0, 46); display.println("4.Inventory");
  display.setCursor(0, 56); display.println("5.Logout");
  display.display();
  Serial.println("[MENU] Admin dashboard shown.");
}

// ═════════════════════════ SHOW STOCKS ═══════════════════════════

void showAllStocks() {
  Serial.println("[INFO] Showing all stocks...");
  for (int i = 0; i < NUM_PRODUCTS; i++) {
    oledClear();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(productNames[i]);
    display.drawLine(0, 10, 127, 10, WHITE);
    display.setCursor(0, 25);
    display.print("Stock: "); display.println(stock[i]);
    display.setCursor(0, 45);
    display.print("Sold : "); display.println(soldCount[i]);
    display.display();
    Serial.print("[STOCK] "); Serial.print(productNames[i]);
    Serial.print(" | Stock: "); Serial.print(stock[i]);
    Serial.print(" | Sold: "); Serial.println(soldCount[i]);
    delay(2500);
  }
}

// ═════════════════════════ FAVORITE PRODUCT ══════════════════════

void showCustomerFavorite() {
  int best = getBestseller();
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("CUSTOMER FAVORITE");
  display.drawLine(0, 10, 127, 10, WHITE);
  if (best == -1) {
    display.setCursor(0, 30);
    display.println("NO SALES YET");
    Serial.println("[INFO] No sales yet.");
  } else {
    display.setCursor(0, 26);
    display.println(productNames[best]);
    display.setCursor(0, 45);
    display.print("Sold: "); display.println(soldCount[best]);
    Serial.print("[FAV] Best seller: "); Serial.print(productNames[best]);
    Serial.print(" | Sold: "); Serial.println(soldCount[best]);
  }
  display.display();
  delay(4000);
}

// ═════════════════════════ INVENTORY REPORT ══════════════════════

void inventoryAnalytics() {
  int totalStock = 0;
  int totalSold  = 0;
  for (int i = 0; i < NUM_PRODUCTS; i++) {
    totalStock += stock[i];
    totalSold  += soldCount[i];
  }
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("INVENTORY REPORT");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 25);
  display.print("Total Stock: "); display.println(totalStock);
  display.setCursor(0, 45);
  display.print("Total Sold : "); display.println(totalSold);
  display.display();
  Serial.print("[INVENTORY] Total Stock: "); Serial.print(totalStock);
  Serial.print(" | Total Sold: "); Serial.println(totalSold);
  delay(5000);
}

// ═════════════════════════ MORE STOCK (ADD STOCK) ════════════════
// Shows a product selection menu and lets user pick 1-4 to add stock.
// Used by BOTH admin (option 1) and staff (after stock-out prompt).

void moreStockMenu() {
  // Show product list
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ADD STOCK");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 16); display.print("1."); display.println(productNames[0]);
  display.setCursor(0, 26); display.print("2."); display.println(productNames[1]);
  display.setCursor(0, 36); display.print("3."); display.println(productNames[2]);
  display.setCursor(0, 46); display.print("4."); display.println(productNames[3]);
  display.setCursor(0, 56); display.println("* = Cancel");
  display.display();
  Serial.println("[MORE STOCK] Product selection shown.");

  // Wait for keypress
  char k = 0;
  while (true) {
    k = keypad.getKey();
    if (k) break;
    delay(50);
  }

  if (k == '*' || k == 'D') {
    Serial.println("[MORE STOCK] Cancelled.");
    return;
  }

  int idx = -1;
  if (k == '1') idx = 0;
  else if (k == '2') idx = 1;
  else if (k == '3') idx = 2;
  else if (k == '4') idx = 3;

  if (idx == -1) {
    beepError();
    oledShow("INVALID", "SELECTION");
    delay(1500);
    Serial.println("[MORE STOCK] Invalid key pressed.");
    return;
  }

  // Ask how many to add
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("HOW MANY TO ADD?");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 20);
  display.println(productNames[idx]);
  display.setCursor(0, 38);
  display.println("Press 1-9");
  display.setCursor(0, 52);
  display.println("* = Cancel");
  display.display();
  Serial.println("[MORE STOCK] Waiting for quantity...");

  char qKey = 0;
  while (true) {
    qKey = keypad.getKey();
    if (qKey) break;
    delay(50);
  }

  if (qKey == '*' || qKey == 'D') {
    Serial.println("[MORE STOCK] Cancelled at quantity.");
    return;
  }

  int qty = qKey - '0';
  if (qty < 1 || qty > 9) {
    beepError();
    oledShow("INVALID", "QUANTITY");
    delay(1500);
    Serial.println("[MORE STOCK] Invalid quantity.");
    return;
  }

  // Add stock
  stock[idx] += qty;

  String ts   = getTimeStr();
  String date = getDateStr();

  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("STOCK ADDED");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 20);
  display.println(productNames[idx]);
  display.setCursor(0, 32);
  display.print("Added  : +"); display.println(qty);
  display.setCursor(0, 44);
  display.print("Total  : "); display.println(stock[idx]);
  display.setCursor(0, 56);
  display.println(ts);
  display.display();

  beepOK();

  Serial.print("[STOCK IN] ");
  Serial.print(productNames[idx]);
  Serial.print(" | +"); Serial.print(qty);
  Serial.print(" | Total: "); Serial.print(stock[idx]);
  Serial.print(" | "); Serial.print(date);
  Serial.print(" "); Serial.println(ts);

  delay(3000);
}

// ═════════════════════════ RFID WAIT ════════════════════════════

int waitForCard(unsigned long timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    rfid.PCD_Init();
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      int idx = identifyCard();
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      return idx;
    }
    delay(50);
  }
  return -2; // timeout
}

// ═════════════════════════ CARD SCAN ════════════════════════════

void handleCardScan(int productIdx) {
  String ts   = getTimeStr();
  String date = getDateStr();

  if (stock[productIdx] == 0) {
    // Auto stock-in if stock was 0
    stock[productIdx]++;

    oledClear();
    display.setTextSize(1);
    display.setCursor(0, 0);  display.println("AUTO STOCK IN");
    display.drawLine(0, 10, 127, 10, WHITE);
    display.setCursor(0, 18); display.println(productNames[productIdx]);
    display.setCursor(0, 32); display.println("Stock Was 0");
    display.setCursor(0, 44); display.println(date);
    display.setCursor(0, 56); display.println(ts);
    display.display();

    beepOK();

    Serial.print("[AUTO STOCK IN] ");
    Serial.print(productNames[productIdx]);
    Serial.print(" | Stock: "); Serial.print(stock[productIdx]);
    Serial.print(" | "); Serial.print(date);
    Serial.print(" "); Serial.println(ts);

    delay(3000);

  } else {
    // Normal stock-out
    stock[productIdx]--;
    soldCount[productIdx]++;

    oledClear();
    display.setTextSize(1);
    display.setCursor(0, 0);  display.println("STOCK OUT");
    display.drawLine(0, 10, 127, 10, WHITE);
    display.setCursor(0, 18); display.println(productNames[productIdx]);
    display.setCursor(0, 32);
    display.print("Remaining: "); display.println(stock[productIdx]);
    display.setCursor(0, 44); display.println(date);
    display.setCursor(0, 56); display.println(ts);
    display.display();

    beepOK();

    Serial.print("[STOCK OUT] ");
    Serial.print(productNames[productIdx]);
    Serial.print(" | Remaining: "); Serial.print(stock[productIdx]);
    Serial.print(" | "); Serial.print(date);
    Serial.print(" "); Serial.println(ts);

    delay(3000);

    if (stock[productIdx] == 1) {
      oledShow(
        "LOW STOCK ALERT",
        productNames[productIdx],
        "ONLY 1 LEFT"
      );
      Serial.print("[ALERT] Low stock: "); Serial.println(productNames[productIdx]);
      lowStockBuzz();
      delay(2500);
    }
  }
}

// ═════════════════════════ UNKNOWN CARD ═════════════════════════

void handleUnknownCard() {
  digitalWrite(RED_LED, HIGH);
  beepError();
  oledShow("UNKNOWN CARD", "NOT REGISTERED");
  Serial.println("[ERROR] Unknown RFID card scanned.");
  delay(2000);
  digitalWrite(RED_LED, LOW);
}

// ═════════════════════════ STAFF: MORE STOCK PROMPT ══════════════
// After a successful staff stock-out, ask if they want to add stock.

void staffMoreStockPrompt() {
  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ADD MORE STOCK?");
  display.drawLine(0, 10, 127, 10, WHITE);
  display.setCursor(0, 25);
  display.println("A = YES");
  display.setCursor(0, 40);
  display.println("B = NO");
  display.display();
  Serial.println("[STAFF] More stock prompt shown.");

  char k = 0;
  unsigned long start = millis();
  while (millis() - start < 10000) { // 10 second timeout
    k = keypad.getKey();
    if (k) break;
    delay(50);
  }

  if (k == 'A') {
    Serial.println("[STAFF] Chose to add more stock.");
    moreStockMenu();
  } else {
    Serial.println("[STAFF] Chose not to add more stock.");
    oledShow("OK", "DONE");
    delay(1000);
  }
}

// ═════════════════════════ STAFF ACCESS FLOW ════════════════════
// Staff can scan multiple RFID cards in a session until timeout.
// After each valid scan, offer the more-stock option.

void staffAccessFlow() {
  while (true) {
    oledClear();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("STAFF ACCESS");
    display.drawLine(0, 10, 127, 10, WHITE);
    display.setCursor(0, 20);
    display.println("SCAN RFID CARD");
    display.setCursor(0, 35);
    display.println("10 SEC TIMEOUT");
    display.setCursor(0, 50);
    display.println("* = EXIT");
    display.display();
    Serial.println("[STAFF] Waiting for RFID card...");

    // Wait for card OR keypress to exit
    int idx = -2;
    unsigned long start = millis();
    while (millis() - start < 10000) {
      char k = keypad.getKey();
      if (k == '*' || k == 'D') {
        Serial.println("[STAFF] Exited by keypress.");
        return; // exit staff flow
      }
      rfid.PCD_Init();
      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        idx = identifyCard();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        break;
      }
      delay(50);
    }

    if (idx == -2) {
      // Timeout
      oledShow("TIMEOUT", "NO CARD SCANNED");
      Serial.println("[STAFF] RFID timeout.");
      delay(2000);
      return;
    } else if (idx == -1) {
      handleUnknownCard();
      // After unknown card, ask to try again or exit
      oledClear();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("TRY AGAIN?");
      display.drawLine(0, 10, 127, 10, WHITE);
      display.setCursor(0, 25); display.println("A = YES");
      display.setCursor(0, 40); display.println("B = NO");
      display.display();
      char retry = 0;
      unsigned long t = millis();
      while (millis() - t < 8000) {
        retry = keypad.getKey();
        if (retry) break;
        delay(50);
      }
      if (retry != 'A') return;
    } else {
      // Valid card scanned
      handleCardScan(idx);
      // After stock-out, offer more stock
      staffMoreStockPrompt();
      // Ask if staff wants to scan another card
      oledClear();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("SCAN ANOTHER?");
      display.drawLine(0, 10, 127, 10, WHITE);
      display.setCursor(0, 25); display.println("A = YES");
      display.setCursor(0, 40); display.println("B = NO");
      display.display();
      Serial.println("[STAFF] Scan another prompt.");
      char another = 0;
      unsigned long t2 = millis();
      while (millis() - t2 < 8000) {
        another = keypad.getKey();
        if (another) break;
        delay(50);
      }
      if (another != 'A') return;
    }
  }
}

// ═════════════════════════ STARTUP ══════════════════════════════

void runStartupSequence() {
  oledClear();
  display.setTextSize(2);
  display.setCursor(5, 20);
  display.println("STOCKSENSE");
  display.display();
  Serial.println("[BOOT] StockSense Pro starting...");
  delay(3000);

  oledClear();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("WELCOME TO");
  display.setCursor(0, 28);
  display.println("WAREHOUSE");
  display.setCursor(0, 46);
  display.println("MANAGEMENT");
  display.display();
  Serial.println("[BOOT] Welcome screen.");
  delay(3000);

  showLoginMenu();
}

// ═════════════════════════ SETUP ════════════════════════════════

void setup() {
  Serial.begin(115200);
  delay(500); // Allow USB serial to stabilize
  Serial.println("=========================");
  Serial.println(" StockSense Pro BOOT");
  Serial.println("=========================");

  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RED_LED, LOW);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ERROR] OLED init failed!");
  } else {
    Serial.println("[OK] OLED initialized.");
  }
  display.setTextColor(WHITE);

  if (!rtc.begin()) {
    Serial.println("[ERROR] RTC not found!");
  } else {
    Serial.println("[OK] RTC initialized.");
    if (rtc.lostPower()) {
      Serial.println("[WARN] RTC lost power, setting time to compile time.");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    Serial.print("[RTC] Current time: ");
    Serial.print(getDateStr());
    Serial.print(" ");
    Serial.println(getTimeStr());
  }

  SPI.begin();
  rfid.PCD_Init();
  Serial.println("[OK] RFID initialized.");

  runStartupSequence();
}

// ═════════════════════════ LOOP ═════════════════════════════════

void loop() {
  char key = keypad.getKey();

  // ═════════ LOGIN MENU ═════════

  if (!isAdmin && !isStaff) {

    if (key == 'A') {
      loginMode = 'A';
      enteredPassword = "";
      oledShow("ADMIN LOGIN", "ENTER PASSWORD");
      Serial.println("[LOGIN] Admin mode selected.");
      delay(500);
      return;
    }

    if (key == 'B') {
      loginMode = 'B';
      enteredPassword = "";
      oledShow("STAFF LOGIN", "ENTER PASSWORD");
      Serial.println("[LOGIN] Staff mode selected.");
      delay(500);
      return;
    }

    if (key && key != 'A' && key != 'B') {

      if (key == '*' || key == 'D') {
        enteredPassword = "";
        showLoginMenu();
        return;
      }

      enteredPassword += key;

      oledClear();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("ENTER PASSWORD");
      display.setCursor(0, 30);
      for (int i = 0; i < (int)enteredPassword.length(); i++) {
        display.print("* ");
      }
      display.display();

      if ((int)enteredPassword.length() == 4) {

        if (loginMode == 'A' && enteredPassword == ADMIN_PASSWORD) {
          isAdmin = true;
          isStaff = false;
          oledShow("ADMIN ACCESS", "GRANTED");
          beepOK();
          Serial.println("[LOGIN] Admin login successful.");
          delay(1500);
          showAdminMenu();
        }

        else if (loginMode == 'B' && enteredPassword == STAFF_PASSWORD) {
          isStaff = true;
          isAdmin = false;
          oledShow("STAFF ACCESS", "GRANTED");
          beepOK();
          Serial.println("[LOGIN] Staff login successful.");
          delay(1500);
          staffAccessFlow();          // Full staff RFID + more-stock flow
          isStaff = false;
          showLoginMenu();
        }

        else {
          beepError();
          oledShow("ACCESS DENIED", "WRONG PASSWORD");
          Serial.println("[LOGIN] Wrong password entered.");
          delay(2000);
          showLoginMenu();
        }

        enteredPassword = "";
      }
    }
  }

  // ═════════ ADMIN DASHBOARD ═════════

  if (isAdmin) {

    if (key == '1') {
      // More Stock
      Serial.println("[ADMIN] More Stock selected.");
      moreStockMenu();
      showAdminMenu();
    }

    else if (key == '6') {
      Serial.println("[ADMIN] View Stocks selected.");
      showAllStocks();
      showAdminMenu();
    }

    else if (key == '3') {
      Serial.println("[ADMIN] Customer Favorite selected.");
      showCustomerFavorite();
      showAdminMenu();
    }

    else if (key == '4') {
      Serial.println("[ADMIN] Inventory Analytics selected.");
      inventoryAnalytics();
      showAdminMenu();
    }

    else if (key == '5') {
      isAdmin = false;
      oledShow("LOGGING OUT");
      Serial.println("[ADMIN] Logged out.");
      delay(1500);
      showLoginMenu();
    }
  }

  yield();
}
