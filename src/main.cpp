#include <Arduino.h>

#define BTN 15
#define RELAY 16

bool relayState = false; // 追蹤繼電器狀態
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150; // 去抖動時間
String inputString = "";      // 存儲串口輸入的字符串
bool stringComplete = false;  // 字符串是否完整接收

void setup()
{
    pinMode(BTN, INPUT_PULLUP); // 使用內建上拉電阻
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW); // 預設關閉繼電器
    Serial.begin(115200); // 初始化 Serial 通訊
    inputString.reserve(10); // 保留字符串空間
}

void loop()
{
    // 處理按鈕輸入（保留原有功能）
    bool currentButtonState = digitalRead(BTN);

    if (currentButtonState == LOW && lastButtonState == HIGH)
    {
        if (millis() - lastDebounceTime > debounceDelay)
        {
            relayState = !relayState; // 切換繼電器狀態
            digitalWrite(RELAY, relayState ? HIGH : LOW);
            Serial.println(relayState ? "ON" : "OFF"); // 顯示狀態
            lastDebounceTime = millis();
        }
    }
    lastButtonState = currentButtonState;

    // 處理串口輸入
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        
        // 如果收到換行符號，表示一次輸入完成
        if (inChar == '\n') {
            stringComplete = true;
        } else {
            // 否則，將字符添加到字符串
            inputString += inChar;
        }
    }

    // 處理完整接收的字符串
    if (stringComplete) {
        // 嘗試將接收到的字符串轉換為整數
        int value = inputString.toInt();
        
        // 如果收到1，開啟繼電器
        if (value == 1) {
            relayState = true;
            digitalWrite(RELAY, HIGH);
            Serial.println("ON");
        } 
        // 如果收到0，關閉繼電器
        else if (value == 0) {
            relayState = false;
            digitalWrite(RELAY, LOW);
            Serial.println("OFF");
        }
        
        // 清空字符串，準備下一次輸入
        inputString = "";
        stringComplete = false;
    }
}