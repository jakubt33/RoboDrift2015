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
#ifndef RFM69_h
#define RFM69_h

#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define RF69_SPI_CS             SS // SS is the SPI slave select pin, for instance D10 on ATmega328


#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0 // XTAL OFF
#define RF69_MODE_STANDBY       1 // XTAL ON
#define RF69_MODE_SYNTH         2 // PLL ON
#define RF69_MODE_RX            3 // RX MODE
#define RF69_MODE_TX            4 // TX MODE

// available frequency bands
#define RF69_315MHZ            31 // non trivial values to avoid misconfiguration
#define RF69_433MHZ            43
#define RF69_868MHZ            86
#define RF69_915MHZ            91

#define KEY         "Robo_Drift_2015"
#define MASTER      50
#define NETWORKID   100
#define GATEWAYID   1
#define FREQUENCY   RF69_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SENSOR1 	51 //non trivial numbers
#define SENSOR2 	52
#define SENSOR3 	53
#define SENSOR4 	54
#define SENSOR5 	55
#define isRFM69HW	true
#define isMASTER	true



#define false				  0
#define true				  1
#define null                  0
#define COURSE_TEMP_COEF    -90 // puts the temperature reading in the ballpark, user can fine tune the returned value
#define RF69_BROADCAST_ADDR 255
#define RF69_CSMA_LIMIT_MS 1000
#define RF69_TX_LIMIT_MS   1000
//#define RF69_FSTEP  61.03515625 // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)

volatile uint8_t PingTARGET;
volatile uint8_t myAddress;
volatile uint8_t dataReceived;
volatile uint8_t mode; // should be protected?
volatile uint8_t slaveSelectPin;
volatile uint8_t interruptPin;
volatile uint8_t interruptNum;
volatile uint8_t promiscuousMode;
volatile uint8_t powerLevel;
volatile unsigned char DATA[RF69_MAX_DATA_LEN]; // recv/xmit buf, including header & crc bytes
volatile uint8_t DATALEN;
volatile uint8_t SENDERID;
volatile uint8_t TARGETID; // should match _address
volatile uint8_t PAYLOADLEN;
volatile uint8_t ACK_REQUESTED;
volatile uint8_t ACK_RECEIVED; // should be polled immediately after sending a packet with ACK request
//volatile int16_t RSSI; // most accurate RSSI during reception (closest to the reception)


void setAddress(uint8_t addr);
void setNetwork(uint8_t networkID);
uint8_t canSend();
void send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK);
uint8_t sendWithRetry(uint8_t toAddress, char buffer, uint8_t bufferSize, uint8_t retries); // 40ms roundtrip req for 61byte packets
uint8_t receiveDone(uint8_t fromNodeID);
uint8_t ACKReceived(uint8_t fromNodeID);
//uint8_t ACKRequested();
//void sendACK(const void* buffer, uint8_t bufferSize);
//uint32_t getFrequency();
//void setFrequency(uint32_t freqHz);
void encrypt(const char* key);
//void setCS(uint8_t newSPISlaveSelect);
//int16_t readRSSI(/*uint8_t forceTrigger*/);
void promiscuous(uint8_t onOff);
void setHighPower(uint8_t onOFF); // has to be called after initialize() for RFM69HW
void setPowerLevel(uint8_t level); // reduce/increase transmit power level
void sleep();
//uint8_t readTemperature(uint8_t calFactor); // get CMOS temperature (8bit)
//void rcCalibration(); // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]

// allow hacking registers by making these public
uint8_t readReg(uint8_t addr);
void writeReg(uint8_t addr, uint8_t val);
void readAllRegs();


//static void isr0();
//void virtual interruptHandler();
void sendFrame(uint8_t toAddress, char buffer, uint8_t bufferSize, uint8_t requestACK, uint8_t sendACK);
uint8_t init_RFM69();
void receiveBegin();
void setMode(uint8_t mode);
void setHighPowerRegs(uint8_t onOff);
void select();
void unselect();

#endif
