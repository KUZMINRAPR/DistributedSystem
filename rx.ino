#include "nRF24L01.h"
#include "RF24.h"
#define CERadioPin 9
#define CSMRadioPin 10
#define SensorsCount 3
#define FreeChannel 0x60

RF24 radio(CERadioPin, CSMRadioPin);  

byte address[][6] = {"1Node", "2Node"};
int data[SensorsCount];

void set_rx(uint8_t ChannelNum)
{
  radio.begin();              
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x60);     // выбираем канал (в котором нет шумов)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль
}

void setup() {
  Serial.begin(9600);
  Serial.println("-------------------------------");
  Serial.println("Температура|Влажность|Освещение");
  set_rx(FreeChannel);
}

void print(int data[3])
{
  Serial.print(data[0]);
  Serial.print("         |");
  Serial.print(data[1]);
  Serial.print("      |");
  Serial.print(data[2]);
  Serial.println("      ");
}

void loop() {
  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    print(data);
  }

}
