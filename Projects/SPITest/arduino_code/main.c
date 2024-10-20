#include <SPI.h>

bool cnt = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, INPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
}

void loop(void)
{
  if (cnt != 0) {
    Serial.println("All data recieved");
    cnt = 0;
  }
  
}

ISR (SPI_STC_vect)
{
  Serial.println("New byte recieved");
  Serial.println(SPDR);
  cnt = 1;
}
