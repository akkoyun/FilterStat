// Define Module
#define __B100BB__

// Global Definations
#define TERMINAL
#define BUZZER
#define LIGHTS
//#define HMI

// Define Version Parameters
#define	__PowerStat_Firmware__			"02.02.01"
#define	__PowerStat_Hardware__			"03.00.00"

// Serial Communications Definations
#define Serial_Terminal					Serial
#define Serial_HMI						Serial1
#define Serial_Energy					Serial2
#define Serial_GSM						Serial3

// Define LED Variables
#define	__WHITE__						(uint8_t)0
#define	__RED__							(uint8_t)1
#define	__GREEN__						(uint8_t)2
#define	__BLUE__						(uint8_t)3
#define	__PURPLE__						(uint8_t)4

// Pressure Measure Parameters
#define __Pressure_Channel_Input__		(uint8_t)1
#define __Pressure_Channel_Output__		(uint8_t)2
#define __Pressure_Measure_Count__		(uint8_t)100
#define __Pressure_Calibration_a__		(float)1.5777
#define __Pressure_Calibration_b__		(float)-1.1925









// Terminal Coordinates
#ifdef TERMINAL

	// Console Texts
	#define _Console_OK_ "OK"
	#define _Console_FAIL_ "FAIL"

	// Version Information Coordinate
	#define _Console_Version_X 6
	#define _Console_Version_Y 69

	// I2C Information Coordinate
	#define _Console_I2C_Diagnostic_X 5
	#define _Console_I2C_Diagnostic_Y 33




	// SerialID Coordinate
	#define _Console_SerialID_X 5
	#define _Console_SerialID_Y 61

	// Environment Coordinate
	#define _Console_Environment_X 8
	#define _Console_Environment_Y 70

	// Battery Coordinate
	#define _Console_Battery_X 5
	#define _Console_Battery_Y 93

	// TimeStamp Coordinate
	#define _Console_TimeStamp_X 2
	#define _Console_TimeStamp_Y 94

	// State Coordinate
	#define _Console_State_X 10
	#define _Console_State_Y 74

	// State Coordinate
	#define _Console_Inputs_X 40
	#define _Console_Inputs_Y 7

	// Pressure Coordinate
	#define _Console_Data_X 43
	#define _Console_Data_Y 5

#endif

// Include PowerStat Definitions
#include "Hardware.h"
#include "I2C.h"
#include "AI.h"

// Include GSM Library
#ifndef __Telit_xE910__
#include <Telit_xE910.h>
#endif

// AVR Interrupt Library
#include <avr/interrupt.h>

// Define JSON Library
#include <ArduinoJson.h>

// Define HMI Object
#ifndef __dWin__
#include <dWin.h>
#endif
