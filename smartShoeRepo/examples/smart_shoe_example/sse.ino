#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "SimpleVR.h"
#include <TinyGPS++.h>
#include <Time.h>
#include "talkie.h"

Talkie talk;
/**
   Connection
   Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/

const uint8_t spHENRY[]         PROGMEM = {0x08, 0xC8, 0x4A, 0x8C, 0x03, 0x1A, 0x68, 0x49, 0x0B, 0xAC, 0xE5, 0x11, 0xFA, 0x14, 0xCD, 0x35, 0x59, 0xC4, 0xE3, 0x5B, 0xEC, 0xBC, 0xA5, 0xD5, 0x88, 0x96, 0x99, 0xBD, 0x9E, 0x95, 0x3C, 0x1B, 0xB3, 0x64, 0x69, 0x1A, 0xEB, 0xD2, 0xA7, 0xA9, 0x1C, 0xE6, 0xD1, 0xDB, 0x98, 0x07, 0xA7, 0x5A, 0xAA, 0x5F, 0x53, 0x4D, 0xAA, 0x61, 0x9E, 0x7D, 0xAC, 0xDD, 0x8E, 0x48, 0xC8, 0x9E, 0xB1, 0x77, 0x5B, 0x44, 0x95, 0xAB, 0xEB, 0x15, 0xAE, 0x1E, 0x0D, 0x2D, 0xF3, 0x4D, 0x7C, 0xFC, 0xF3, 0xFF};
VR myVR(4, 5);   // 2:RX 3:TX, you can choose your favourite pins.
SoftwareSerial gpsSerial(9, 10);
TinyGPS gps;
uint8_t buf[64];

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

static void print_date(TinyGPS &gps);

unsigned long age, date, times, chars = 0;


/**
   0: wait name
   1: wait command voice
*/
uint8_t sta;

/** index value of voice */
#define WAKE_WORD        (0x2)
#define TURN_ON_LIGHTS      (0x3)
#define PLEASE_TURN_OFF_LIGHTS (0x6)
#define WHERE_AM_I       (0x4)
#define LIGHT_PATTERN_ONE (0x5)
#define WHAT_IS_YOUR_NAME (0x15)

unsigned long start_time;

int led = 13;


void setup(void)
{
  digitalWrite(led, LOW);

  Serial.begin(115200);
  Serial.print("test");
  gpsSerial.begin(9600);
  myVR.begin(9600);


  /** set led off */

  /** Wait name is called */
  sta = 0;
  start_time = millis();
}

void loop(void)
{
  myVR.begin(9600);
  myVR.listen();
  int ret;
  uint16_t voice = 0;
  ret = myVR.recognize(buf, 50);
  if (ret > 0) {
    voice += buf[0];
    voice <<= 8;
    voice += buf[1];
    switch (voice) {
      case WAKE_WORD:
        if (sta == 0) {
          sta = 1;
          start_time = millis();
          Serial.println(F("Operational wake word called. Waiting action command."));
        }
        break;
      case TURN_ON_LIGHTS:
        if (sta == 1) {
          sta = 0;
          Serial.println(F("Question asked, 'turn on lights'. Turning on led: "));


          digitalWrite(led, HIGH);

          myVR.listen();

        }
        break;
      case WHAT_IS_YOUR_NAME:
        if (sta == 1) {
          sta = 0;
          Serial.println(F("Question asked, 'what is your name'. Saying Name: "));

          delay(1000);

          talk.say(spHENRY);

          myVR.listen();

        }
        break;
      case PLEASE_TURN_OFF_LIGHTS:
        if (sta == 1) {
          sta = 0;
          Serial.println(F("Question asked, 'please turn off lights'. Turning on led: "));


          digitalWrite(led, LOW);

          myVR.listen();

        }
        break;
      case  LIGHT_PATTERN_ONE:
        if (sta == 1) {
          sta = 0;
          Serial.println(F("Question asked, 'Load light pattern one'. Turning on led: "));

          digitalWrite(led, HIGH);
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(led, HIGH);
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(led, HIGH);
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(led, HIGH);
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(led, HIGH);
          delay(1000);
          digitalWrite(led, LOW);



          myVR.listen();
        }

        myVR.listen();
        break;
      case WHERE_AM_I:
        if (sta == 1) {
          sta = 0;
          Serial.println(F("Question asked, 'where am I', Giving coordinates: "));
          delay(900);
          gpsSerial.begin(9600);
          gpsSerial.listen();
          delay(900);

          for (int i = 0; i < 10; i++) {

            gpsSerial.begin(9600);
            gpsSerial.listen();

            float flat, flon;
            delay(50);
            unsigned short sentences = 0, failed = 0;

            gps.f_get_position(&flat, &flon);
            Serial.print("LATITUDE: ");
            print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
            Serial.println(" ");
            Serial.print("LONGITUDE: ");
            print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
            Serial.println(" ");

            Serial.print("altitude: ");
            print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
            Serial.println(" ");
            Serial.print("COURSE:");
            print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
            Serial.println("");
            print_date(gps);


            Serial.print("DIRECTION: ");
            int d;
            print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
            d = gps.f_course();
            Serial.println();
            Serial.println();

            delay(200);
            smartdelay(1000);



          }
        }

        myVR.listen();
        Serial.write("2 cicles completed");


    }

  }
}


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  }
  while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}
static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartdelay(0);
}

static void print_date(TinyGPS & gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
            month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}
