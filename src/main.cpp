// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Include PowerStat Definitions
#include "Definition.h"

// Declare Objects
Hardware B100BB_Hardware;
I2C B100BB_I2C;
AI B100BB_AI;
//dwin LCD;

// Declare Global Variable
struct FilterStat_Struct {

	// Interrupt Variables
	struct Interrupt_Struct {
		bool Interrupt_Enable = true;
		bool Pressure_Measurement = false;
		bool Handle_Status = false;
		bool Handle_Clean = false;
		bool UNIX_Time_Update = false;
		bool RTC_Timer_Interrupt = false;

		bool GSM_Interrupt = false;
	} Interrupt;

	// GSM Variables
	struct IoT_Struct {
		bool Online = false;
		char JSON[650];
	} IoT;

} FilterStat;

// Time Functions
void Update_UNIX_Time(void) {

	// Update UNIX Time
	if (FilterStat.Interrupt.UNIX_Time_Update) {

		// Get UNIX Time
		B100BB_I2C.Get_UNIX();

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(2, 13, WHITE, String(B100BB_I2C.Network.Time.UNIX));
		#endif

		// Clear Interrupt
		FilterStat.Interrupt.UNIX_Time_Update = false;

	}

}

// Clean Function
void Filter_Clean(void) {

	// Terminal Update
	#ifdef TERMINAL
		Terminal.Text(48, 3, GREEN, "                                                  ");
	#endif

	// Clear RTC Timer
	B100BB_I2C.Set_Timer(0);

	// Handle Status
	B100BB_AI.Handle_Status();

	// Pressure Stabiliser ON
	if (B100BB_Hardware.Module.Setting.Stabiliser) {

		// Open Selenoid
		B100BB_I2C.Set_Selenoid(16, true);

		// On Delay
		delay(500);

	}

	// HydroSyclone Clean
	if (B100BB_Hardware.Module.Setting.HydroSyclone) {

		// Set Variables
		bool Wait_Done = false;
		bool Selenoid_Open = false;
		bool Selenoid_Close = false;

		// Time Update
		Update_UNIX_Time();

		// Set Selenoid On Timer
		uint32_t Selenoid_Start_Time = B100BB_I2C.Network.Time.UNIX;

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(48, 3, GREEN, "                                                  ");
			Terminal.Text(48, 3, GREEN, "Hydrosyclone is Openening");
			Terminal.Text(35, 88, GREEN, "   ");
		#endif

		// Selenoid On Delay
		do {

			// Open Selenoid
			if (!Selenoid_Open) {

				// Open Selenoid
				B100BB_I2C.Set_Selenoid(15, true);

				// Set Variable
				Selenoid_Open = true;

			}

			// Time Update
			Update_UNIX_Time();

			// Declare Terminal Counter
			uint32_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				Terminal.Text(35, 88, GREEN, String(Selenoid_Time_Counter));
			#endif
			
			// Control for On Delay
			if (Selenoid_Time_Counter > B100BB_AI.Parameters.Clean.Defaults.On_Interval) Wait_Done = true;


		} while (!Wait_Done);

		// Time Update
		Update_UNIX_Time();

		// Set Selenoid On Timer
		Selenoid_Start_Time = B100BB_I2C.Network.Time.UNIX;

		// Clear Variables
		Wait_Done = false;
		Selenoid_Open = false;
		Selenoid_Close = false;

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(48, 3, GREEN, "                                                  ");
			Terminal.Text(48, 3, GREEN, "Hydrosyclone is Closing");
			Terminal.Text(35, 88, GREEN, "   ");
		#endif

		// Selenoid Off Delay
		do {

			// Open Selenoid
			if (!Selenoid_Close) {

				// Open Selenoid
				B100BB_I2C.Set_Selenoid(15, false);

				// Set Variable
				Selenoid_Close = true;

			}

			// Time Update
			Update_UNIX_Time();

			// Declare Terminal Counter
			uint32_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				Terminal.Text(35, 88, YELLOW, String(Selenoid_Time_Counter));
			#endif
			
			// Control for On Delay
			if (Selenoid_Time_Counter > B100BB_AI.Parameters.Clean.Defaults.Off_Interval) Wait_Done = true;


		} while (!Wait_Done);

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(35, 88, GREEN, "   ");
		#endif

	}

	// Clean Filter
	for (size_t i = 1; i <= B100BB_Hardware.Module.Setting.Filter_Count; i++) {

		// Set Variables
		bool Wait_Done = false;
		bool Selenoid_Open = false;
		bool Selenoid_Close = false;

		// Time Update
		Update_UNIX_Time();

		// Set Selenoid On Timer
		uint32_t Selenoid_Start_Time = B100BB_I2C.Network.Time.UNIX;

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(48, 3, GREEN, "                                                  ");
			Terminal.Text(48, 3, GREEN, "Selenoid    is Openening");
			Terminal.Text(48, 12, CYAN, String(i));
			Terminal.Text(35, ((i * 6) - 2), GREEN, "   ");
		#endif

		// Selenoid On Delay
		do {

			// Open Selenoid
			if (!Selenoid_Open) {

				// Open Selenoid
				B100BB_I2C.Set_Selenoid(i, true);

				// Set Variable
				Selenoid_Open = true;

			}

			// Time Update
			Update_UNIX_Time();

			// Declare Terminal Counter
			uint32_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				Terminal.Text(35, ((i * 6) - 2), GREEN, String(Selenoid_Time_Counter));
			#endif
			
			// Control for On Delay
			if (Selenoid_Time_Counter > B100BB_AI.Parameters.Clean.Defaults.On_Interval) Wait_Done = true;


		} while (!Wait_Done);

		// Time Update
		Update_UNIX_Time();

		// Set Selenoid On Timer
		Selenoid_Start_Time = B100BB_I2C.Network.Time.UNIX;

		// Clear Variables
		Wait_Done = false;
		Selenoid_Open = false;
		Selenoid_Close = false;

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(48, 3, GREEN, "                                                  ");
			Terminal.Text(48, 3, GREEN, "Selenoid    is Closing");
			Terminal.Text(48, 12, CYAN, String(i));
			Terminal.Text(35, ((i * 6) - 2), GREEN, "   ");
		#endif

		// Selenoid Off Delay
		do {

			// Open Selenoid
			if (!Selenoid_Close) {

				// Open Selenoid
				B100BB_I2C.Set_Selenoid(i, false);

				// Set Variable
				Selenoid_Close = true;

			}

			// Time Update
			Update_UNIX_Time();

			// Declare Terminal Counter
			uint32_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				Terminal.Text(35, ((i * 6) - 2), YELLOW, String(Selenoid_Time_Counter));
			#endif
			
			// Control for On Delay
			if (Selenoid_Time_Counter > B100BB_AI.Parameters.Clean.Defaults.Off_Interval) Wait_Done = true;


		} while (!Wait_Done);

		// Terminal Update
		#ifdef TERMINAL
			Terminal.Text(35, ((i * 6) - 2), GREEN, "   ");
		#endif

		// Decide Clean
		B100BB_AI.Handle_Status();

		// Get Detail Array
		B100BB_AI.Parameters.Clean.Pressure_Array.Input[i - 1] = B100BB_AI.Parameters.Pressure.Input;
		B100BB_AI.Parameters.Clean.Pressure_Array.Output[i - 1] = B100BB_AI.Parameters.Pressure.Output;

	}

	// Pressure Stabiliser OFF
	if (B100BB_Hardware.Module.Setting.Stabiliser) {

		// Open Selenoid
		B100BB_I2C.Set_Selenoid(16, false);

		// On Delay
		delay(500);

	}

	// Start Timer
	B100BB_I2C.Set_Timer(B100BB_AI.Parameters.Clean.Defaults.Interval);

	// Terminal Update
	#ifdef TERMINAL
		Terminal.Text(48, 3, GREEN, "                                                  ");
	#endif

	// Clear Clean Variable
	B100BB_AI.Parameters.Clean.Process = false;
	B100BB_AI.Parameters.Status.Clean_Type = 0;
}

// Timer Interrupt
ISR(TIMER5_COMPA_vect) {

	// Control Interrupt Enable
	if (FilterStat.Interrupt.Interrupt_Enable) B100BB_Hardware.Timer_Count();

	// Update UNIX Time
	FilterStat.Interrupt.UNIX_Time_Update = true;

	// Control for Status 
	FilterStat.Interrupt.Handle_Status = true;

	// Control for Clean 
	FilterStat.Interrupt.Handle_Clean = true;

}

// RTC Timer Interrupt
ISR(PCINT0_vect) {

	// Set Interrupt
	FilterStat.Interrupt.RTC_Timer_Interrupt = true;

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {



}

void setup() {

	// Start Objects
	B100BB_Hardware.Begin();
	B100BB_I2C.Begin();
	B100BB_AI.Begin();

	// Pressure Stabiliser OFF
	if (B100BB_Hardware.Module.Setting.Stabiliser) {

		// Open Selenoid
		B100BB_I2C.Set_Selenoid(16, false);

		// On Delay
		delay(500);

	}

	// Pressure Stabiliser OFF
	if (B100BB_Hardware.Module.Setting.HydroSyclone) {

		// Open Selenoid
		B100BB_I2C.Set_Selenoid(15, false);

		// On Delay
		delay(500);

	}

	// Set Selenoids OFF
	B100BB_I2C.Clear_Selenoid(B100BB_Hardware.Module.Setting.Filter_Count);

	// Measure Pressure
	B100BB_AI.Handle_Status();

	// Boot Delay
	delay(500);

	// Set Pressure Measurement
	FilterStat.Interrupt.Pressure_Measurement = true;

}

void loop() {

	// Time Update
	Update_UNIX_Time();

	// Handle Status
	if (FilterStat.Interrupt.Handle_Status) {

		// Decide Clean
		B100BB_AI.Handle_Status();

		// Decide Irrigation
		if ((B100BB_AI.Parameters.Pressure.Input >= B100BB_AI.Parameters.Irrigation.Limit.Start_Limit) and !B100BB_AI.Parameters.Status.Irrigation) {
			
			// Set Irrigation Variable
			B100BB_AI.Parameters.Status.Irrigation = true;

			// Get Time
			B100BB_I2C.Get_Time();

			// Handle TimeStamp
			sprintf(B100BB_AI.Parameters.Irrigation.Time.Start, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", B100BB_I2C.Network.Time.Year, B100BB_I2C.Network.Time.Month, B100BB_I2C.Network.Time.Day, B100BB_I2C.Network.Time.Hour, B100BB_I2C.Network.Time.Minute, B100BB_I2C.Network.Time.Second);

			// Start Timer
			B100BB_I2C.Set_Timer(B100BB_AI.Parameters.Clean.Defaults.Interval);

		}
		if ((B100BB_AI.Parameters.Pressure.Input < B100BB_AI.Parameters.Irrigation.Limit.End_Limit) and B100BB_AI.Parameters.Status.Irrigation) {

			// Get Time
			B100BB_I2C.Get_Time();

			// Handle TimeStamp
			sprintf(B100BB_AI.Parameters.Irrigation.Time.End, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", B100BB_I2C.Network.Time.Year, B100BB_I2C.Network.Time.Month, B100BB_I2C.Network.Time.Day, B100BB_I2C.Network.Time.Hour, B100BB_I2C.Network.Time.Minute, B100BB_I2C.Network.Time.Second);

			// Reset Variables
			B100BB_AI.Parameters.Status.Irrigation = false;
			B100BB_AI.Parameters.Status.Device = 0;
			B100BB_AI.Parameters.Status.Clean_Type = 0;

			// Start Timer
			B100BB_I2C.Set_Timer(0);

		}

		// Clear Interrupt
		FilterStat.Interrupt.Handle_Status = false;

	}

	// Handle Clean
	if (FilterStat.Interrupt.Handle_Clean) {

		// Auto Clean Control
		if ((B100BB_AI.Parameters.Pressure.Delta > B100BB_AI.Parameters.Clean.Defaults.Auto_Clean_Start) and B100BB_AI.Parameters.Clean.Authorization.Auto) {

			// Set Clean Variable
			B100BB_AI.Parameters.Status.Clean_Type = 1;
			B100BB_AI.Parameters.Clean.Process = true;

		} 

		// Timed Clean Control
		if (FilterStat.Interrupt.RTC_Timer_Interrupt and B100BB_AI.Parameters.Clean.Authorization.Timed) {

			// Set Clean Variable
			B100BB_AI.Parameters.Status.Clean_Type = 2;
			B100BB_AI.Parameters.Clean.Process = true;

			// Clear Interrupt
			FilterStat.Interrupt.RTC_Timer_Interrupt = false;

		}

		// Clean Process
		if (B100BB_AI.Parameters.Clean.Process) Filter_Clean();

		// Clear Interrupt
		FilterStat.Interrupt.Handle_Clean = false;

	}

}

// 1903
// STF Irrigation Systems
// Res-Dev Department