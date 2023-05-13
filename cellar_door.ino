#include <SPI.h>
#include <MFRC522.h>

#define DEBUG

#define SS_PIN 10
#define RST_PIN 9

#define door_push 2
#define door_pull 3

String correctCard = "2272813922";
String correctCardBackup = "19510723820";
String gameCards[] = {
    "321953936",
    "21020213228",
    "322414536",
    "6721018522"};

MFRC522 mfrc522[1];

String dump_byte_array(byte *buffer, byte bufferSize);
bool is_game_card(String cardId);

void open_door(bool active);

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Start");
#endif

    pinMode(door_push, OUTPUT);

    SPI.begin();
    mfrc522[0].PCD_Init(SS_PIN, RST_PIN);
#ifdef DEBUG
    mfrc522[0].PCD_DumpVersionToSerial();
#endif
}

void loop()
{
    if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial())
    {
        String cardId = dump_byte_array(mfrc522[0].uid.uidByte, mfrc522[0].uid.size);
        mfrc522[0].PCD_Init(SS_PIN, RST_PIN);
#ifdef DEBUG
        Serial.print(F("Card ID: "));
        Serial.println(cardId);
#endif
        if (cardId == correctCard || cardId == correctCardBackup)
        {
#ifdef DEBUG
            Serial.print(F("Correct card\n"));
#endif
            open_door(true);
        }
        else if (is_game_card(cardId))
        {
#ifdef DEBUG
            Serial.println(F("Wrong card"));
#endif
        }
        else
        {
#ifdef DEBUG
            Serial.println(F("Admin card"));
#endif
            open_door(false);
        }
    }
    delay(100);
}

String dump_byte_array(byte *buffer, byte bufferSize)
{
    String data = "";
    for (byte i = 0; i < bufferSize; i++)
    {
        data += (String)buffer[i];
    }
    return data;
}

bool is_game_card(String cardId)
{
    for (int i = 0; i < sizeof(gameCards) / sizeof(gameCards[0]); i++)
    {
        if (cardId == gameCards[i])
        {
            return true;
        }
    }
    return false;
}

void open_door(bool active)
{
    if (active)
    {
        digitalWrite(door_push, LOW);
        digitalWrite(door_pull, HIGH);
        delay(1000);
        digitalWrite(door_pull, LOW);
    }
    else
    {
        digitalWrite(door_push, HIGH);
        digitalWrite(door_pull, LOW);
        delay(1000);
        digitalWrite(door_push, LOW);
    }
}
