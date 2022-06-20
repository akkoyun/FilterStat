// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Declare Global Variable
struct FilterStat_Struct {

	// Interrupt Variables
	struct Interrupt_Struct {
		bool Interrupt_Enable = false;
		bool Pressure_Measurement = false;
		bool Handle_Status = false;
		bool Handle_Clean = false;
		bool UNIX_Time_Update = false;
		bool RTC_Timer_Interrupt = false;
		bool GSM_Interrupt = false;
	} Interrupt;

	// GSM Variables
	struct IoT_Struct {
		char Server_IP[15] = "54.216.226.171";
		char Server_EndPoint[15] = "/api/v1.1/p402";
		bool Online = false;
		uint16_t Remote_Command = 0;
		char JSON[650];
	} IoT;

} FilterStat;