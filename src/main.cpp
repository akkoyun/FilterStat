// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Include PowerStat Definitions
#include "Definition.h"

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

// Declare Objects
Hardware B100BB_Hardware;
I2C B100BB_I2C;
AI B100BB_AI;
//dwin LCD;

// Define IoT Object
Telit_xE910 GSM;
GSM_Socket_Incomming Incomming_Socket((uint8_t)2);
GSM_Socket_Outgoing Outgoing_Socket((uint8_t)3, FilterStat.IoT.Server_IP, FilterStat.IoT.Server_EndPoint);

// Time Functions
void Update_UNIX_Time(void) {

	// Update UNIX Time
	if (FilterStat.Interrupt.UNIX_Time_Update) {

		// Get UNIX Time
		B100BB_I2C.Get_UNIX();

		// Get Time
		B100BB_I2C.Get_Time();

		// Clear Interrupt
		FilterStat.Interrupt.UNIX_Time_Update = false;

	}

}

// IoT Functions
void Online_Routine(void) {

	// Control Online Status
	if (!FilterStat.IoT.Online and B100BB_AI.Parameters.Status.Irrigation) {

		// Disable Interrupt
		FilterStat.Interrupt.Interrupt_Enable = false;

		// GSM Begin
		GSM.Power(true);

		// Initialize Modem
		GSM.Initialize();
		
		// Connect Modem
		FilterStat.IoT.Online = GSM.Connect();

		// Time Update
		if (GSM.Time_Update()) B100BB_I2C.Set_Time(GSM.Time.Day, GSM.Time.Month, GSM.Time.Year, GSM.Time.Hour, GSM.Time.Minute, GSM.Time.Second);

		// Socket Config
		Incomming_Socket.Configure();
		Outgoing_Socket.Configure();

		// Listen Socket
		Incomming_Socket.Listen(true);

		// Start Timer
		B100BB_I2C.Set_Timer(B100BB_AI.Parameters.Clean.Defaults.Interval);

		// Enable Interrupt
		FilterStat.Interrupt.Interrupt_Enable = true;

	}

}
void Recieve_Command(void) {

	// Control for GSM Ring
	if (FilterStat.Interrupt.GSM_Interrupt) {

		// Get Command
		FilterStat.IoT.Remote_Command = Incomming_Socket.Get();

		// Beep
		Terminal.Beep();

		// Clear Interrupt
		FilterStat.Interrupt.GSM_Interrupt = false;

	}

}

// Clean Function
void Filter_Clean(void) {

	// Disable Interrupt
	FilterStat.Interrupt.Interrupt_Enable = false;

	// Terminal Update
	#ifdef TERMINAL
		Terminal.Text(48, 3, GREEN, "                                                  ");

		// Print Clean Text
		if (B100BB_AI.Parameters.Status.Clean_Type == 1) Terminal.Text(48, 105, YELLOW, "   Auto   ");
		if (B100BB_AI.Parameters.Status.Clean_Type == 2) Terminal.Text(48, 105, YELLOW, "  Timed   ");
		if (B100BB_AI.Parameters.Status.Clean_Type == 3) Terminal.Text(48, 105, YELLOW, "  Remote  ");
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
		uint8_t Selenoid_Start_Time = B100BB_I2C.Network.Time.UNIX;

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
			uint8_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				char Selenoid_Timer[4]; // 2022-03-24  10:36:00
				sprintf(Selenoid_Timer, "%03hhu", Selenoid_Time_Counter);
				Terminal.Text(35, 88, GREEN, String(Selenoid_Timer));
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
			uint8_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				char Selenoid_Timer[4]; // 2022-03-24  10:36:00
				sprintf(Selenoid_Timer, "%03hhu", Selenoid_Time_Counter);
				Terminal.Text(35, 88, YELLOW, String(Selenoid_Timer));
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
	for (uint8_t i = 1; i <= B100BB_Hardware.Module.Setting.Filter_Count; i++) {

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
			char Selenoid_ID_On[3];
			sprintf(Selenoid_ID_On, "%02hhu", i);
			Terminal.Text(48, 12, YELLOW, String(Selenoid_ID_On));
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
			uint8_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				char Selenoid_Timer[4]; // 2022-03-24  10:36:00
				sprintf(Selenoid_Timer, "%03hhu", Selenoid_Time_Counter);
				Terminal.Text(35, ((i * 6) - 2), GREEN, String(Selenoid_Timer));
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
			char Selenoid_ID_Off[3];
			sprintf(Selenoid_ID_Off, "%02hhu", i);
			Terminal.Text(48, 12, YELLOW, String(Selenoid_ID_Off));
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
			uint8_t Selenoid_Time_Counter = B100BB_I2C.Network.Time.UNIX - Selenoid_Start_Time;

			// Terminal Update
			#ifdef TERMINAL
				char Selenoid_Timer[4]; // 2022-03-24  10:36:00
				sprintf(Selenoid_Timer, "%03hhu", Selenoid_Time_Counter);
				Terminal.Text(35, ((i * 6) - 2), YELLOW, String(Selenoid_Timer));
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

	// Enable Interrupt
	FilterStat.Interrupt.Interrupt_Enable = true;

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
	if (FilterStat.Interrupt.Interrupt_Enable) FilterStat.Interrupt.RTC_Timer_Interrupt = true;

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {

	// Control Interrupt Handler
	if (FilterStat.Interrupt.Interrupt_Enable) {

		// Control Ring Interrupt [PJ2]
		if ((PINJ & (1 << PINJ2)) == (1 << PINJ2)) {
			
			// Set Interrupt Variable
			FilterStat.Interrupt.GSM_Interrupt = true;

			// Interrupt Delay
			delay(75);

		} else {
			
			// Set Interrupt Variable
			FilterStat.Interrupt.GSM_Interrupt = false;

		}

	}

}

void setup() {

	// Start Objects
	B100BB_Hardware.Begin();
	B100BB_I2C.Begin();
	B100BB_AI.Begin();

	// Set Variables
	B100BB_AI.Parameters.Clean.Defaults.Auto_Clean_Start = 0.5;
	B100BB_AI.Parameters.Clean.Defaults.Interval = 300;
	B100BB_AI.Parameters.Irrigation.Limit.Start_Limit = 0.2;
	B100BB_AI.Parameters.Irrigation.Limit.End_Limit = 0.2;

	// Set Selenoids OFF
	B100BB_I2C.Clear_Selenoid(B100BB_Hardware.Module.Setting.Filter_Count);

	// Hydrosyclone OFF
	if (B100BB_Hardware.Module.Setting.HydroSyclone) B100BB_I2C.Set_Selenoid(15, false);

	// Pressure Stabiliser OFF
	if (B100BB_Hardware.Module.Setting.Stabiliser) B100BB_I2C.Set_Selenoid(16, false);

	// Measure Pressure
	B100BB_AI.Handle_Status();
	
	// Set Pin Change Interrupt Mask 1
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);

	// Enable Interrupt Variables
	FilterStat.Interrupt.Interrupt_Enable = true;
	FilterStat.Interrupt.Pressure_Measurement = true;

	// Boot Delay
	delay(500);

}

void loop() {

	// Time Update
	Update_UNIX_Time();

	// Get Server Command
	Recieve_Command();

	// Handle Status
	if (FilterStat.Interrupt.Handle_Status) {

		// Decide Clean
		B100BB_AI.Handle_Status();

		// GSM Online Routine
		Online_Routine();

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

		// Timed Clean Control
		if (FilterStat.IoT.Remote_Command == 100 and B100BB_AI.Parameters.Clean.Authorization.Timed) {

			// Set Clean Variable
			B100BB_AI.Parameters.Status.Clean_Type = 3;
			B100BB_AI.Parameters.Clean.Process = true;

			// Clear Command
			FilterStat.IoT.Remote_Command = 0;

		}

		// Clean Process
		if (B100BB_AI.Parameters.Clean.Process) Filter_Clean();

		// Clear Interrupt
		FilterStat.Interrupt.Handle_Clean = false;

	}

	// Control for Socket
	if (B100BB_Hardware.Timer_Control(30)) Incomming_Socket.Listen(true);

}

// 1903
// STF Irrigation Systems
// Res-Dev Department