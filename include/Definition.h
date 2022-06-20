// Define Module
#define __B100BB__

// Global Definations
#define TERMINAL
#define BUZZER
#define LIGHTS
//#define HMI

// Define Version Parameters
#define	__PowerStat_Firmware__			"02.02.05"
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

// Include Library Deps.
#include "Console_Parameters.h"
#include <Console.h>
#include <dWin.h>
#include <Telit_xE910.h>
#include <ArduinoJson.h>

// AVR Librarys
#include <avr/interrupt.h>

// Include FilterStat Definitions
#include "Hardware.h"
#include "I2C.h"
#include "AI.h"
