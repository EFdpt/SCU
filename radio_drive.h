/** 
	*  @file           radio_drive.h
	*  @author         De Giovanni Marco <br/>
	*                  (mail: mar.degiovanni@stud.uniroma3.it)
	*  @date           2018
	*  @brief          nRF24L01 radio module header
*/

#define DATARATE_250KB	0x20 // RF data rate to 250kb
#define DATARATE_1MB	0x00 // RF data rate to 1Mb
#define DATARATE_2MB	0x08 // RF data rate to 2Mb

#define POWER_MAX		0x06 //	RF output 0dBm
#define POWER_HIGH		0x04 // RF output -6dBm
#define POWER_LOW		0x02 // RF output -12dBm
#define POWER_MIN		0x00 // RF output -18dBm

#define CE       7          
#define CSN      8
#define IRQ      2

bool radioInit();
bool radioSend(char* value, uint16_t length);

