/** 
	*  @file           radio_drive.cpp
	*  @author         De Giovanni Marco <br/>
	*                  (mail: mar.degiovanni@stud.uniroma3.it)
	*  @date           2018
	*  @brief          Implementation to nRF24L01 radio module
*/

#include <SPI.h>
#include "radio_drive.h"

#define WRITE_REG       0x20  // Define write command to register
#define STATUS          0x07  // 'Status' register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define CONFIG          0x00  // 'Config' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define RF_CH           0x05  // 'RF channel' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define MAX_RT   		0x10  // Maximum number of retransmits interrupt
#define TX_DS    		0x20  // Data Sent TX FIFO interrupt.

#define TX_ADR_WIDTH    5	//Address length

unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = {0x34, 0x43, 0x10, 0x10, 0x01}; // Static TX address

/**************************************************
 * Function: spiRead();
 * 
 * Description:
 * Read one unsigned char from nRF24L01 register, 'reg'
/**************************************************/
static unsigned char spiRead(unsigned char reg) {
	unsigned char reg_val;

	digitalWrite(CSN, 0);                // CSN low, initialize SPI communication...
	SPI.transfer(reg);                         // Select register to read from..
	reg_val = SPI.transfer(0);                 // ..then read register value
	digitalWrite(CSN, 1);                // CSN high, terminate SPI communication

	return(reg_val);                     // return register value
}

/**************************************************
 * Function: spiRead();
 * 
 * Description:
 * Reads 'unsigned chars' #of unsigned chars from register 'reg'
/**************************************************/
static unsigned char spiRead(unsigned char reg, char *pBuf, uint16_t length) {
	unsigned char sstatus,i;

	digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
	sstatus = SPI.transfer(reg);       	    // Select register to write to and read status unsigned char

	for(i=0;i<length;i++)
		pBuf[i] = SPI.transfer(0);    // Perform SPI.transfer to read unsigned char from nRF24L01

	digitalWrite(CSN, 1);                   // Set CSN high again

	return(sstatus);                  // return nRF24L01 status unsigned char
}


/**************************************************
 * Function: spiWrite();
 * 
 * Description:
 * Writes value 'value' to register 'reg'
/**************************************************/
static unsigned char spiWrite(unsigned char reg, char value) {
	unsigned char sstatus;

	digitalWrite(CSN, 0);                   // CSN low, init SPI transaction
	SPI.transfer(reg);                            // select register
	SPI.transfer(value);                          // ..and write value to it..
	digitalWrite(CSN, 1);                   // CSN high again

	return(sstatus);                   // return nRF24L01 status unsigned char
}


/**************************************************
 * Function: spiWrite();
 * 
 * Description:
 * Writes contents of buffer '*pBuf' to nRF24L01
/**************************************************/
static unsigned char spiWrite(unsigned char reg, char *pBuf, uint16_t length) {
  unsigned char sstatus,i;

  digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
  sstatus = SPI.transfer(reg);             // Select register to write to and read status unsigned char
  for(i=0;i<length; i++)             // then write all unsigned char in buffer(*pBuf)
  {
    SPI.transfer(*pBuf++);
  }
  digitalWrite(CSN, 1);                   // Set CSN high again
  return(sstatus);                  // return nRF24L01 status unsigned char
}

/**************************************************
 * Function: radioInit();
 * 
 * Description:
 * Initialize the nRF24L01 module, with static address, max power with 2Mb datarate and 10 retrans with 500us of delay
/**************************************************/
bool radioInit() {
	pinMode(CE,  OUTPUT);
	pinMode(CSN, OUTPUT);
	pinMode(IRQ, INPUT);
	SPI.begin();
	digitalWrite(IRQ, 0);
	digitalWrite(CE, 0);	
	digitalWrite(CSN, 1);
	unsigned char sstatus=spiRead(STATUS);
	if(sstatus != 0x00) {
		//***set up to TX mode***
		digitalWrite(CE, 0);
		spiWrite(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
		spiWrite(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
		spiWrite(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
		spiWrite(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
		spiWrite(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
		spiWrite(WRITE_REG + RF_CH, 0x28);        // Select RF channel 40
		spiWrite(WRITE_REG + RF_SETUP, DATARATE_2MB+POWER_MAX);   // TX_PWR:0dBm, Datarate:2Mbps
		spiWrite(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..
		digitalWrite(CE, 1);
		return true;
	} else return false;
}

/**************************************************
 * Function: radioSend();
 * 
 * Description:
 * Send a value to the receiver, return true if done
/**************************************************/
bool radioSend(char* value, uint16_t length) {
	unsigned char sstatus = spiRead(STATUS);

	if(sstatus&TX_DS) {                            // if receive data ready (TX_DS) interrupt
    	spiWrite(FLUSH_TX,0);                                  
    	spiWrite(WR_TX_PLOAD,value,length);       // write playload to TX_FIFO
    }

    spiWrite(WRITE_REG+STATUS,sstatus);         // clear TX_DS or MAX_RT interrupt flag

	return !(sstatus&MAX_RT);					// return true if MAX_RT interrupt is not flagged
}
