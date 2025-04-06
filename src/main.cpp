#include <Arduino.h>

#define BTN 15
#define RELAY 16

bool relayState = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;
String inputString = "";
String lastInputString = "";
bool stringComplete = false;

void setup()
{
    pinMode(BTN, INPUT_PULLUP);
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW);
    Serial.begin(9600);
    inputString.reserve(10);
}

void loop()
{
    bool currentButtonState = digitalRead(BTN);

    if (currentButtonState == LOW && lastButtonState == HIGH)
    {
        if (millis() - lastDebounceTime > debounceDelay)
        {
            relayState = !relayState;
            digitalWrite(RELAY, relayState ? HIGH : LOW);
            Serial.println(relayState ? "ON" : "OFF");
            lastDebounceTime = millis();
        }
    }
    lastButtonState = currentButtonState;

    while (Serial.available())
    {
        char inChar = (char)Serial.read();

        if (inChar == '\n')
        {
            stringComplete = true;
        }
        else
        {
            inputString += inChar;
        }
    }

    if (stringComplete)
    {
        if (inputString.length() == 0)
        {
            stringComplete = false;
            Serial.println("No input received.");
            return;
        }

        if (inputString == lastInputString)
        {
            stringComplete = false;
            Serial.println("Duplicate input received.");
            return;
        }

        if (!inputString.equals("0") && inputString.toInt() == 0)
        {
            Serial.println("Invalid input received.");
            stringComplete = false;
            inputString = "";
            return;
        }

        int value = inputString.toInt();

        if (value == 1)
        {
            relayState = true;
            digitalWrite(RELAY, HIGH);
            Serial.println("ON");
        }
        else if (value == 0)
        {
            relayState = false;
            digitalWrite(RELAY, LOW);
            Serial.println("OFF");
        }

        lastInputString = inputString;
        inputString = "";
        stringComplete = false;
    }
}