#include <GyverNTC.h>
#include "RF24.h"
#include "nRF24L01.h"

#define PhotoPin A0
#define MoisurePin A1
#define TemperaturePin A2
#define CERadioPin 9
#define CSMRadioPin 10
#define SensorsCount 3
#define FreeChannel 0x60

RF24 radio(CERadioPin, CSMRadioPin);

byte address[][6] = {"1Node", "2Node"};
int data[SensorsCount];

int get_temperature(uint8_t TemperaturePin)
{
  GyverNTC term(TemperaturePin, 10000, 3950);
  return term.getTemp();
}

int get_moisure(uint8_t MoisurePin)
{
  return analogRead(MoisurePin);
}

int get_illumination(uint8_t PhotoPin)
{
  return analogRead(PhotoPin);
}

void set_tx(uint8_t ChannelNum)
{
  radio.begin();
  radio.setAutoAck(1); // Режим подтверждения приема, 1 вкл, 0 выкл
  radio.setRetries(0, 15); // (Время между попыткой достучаться, число попыток)
  radio.enableAckPayload(); // Разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32); // размер пакета в байтах

  radio.openWritingPipe(address[0]); // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(ChannelNum); // выбираем канал (в котором нет шумов)

  radio.setPALevel(RF24_PA_MAX); // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(RF24_250KBPS); // Скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // должна быть одинакова на приемнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность

  radio.powerUp();
  radio.stopListening(); // не слушаем эфир, мы передатчик
}

void print(int data[3])
{
  Serial.print("Температура: ");
  Serial.println(data[0]);
  Serial.print("Влажность: ");
  Serial.println(data[1]);
  Serial.print("Освещение: ");
  Serial.println(data[2]);
}

void setup() {
  Serial.begin(9600);
  set_tx(FreeChannel);
}

void loop() {
  data[0] = get_temperature(TemperaturePin);
  data[1] = get_moisure(MoisurePin);
  data[2] = get_illumination(PhotoPin);
  print(data);
  radio.write(&data, sizeof(data));
  delay(500);
}
