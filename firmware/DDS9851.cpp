/***************************************************
  DDS9851

  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "DDS9851.h"

#ifdef SPARK  //Only defined if a Particle device
  #include "application.h"
  //#include "fast_pin.h"
  //STM32_Pin_Info* PIN_MAP = HAL_Pin_Map(); // Pointer required for highest access speed
#if (PLATFORM_ID == 0)  // Core

#elif (PLATFORM_ID == 6) // Photon
#else
  #error "*** PLATFORM_ID not supported by this library. PLATFORM should be Core or Photon ***"
#endif
#endif

WsprTx::WsprTx(DDS9851 *dds, DdsUIWidget *ddsWidget) {
		_dds = dds;
		_ddsWidget = ddsWidget;
}
void WsprTx::sendBeacon(uint8_t band) {
	  switch(band) {
		  case BUTTON_BAND_160: wsprTransmitFrequency = WSPR_TX_160M; break;
          case BUTTON_BAND_80:  wsprTransmitFrequency = WSPR_TX_80M; break;
          case BUTTON_BAND_60:  wsprTransmitFrequency = WSPR_TX_60M; break;
          case BUTTON_BAND_40:  wsprTransmitFrequency = WSPR_TX_40M; break;
          case BUTTON_BAND_30:  wsprTransmitFrequency = WSPR_TX_30M; break;
          case BUTTON_BAND_20:  wsprTransmitFrequency = WSPR_TX_20M; break;
          case BUTTON_BAND_17:  wsprTransmitFrequency = WSPR_TX_17M; break;
          case BUTTON_BAND_15:  wsprTransmitFrequency = WSPR_TX_15M; break;
          case BUTTON_BAND_12:  wsprTransmitFrequency = WSPR_TX_12M; break;
          case BUTTON_BAND_10:  wsprTransmitFrequency = WSPR_TX_10M; break;
          case BUTTON_BAND_6:   wsprTransmitFrequency = WSPR_TX_6M; break;
		  default: wsprTransmitFrequency = (WSPR_TX_40M+DDS_OSET + random(10, 190)); break;
	  }
	  wsprTransmitFrequency += DDS_OSET + random(10, 190);
	  for (int i=0; i<162; i++) {
		_dds->sendFrequency(wsprTransmitFrequency + WSPR_DATA[i] * 2, DDS_ENABLE);
		_ddsWidget->setFrequency(wsprTransmitFrequency + WSPR_DATA[i] * 2);
		_ddsWidget->updateDisplay();
        delay(682);
      }
	  _dds->sendFrequency((wsprTransmitFrequency), DDS_DISABLE);
}

DDS9851::DDS9851(uint8_t load, uint8_t clock, uint8_t data) {
    _load = load;
	_clock = clock;
	_data = data;
}

void DDS9851::begin() {
    pinMode(_load, OUTPUT);
	digitalWrite(_load, LOW);
	pinMode(_clock, OUTPUT);
	digitalWrite(_clock, LOW);
	pinMode(_data, OUTPUT);
	digitalWrite(_data, LOW);
	delay(1);
	pulseHigh(_clock);
	digitalWrite(_load, HIGH);
	delay(2);
	for (int b=0; b<4; b++) {
      tfr_byte(0);
    }
	tfr_byte(0x01);
	pulseHigh(_load);
	sendFrequency(7000000, DDS_DISABLE);
}
void DDS9851::tfr_byte(byte data) {
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(_data, data & 0x01);
    pulseHigh(_clock);   //after each bit sent, CLK is pulsed high
  }
}
void DDS9851::sendFrequency(long frequencyHz, uint8_t enable) {
	/*
	10MHz example:
	            Power-Down  b[2] 1=power-down, 0=power-up
	       ppppp  6x-REFCLK b[0] 1=enabled
		   43210| |
	W0  =  00001001   //phase_word(5b), control_bits(3b)
    W1  =  00001110
    W2  =  00111000
    W3  =  11100011
    W4  =  10001110
    tuning_word=0x0E38E38E=238609294
	phase_word 5b in MSB of W0, 11.25degree steps
    2^32 = 4294967296
    FoutMHz=(tuning_word)*SysClockMHz/4294967296
    tuning_word=foutMHz*4294967296/SysClockMHz
	           =foutHz*4294.967295/SysClockMHz
    */
  int32_t tuning_word = 6.0 * (frequencyHz * 4294.967296)/180.0;  // note 125 MHz clock on 9850, 180MHz on 9851
  for (int b=0; b<4; b++, tuning_word>>=8) {
    tfr_byte(tuning_word & 0xFF);
  }
  uint16_t control_word = 0x04;
  if(enable==DDS_ENABLE)
    control_word &= 0xFB;
  tfr_byte(control_word);   // Final control byte, all 0 for 9850 chip
  pulseHigh(_load);  // Done!  Should see output
}
