/** 
	*  @file           telemetry.cpp
	*  @author         De Giovanni Marco <br/>
	*                  (mail: mar.degiovanni@stud.uniroma3.it)
	*  @date           2018
	*  @brief          Implementation to nRF24L01 radio module
*/

#define CE       7          
#define CSN      8
#define IRQ      2

void telemetry::init();
void telemetry::send(char* value, uint16_t length);

