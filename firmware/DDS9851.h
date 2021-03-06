/***************************************************
  DDS9851
  
  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _DDS9851_H
#define _DDS9851_H

#ifdef ARDUINO
#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "application.h"
#endif
#include <avr/pgmspace.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
#endif

#ifdef SPARK
#include "application.h"
#define digitalPinToBitMask(pin) (volatile uint32_t)pin
#define portOutputRegister(port) NULL
#define digitalPinToPort(pin) (volatile uint32_t)pin
#define SPARK_SPI_SPEED SPI_CLOCK_DIV4
#endif

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
 
#define DDS_ENABLE  1
#define DDS_DISABLE 0
class DDS9851 {

 public:
  DDS9851(uint8_t load, uint8_t clock, uint8_t data);
  //void setFrequencyHz(uint freqHz); 
  void begin();
  void tfr_byte(byte data);
  void sendFrequency(long frequencyHz, uint8_t enable);
 private:
  uint8_t _load;
  uint8_t _clock;
  uint8_t _data; 
};
// DDS Offset in Hz
#define DDS_OSET  0  //DDS #2

// WSPR Output Frequency
//#define WSPR_TX_A    10.140100e6  // this is the bottom of the band. The station moves about.
#define WSPR_TX_160M 1.838000e6
#define WSPR_TX_80M  3.594000e6
#define WSPR_TX_60M  5.288600e6
#define WSPR_TX_40M  7.040000e6  // this is the bottom of the band. The station moves about.
#define WSPR_TX_30M  10.140100e6
#define WSPR_TX_20M  14.097000e6
#define WSPR_TX_17M  18.106000e6
#define WSPR_TX_15M  21.096000e6
#define WSPR_TX_12M  28.126000e6
#define WSPR_TX_10M  7.040000e6
#define WSPR_TX_6M  50.294400e6

#define BUTTON_BAND_160		0
#define BUTTON_BAND_80		1
#define BUTTON_BAND_60		2
#define BUTTON_BAND_40		3
#define BUTTON_BAND_30		4
#define BUTTON_BAND_20		5
#define BUTTON_BAND_17		6
#define BUTTON_BAND_15		7
#define BUTTON_BAND_12		8
#define BUTTON_BAND_10		9
#define BUTTON_BAND_6		10

#define WSPR_DUTY  3 // transmit every N slices.

class WsprTx {
  public:
    WsprTx(DDS9851 *dds);
    void sendBeacon(uint8_t band);
  private:
    DDS9851 *_dds;
    long wsprTransmitFrequency;
    const byte WSPR_DATA[162] = {3,1,0,0,2,0,2,2,1,2,2,2,3,3,1,0,2,2,3,0,0,3,2,1,1,3,3,2,0,0,0,2,0,0,3,2,0,1,0,3,0,0,2,2,2,2,1,0,1,1,2,2,1,1,0,3,2,2,2,3,3,2,1,0,2,0,2,3,1,0,3,0,3,2,1,2,1,0,0,1,0,0,3,0,1,3,2,0,2,1,1,2,1,2,1,2,0,2,3,2,2,0,2,2,1,2,0,3,2,0,1,3,3,2,3,1,0,2,3,3,0,1,0,0,2,1,3,1,2,0,2,2,0,3,0,1,0,0,3,1,0,0,2,2,0,2,0,3,1,0,3,0,3,3,2,0,0,1,1,2,0,0,};
};
#endif
