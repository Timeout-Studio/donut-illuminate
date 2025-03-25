#include <Arduino.h>

#define BTN 15
#define RELAY 16

bool relayState = false; // 追蹤繼電器狀態
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150; // 去抖動時間

void setup()
{
    pinMode(BTN, INPUT_PULLUP); // 使用內建上拉電阻
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW); // 預設關閉繼電器
    Serial.begin(115200); // 初始化 Serial 通訊
}

void loop()
{
    bool currentButtonState = digitalRead(BTN);

    if (currentButtonState == LOW && lastButtonState == HIGH)
    {
        if (millis() - lastDebounceTime > debounceDelay)
        {
            relayState = !relayState; // 切換繼電器狀態
            digitalWrite(RELAY, relayState ? HIGH : LOW);
            Serial.print("Relay State: ");
            Serial.println(relayState ? "ON" : "OFF"); // 顯示狀態
            lastDebounceTime = millis();
        }
    }
    lastButtonState = currentButtonState;
}