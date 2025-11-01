// DO NOT CHANGE ANY CODE IN THIS FILE
// YOU SHOULD ONLY NEED TO CHANGE THE CODE IN "Arduino-Starter.ino"
// CHANGING THIS FILE MAY BREAK THE BUILD SYSTEM AND/OR THE WIRELESS SYSTEM

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <servo.pio.h>

#define MOTOR1_ENABLE 27
#define MOTOR1_PHASE 26

#define MOTOR2_ENABLE 21
#define MOTOR2_PHASE 20

#define MOTOR3_ENABLE 19
#define MOTOR3_PHASE 18

#define MOTOR4_ENABLE 17
#define MOTOR4_PHASE 16

#define SERVO1_PIN 15
#define SERVO2_PIN 14
#define SERVO3_PIN 13
#define SERVO4_PIN 12

#define RADIO_LED_PIN 28

uint8_t TARGET_ADDRESS[] = {0xF0,0xF0,0xF0,0xF0,0xD2};
uint8_t LOCAL_ADDRESS[] = {0xF0,0xF0,0xF0,0xF0,0xE1};
#define CHANNEL (TEAM_NUMBER * 3) // 0-125
#define RF_POWER_LEVEL RF24_PA_HIGH

RF24 radio(22, 1, 1000000);

#define PACKET_LEFT_X    0
#define PACKET_LEFT_Y    1
#define PACKET_RIGHT_X   2
#define PACKET_RIGHT_Y   3
#define PACKET_BUTTONS_1 4
#define PACKET_BUTTONS_2 5

#define ULTRASONIC_TRIG 4
#define ULTRASONIC_ECHO 5

uint8_t radio_packet[6];
uint8_t radio_led_state = 0;
bool recv_first_packet = false;

bool RR_buttonA() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_1] >> 5) & 1;
}
bool RR_buttonB() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_1] >> 6) & 1;
}
bool RR_buttonX() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_1] >> 4) & 1;
}
bool RR_buttonY() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_1] >> 7) & 1;
}
bool RR_buttonLB() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 0) & 1;
}
bool RR_buttonRB() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 1) & 1;
}
bool RR_buttonBack() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 4) & 1;
}
bool RR_buttonStart() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 5) & 1;
}
bool RR_buttonL3() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 6) & 1;
}
bool RR_buttonR3() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 7) & 1;
}
float RR_axisLX() {
  if(!recv_first_packet) return 0.0;
  float lx = (((float)(radio_packet[0])) / 127.5) - 1.0;
  lx = constrain(lx, -1.0, 1.0);
  if(lx >= -0.05 && lx <= 0.05) return 0.0;
  return lx;
}
float RR_axisLY() {
  if(!recv_first_packet) return 0.0;
  float ly = (((float)(radio_packet[1])) / 127.5) - 1.0;
  ly = constrain(ly, -1.0, 1.0);
  if(ly >= -0.05 && ly <= 0.05) return 0.0;
  return ly;
}
float RR_axisRX() {
  if(!recv_first_packet) return 0.0;
  float rx = (((float)(radio_packet[2])) / 127.5) - 1.0;
  rx = constrain(rx, -1.0, 1.0);
  if(rx >= -0.05 && rx <= 0.05) return 0.0;
  return rx;
}
float RR_axisRY() {
  if(!recv_first_packet) return 0.0;
  float ry = (((float)(radio_packet[3])) / 127.5) - 1.0;
  ry = constrain(ry, -1.0, 1.0);
  if(ry >= -0.05 && ry <= 0.05) return 0.0;
  return ry;
}
bool RR_buttonLT() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 2) & 1;
}
bool RR_buttonRT() {
  if(!recv_first_packet) return 0;
  return (radio_packet[PACKET_BUTTONS_2] >> 3) & 1;
}
/*float axisLT() {
  float lt = (((float)(radio_packet[4])) - 127.5) / 127.5;
  lt = constrain(lt, 0.0, 1.0);
  if(lt <= 0.05) return 0.0;
  return lt;
}
float axisRT() {
  float rt = (127.5 - ((float)(radio_packet[4]))) / 127.5;
  rt = constrain(rt, 0.0, 1.0);
  if(rt <= 0.05) return 0.0;
  return rt;
}*/
int RR_dpad() {
  if(!recv_first_packet) return 8;
  return radio_packet[PACKET_BUTTONS_1] & 0xF;
}

void setup1() {
  radio_packet[0] = 127;
  radio_packet[1] = 127;
  radio_packet[2] = 127;
  radio_packet[3] = 127;
  radio_packet[4] = 0;
  radio_packet[5] = 0;
  
  pinMode(MOTOR1_ENABLE, OUTPUT);
  pinMode(MOTOR1_PHASE, OUTPUT);
  pinMode(MOTOR2_ENABLE, OUTPUT);
  pinMode(MOTOR2_PHASE, OUTPUT);

  pinMode(MOTOR3_ENABLE, OUTPUT);
  pinMode(MOTOR3_PHASE, OUTPUT);
  pinMode(MOTOR4_ENABLE, OUTPUT);
  pinMode(MOTOR4_PHASE, OUTPUT);

  pinMode(RADIO_LED_PIN, OUTPUT);
  
  SPI.setTX(3);
  SPI.setRX(0);
  SPI.setSCK(2);
  SPI.setCS(1);
  SPI.begin();
  
  //Serial.begin(115200);
  
  if(!radio.begin(&SPI)) {
    delay(1000);
    Serial.println("Radio not responding");
  } else {
    //Serial.println("Radio connected");
  }
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(CHANNEL);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPALevel(RF_POWER_LEVEL);
  radio.openWritingPipe(TARGET_ADDRESS);
  radio.openReadingPipe(1,LOCAL_ADDRESS);
  radio.startListening();
}

bool isPacketValid(uint8_t *pkt) {
  if(pkt[0] != pkt[10]) return false;
  if(pkt[1] != pkt[11]) return false;
  if(pkt[2] != pkt[12]) return false;
  if(pkt[3] != pkt[13]) return false;
  
  uint8_t sum = 0;
  for(int i = 0; i < 14; i++) {
    sum += pkt[i];
  }
  if(sum != pkt[14]) return false;
  
  return true;
}

void loop1() {
  delayMicroseconds(1000);
  if(radio.available()) {
    uint8_t buf[20];
    radio.read(buf, sizeof(buf));
    //Serial.println("Recv radio packet");

    if(isPacketValid(buf+1)) {
      memcpy(radio_packet, buf+5, 6);
      recv_first_packet = true;
      radio_led_state = (radio_led_state + 1) % 16;
      digitalWrite(RADIO_LED_PIN, radio_led_state == 0);
      //Serial.print("Valid ");
      //Serial.println(radio_led_state);
    } else if(isPacketValid(buf+2)) {
      memcpy(radio_packet, buf+6, 6);
      recv_first_packet = true;
      radio_led_state = (radio_led_state + 1) % 16;
      digitalWrite(RADIO_LED_PIN, radio_led_state);
      //Serial.print("Valid ");
      //Serial.println(radio_led_state);
    } else {
      // Invalid
      /*Serial.print("Invalid ");
      for(int i = 0; i < 20; i++) {
        Serial.print(buf[i]);
        Serial.print(" ");
      }
      Serial.println();*/
    }
  }
}

void RR_setMotor1(float speed) {
  speed = constrain(speed, -1.0, 1.0);
  analogWrite(MOTOR1_ENABLE, int(255 * abs(speed)));
  digitalWrite(MOTOR1_PHASE, speed > 0.0);
}

void RR_setMotor2(float speed) {
  speed = constrain(speed, -1.0, 1.0);
  analogWrite(MOTOR2_ENABLE, int(255 * abs(speed)));
  digitalWrite(MOTOR2_PHASE, speed > 0.0);
}

void RR_setMotor3(float speed) {
  speed = constrain(speed, -1.0, 1.0);
  analogWrite(MOTOR3_ENABLE, int(255 * abs(speed)));
  digitalWrite(MOTOR3_PHASE, speed > 0.0);
}

void RR_setMotor4(float speed) {
  speed = constrain(speed, -1.0, 1.0);
  analogWrite(MOTOR4_ENABLE, int(255 * abs(speed)));
  digitalWrite(MOTOR4_PHASE, speed > 0.0);
}

void RR_drive(float l, float r) {
  RR_setMotor1(l);
  RR_setMotor2(r);
}

Servo servo1, servo2, servo3, servo4;

void init() {
  static bool is_init = false;
  if (is_init) {
    return;
  }

  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);

  servo1.attach(SERVO1_PIN, 600, 2400);
  servo2.attach(SERVO2_PIN, 600, 2400);
  servo3.attach(SERVO3_PIN, 600, 2400);
  servo4.attach(SERVO4_PIN, 600, 2400);

  is_init = true;
}

void RR_setServo1(int angle) {
  init();
  angle = constrain(angle, 0, 180);
  servo1.write(angle);
}

void RR_setServo2(int angle) {
  init();
  angle = constrain(angle, 0, 180);
  servo2.write(angle);
}

void RR_setServo3(int angle) {
  init();
  angle = constrain(angle, 0, 180);
  servo3.write(angle);
}

void RR_setServo4(int angle) {
  init();
  angle = constrain(angle, 0, 180);
  servo4.write(angle);
}

float RR_getUltrasonic() {
  init();


  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 20000);

  if (duration >= 20000) {
    return 0.0;
  }

  return duration * 0.034 / 2.0;
}


const int LINE_SENSOR_PINS[] = { 6, 7, 8, 9, 10, 11 };

//int SENSOR_READING[] = { 0, 0, 0, 0, 0, 0 };

void RR_getLineSensors(int sensors[6]) {
  for (int i = 0; i < 6; ++i) {
    pinMode(LINE_SENSOR_PINS[i], OUTPUT);
    digitalWrite(LINE_SENSOR_PINS[i], HIGH);
    delay(1);
    pinMode(LINE_SENSOR_PINS[i], INPUT);
    unsigned long long start = micros();
    while (digitalRead(LINE_SENSOR_PINS[i]) && micros() - start < 10000) {}
    unsigned long long end = micros();

    int diff = end - start;

    sensors[i] = diff;
  }
}

// vim: tabstop=2 shiftwidth=2 expandtab
