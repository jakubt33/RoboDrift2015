// **********************************************************************************
// Driver definition for HopeRF RFM69W/RFM69HW/RFM69CW/RFM69HCW, Semtech SX1231/1231H
// **********************************************************************************
// Copyright Felix Rusu (2014), felix@lowpowerlab.com
// http://lowpowerlab.com/
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// You should have received a copy of the GNU General    
// Public License along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "communication.h"
#include "RFM69registers.h"
#include "IO.h"
#include "main.h"
#include "RFM69.h"

uint8_t init_RFM69(){
	const uint8_t CONFIG[][2] =
	  {
	    /* 0x01 */ { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
	    /* 0x02 */ { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
	    /* 0x03 */ { REG_BITRATEMSB, RF_BITRATEMSB_4800}, // default: 4.8 KBPS
	    /* 0x04 */ { REG_BITRATELSB, RF_BITRATELSB_4800},
	    /* 0x05 */ { REG_FDEVMSB, RF_FDEVMSB_5000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
	    /* 0x06 */ { REG_FDEVLSB, RF_FDEVLSB_5000},

	    /* 0x07 */ { REG_FRFMSB, (uint8_t) (RF_FRFMSB_433) },
	    /* 0x08 */ { REG_FRFMID, (uint8_t) (RF_FRFMID_433) },
	    /* 0x09 */ { REG_FRFLSB, (uint8_t) (RF_FRFLSB_433) },

	    // looks like PA1 and PA2 are not implemented on RFM69W, hence the max output power is 13dBm
	    // +17dBm and +20dBm are possible on RFM69HW
	    // +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
	    // +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
	    // +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high power PA settings (section 3.3.7 in datasheet)
	    ///* 0x11 */ { REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},
	    ///* 0x13 */ { REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)

	    // RXBW defaults are { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5} (RxBw: 10.4KHz)
	    /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
	    //for BR-19200: /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3 },
	    /* 0x25 */ { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
	    /* 0x26 */ { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
	    /* 0x28 */ { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
	    /* 0x29 */ { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
	    /* 0x2D */ { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE }, // default 3 preamble bytes 0xAAAAAA
	    /* 0x2E */ { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
	    /* 0x2F */ { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
	    /* 0x30 */ { REG_SYNCVALUE2, NETWORKID }, // NETWORK ID
	    /* 0x37 */ { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_FIXED | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_OFF | RF_PACKET1_ADRSFILTERING_OFF },
	    /* 0x38 */ { REG_PAYLOADLENGTH, 4 }, // in variable length mode: the max frame size, not used in TX
	    ///* 0x39 */ { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
	    /* 0x3C */ { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
	    /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
	    //for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
	    /* 0x6F */ { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
	    {255, 0}
	  };

	do writeReg(REG_SYNCVALUE1, 0xAA); while (readReg(REG_SYNCVALUE1) != 0xAA);
	do writeReg(REG_SYNCVALUE1, 0x55); while (readReg(REG_SYNCVALUE1) != 0x55);

	  for (uint8_t i = 0; CONFIG[i][0] != 255; i++)
	    writeReg(CONFIG[i][0], CONFIG[i][1]);

	  // Encryption is persistent between resets and can trip you up during debugging.
	  // Disable it during initialization so we always start from a known state.
	  encrypt(0);

	  setHighPower(isRFM69HW); // called regardless if it's a RFM69W or RFM69HW
	  setMode(RF69_MODE_STANDBY);
	  while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady

	  encrypt(KEY);
	  promiscuous(true);
	  return true;
}

void sendFrame(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK, uint8_t sendACK){
	 setMode(RF69_MODE_STANDBY); // turn off receiver to prevent reception while filling fifo
	  while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady

	  writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
	  if (bufferSize > RF69_MAX_DATA_LEN) bufferSize = RF69_MAX_DATA_LEN;

	  // control byte
	  uint8_t CTLbyte = 0x00;
	  if (sendACK)
	    CTLbyte = 0x80;
	  else if (requestACK)
	    CTLbyte = 0x40;

	  // write to FIFO
	  select();
	  spi_send(REG_FIFO | 0x80);
	  spi_send(bufferSize + 3);
	  spi_send(toAddress);
	  spi_send(NODEID);
	  spi_send(CTLbyte);

	  for (uint8_t i = 0; i < bufferSize; i++)
		  spi_send(((uint8_t*) buffer)[i]);
	  unselect();

	  // no need to wait for transmit mode to be ready since its handled by the radio
	  setMode(RF69_MODE_TX);
	  //while (digitalRead(_interruptPin) == 0); // wait for DIO0 to turn HIGH signalling transmission finish
	  while ( (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) == 0x00); // wait for ModeReady
	  setMode(RF69_MODE_STANDBY);
}

void setMode(uint8_t newMode)
{
  if (newMode == mode)
    return;

  cli();
  switch (newMode) {
    case RF69_MODE_TX:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
      if (isRFM69HW) setHighPowerRegs(true);
      break;
    case RF69_MODE_RX:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
      if (isRFM69HW) setHighPowerRegs(false);
      break;
    case RF69_MODE_SYNTH:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
      break;
    case RF69_MODE_STANDBY:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
      break;
    case RF69_MODE_SLEEP:
      writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
      break;
    default:
    	sei();
        return;
  }

  // we are using packet mode, so this check is not really needed
  // but waiting for mode ready is necessary when going from sleep because the FIFO may not be immediately available from previous mode
  while (mode == RF69_MODE_SLEEP && (readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady

  mode = newMode;
  sei();
}

uint8_t readReg(uint8_t addr)
{
  select();
  spi_send(addr & 0x7F);
  uint8_t regval = spi_send(0);
  unselect();
  return regval;
}

void writeReg(uint8_t addr, uint8_t value)
{
  select();
  spi_send( addr | 0x80 ); //0x80 = 0b1000 0000, 1.bit=1->write access, 1.bit=0 -> read access
  spi_send(value);
  unselect();
}

// select the transceiver
void select() {
	cli();
	PORT_SPI &= ~(1<<SS);
}

// UNselect the transceiver chip
void unselect() {
	PORT_SPI |= (1<<SS);
    sei();
}

void setHighPowerRegs(uint8_t onOff) {
  writeReg(REG_TESTPA1, onOff ? 0x5D : 0x55);
  writeReg(REG_TESTPA2, onOff ? 0x7C : 0x70);
}

// To enable encryption: radio.encrypt("ABCDEFGHIJKLMNOP");
// To disable encryption: radio.encrypt(null) or radio.encrypt(0)
// KEY HAS TO BE 16 bytes !!!
void encrypt(const char* key) {
  setMode(RF69_MODE_STANDBY);
  if (key != 0)
  {
    select();
    spi_send(REG_AESKEY1 | 0x80);
    for (uint8_t i = 0; i < 16; i++)
      spi_send(key[i]);
    unselect();
  }
  writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFE) | (key ? 1 : 0));
}

// ON  = disable filtering to capture all frames on network
// OFF = enable node/broadcast filtering to capture only frames sent to this/broadcast address
void promiscuous(uint8_t onOff) {
  promiscuousMode = onOff;
  writeReg(REG_PACKETCONFIG1, (readReg(REG_PACKETCONFIG1) & 0xF9) | (onOff ? RF_PACKET1_ADRSFILTERING_OFF : RF_PACKET1_ADRSFILTERING_NODEBROADCAST));
}

void setHighPower(uint8_t onOff) {
  writeReg(REG_OCP, (isRFM69HW&onOff) ? RF_OCP_OFF : RF_OCP_ON);
  if (isRFM69HW) // turning ON
    writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON); // enable P1 & P2 amplifier stages
  else
    writeReg(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | powerLevel); // enable P0 only
}

/*void sleep() {
  setMode(RF69_MODE_SLEEP);
}

void setAddress(uint8_t addr)
{
  address = addr;
  writeReg(REG_NODEADRS, address);
}

void setNetwork(uint8_t networkID)
{
  writeReg(REG_SYNCVALUE2, networkID);
}*/

/*
// set output power: 0 = min, 31 = max
// this results in a "weaker" transmitted signal, and directly results in a lower RSSI at the receiver
void setPowerLevel(uint8_t powerLevel2)
{
  powerLevel = powerLevel2;
  writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0xE0) | (powerLevel > 31 ? 31 : powerLevel));
}*/

/*
uint8_t canSend(){
  if (mode == RF69_MODE_RX && PAYLOADLEN == 0*/ /*&& readRSSI() < CSMA_LIMIT*/ /*) // if signal stronger than -100dBm is detected assume channel activity
  {
    setMode(RF69_MODE_STANDBY);
    return true;
  }
  return false;
}*/

/*
void send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK)
{
  writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
  //													uint32_t now = millis();
  while (!canSend()*/ /*&& millis() - now < RF69_CSMA_LIMIT_MS*/ /*) receiveDone();
  sendFrame(toAddress, buffer, bufferSize, requestACK, false);
} */

// to increase the chance of getting a packet across, call this function instead of send
// and it handles all the ACK requesting/retrying for you :)
// The only twist is that you have to manually listen to ACK requests on the other side and send back the ACKs
// The reason for the semi-automaton is that the lib is interrupt driven and
// requires user action to read the received data and decide what to do with it
// replies usually take only 5..8ms at 50kbps@915MHz

/*
uint8_t sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime) {
  uint32_t sentTime;
  for (uint8_t i = 0; i <= retries; i++)
  {
    send(toAddress, buffer, bufferSize, true);
    sentTime = millis();
    while (millis() - sentTime < retryWaitTime)
    {
      if (ACKReceived(toAddress))
      {
        //Serial.print(" ~ms:"); Serial.print(millis() - sentTime);
        return true;
      }
    }
    //Serial.print(" RETRY#"); Serial.println(i + 1);
  }
  return false;
}
*/

/*
// should be polled immediately after sending a packet with ACK request
uint8_t ACKReceived(uint8_t fromNodeID) {
  if (receiveDone())
    return (SENDERID == fromNodeID || fromNodeID == RF69_BROADCAST_ADDR) && ACK_RECEIVED;
  return false;
}*/


/*
// check whether an ACK was requested in the last received packet (non-broadcasted packet)
uint8_t ACKRequested() {
  return ACK_REQUESTED && (TARGETID != RF69_BROADCAST_ADDR);
}*/


/*
// should be called immediately after reception in case sender wants ACK
void sendACK(const void* buffer, uint8_t bufferSize) {
  uint8_t sender = SENDERID;
  //int16_t _RSSI = RSSI; // save payload received RSSI value
  writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
  	  	  	  	  	  //uint32_t now = millis();
  while (!canSend() *//* && millis() - now < RF69_CSMA_LIMIT_MS*/  /*) receiveDone();
  sendFrame(sender, buffer, bufferSize, false, true);
  //RSSI = _RSSI; // restore payload RSSI
}*/

/*void interruptHandler() {
  //pinMode(4, OUTPUT);
  //digitalWrite(4, 1);
  if (mode == RF69_MODE_RX && (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY))
  {
    //RSSI = readRSSI();
    setMode(RF69_MODE_STANDBY);
    select();
    spi_send(REG_FIFO & 0x7F);
    PAYLOADLEN = spi_send(0);
    PAYLOADLEN = PAYLOADLEN > 66 ? 66 : PAYLOADLEN; // precaution
    TARGETID = spi_send(0);
    if(!(promiscuousMode || TARGETID == address || TARGETID == RF69_BROADCAST_ADDR) // match this node's address, or broadcast address or anything in promiscuous mode
       || PAYLOADLEN < 3) // address situation could receive packets that are malformed and don't fit this libraries extra fields
    {
      PAYLOADLEN = 0;
      unselect();
      receiveBegin();
      //digitalWrite(4, 0);
      return;
    }

    DATALEN = PAYLOADLEN - 3;
    SENDERID = spi_send(0);
    uint8_t CTLbyte = spi_send(0);

    ACK_RECEIVED = CTLbyte & 0x80; // extract ACK-received flag
    ACK_REQUESTED = CTLbyte & 0x40; // extract ACK-requested flag

    for (uint8_t i = 0; i < DATALEN; i++)
    {
      DATA[i] = spi_send(0);
    }
    if (DATALEN < RF69_MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string
    unselect();
    setMode(RF69_MODE_RX);
  }
  RSSI = readRSSI();
  //digitalWrite(4, 0);
}*/

//void isr0() { selfPointer->interruptHandler(); }

void receiveBegin() {
  DATALEN = 0;
  SENDERID = 0;
  TARGETID = 0;
  PAYLOADLEN = 0;
  ACK_REQUESTED = 0;
  ACK_RECEIVED = 0;
  RSSI = 0;
  if (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY)
    writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
  writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01); // set DIO0 to "PAYLOADREADY" in receive mode
  setMode(RF69_MODE_RX);
}

uint8_t receiveDone() {
//ATOMIC_BLOCK(ATOMIC_FORCEON)
//{
  cli();//noInterrupts(); // re-enabled in unselect() via setMode() or via receiveBegin()
  if (mode == RF69_MODE_RX && PAYLOADLEN > 0)
  {
    setMode(RF69_MODE_STANDBY); // enables interrupts
    return true;
  }
  else if (mode == RF69_MODE_RX) // already in RX no payload yet
  {
    sei();//interrupts(); // explicitly re-enable interrupts
    return false;
  }
  receiveBegin();
  return false;
//}
}


/*
int16_t readRSSI(*/ /*uint8_t forceTrigger*/ /*) {
  int16_t rssi = 0;
  if (0 */ /*forceTrigger*/  /*)
  {
    // RSSI trigger not needed if DAGC is in continuous mode
    writeReg(REG_RSSICONFIG, RF_RSSI_START);
    while ((readReg(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00); // wait for RSSI_Ready
  }
  rssi = -readReg(REG_RSSIVALUE);
  rssi >>= 1;
  return rssi;
}*/
