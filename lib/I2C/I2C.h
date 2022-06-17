// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define Wire Library
#ifndef __Wire__
#include <Wire.h>
#endif

// Define RTC Library
#ifndef __RV3028__
#include <RV3028.h>
#endif

// Define Statistical Library
#ifndef __Statistical__
#include <Statistical.h>
#endif

// Define Environment Library
#ifndef __Environment__
#include <Environment.h>
#endif

// Define Battery Gauge Library
#ifndef __MAX17055__
#include <MAX17055.h>
#endif

// Define Battery Charger Library
#ifndef __BQ24298__
#include <BQ24298.h>
#endif

// Define Console Library
#ifndef __Console__
#include <Console.h>
#endif

// Define HMI Object
#ifndef __dWin__
#include <dWin.h>
#endif

// Define Objects
dwin LCD_I2C;

class I2C {

	private:

		/**
		 * @brief Detect I2C Device
		 */
		void Device_TCA9548(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_TCA9548__);

			// Detect I2C Device
			this->Network.Diagnostic.Multiplexer = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Multiplexer) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_RV3028C7(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_RV3028C7__, true, 1);

			// Detect I2C Device
			this->Network.Diagnostic.RTC = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.RTC) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 1, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

				// RTC Object Defination	
				RV3028 RTC(true, 1);

				// Set RTC Parameters
				RTC.Disable_Trickle_Charger();
				RTC.Set_24h_Clock();
				RTC.Clear_UNIX_Time();

				// Get Time
				this->Get_Time();

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
						Terminal.Text(_Console_I2C_Diagnostic_X + 1, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_DS28C(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_DS28C__, true, 2);

			// Detect I2C Device
			this->Network.Diagnostic.Serial_ID = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Serial_ID) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 2, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

				// Define Variable
				uint64_t _Serial = 0x00;
				uint8_t _Read_Byte;

				// Set DS28C to I2C Mode
				I2C_Device.Write_Register(0x08, 0x01, false);

				// Send CRC  Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x07);
				_Serial |= (uint64_t)_Read_Byte;

				// Send 40-47 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x06);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send 32-39 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x05);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send 24-31 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x04);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send 16-23 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x03);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send 08-15 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x02);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send 00-07 bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x01);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Send Device Family bit Read Request to DS28C and read
				_Read_Byte = I2C_Device.Read_Register(0x00);
				_Serial = _Serial << 8;
				_Serial |= (uint64_t)_Read_Byte;

				// Set Array
				String(uint64ToString(_Serial)).toCharArray(this->Network.Serial_ID, 16);
				
				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_SerialID_X, _Console_SerialID_Y, CYAN, String(this->Network.Serial_ID));
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 2, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_HDC2010(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_HDC2010__, true, 3);

			// Detect I2C Device
			this->Network.Diagnostic.Environment = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Environment) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 3, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

				// Read Environment
				this->Get_Environment();

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 3, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_MAX17055(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_MAX17055__, true, 4);

			// Detect I2C Device
			this->Network.Diagnostic.Gauge = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Gauge) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 4, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

				// Get Battery Parameters
				this->Get_Battery();

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 4, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_BQ24298(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_BQ24298__, true, 5);

			// Detect I2C Device
			this->Network.Diagnostic.Charger = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Charger) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 5, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
				#endif

				// Get Charger Variable
				this->Get_Charger();

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 5, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_PCF8574_A(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_PCF8574_A__, true, 6);

			// Detect I2C Device
			this->Network.Diagnostic.Expander_A = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Expander_A) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 19, GREEN, _Console_OK_);
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 18, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_PCF8574_B(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_PCF8574_B__, true, 6);

			// Detect I2C Device
			this->Network.Diagnostic.Expander_B = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Expander_B) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 43, GREEN, _Console_OK_);
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 42, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_PCF8574_C(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_PCF8574_C__, true, 6);

			// Detect I2C Device
			this->Network.Diagnostic.Expander_C = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Expander_C) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 67, GREEN, _Console_OK_);
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 66, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		/**
		 * @brief Detect I2C Device
		 */
		void Device_PCF8574_D(void) {

			// Define Object
			I2C_Functions I2C_Device(__I2C_Addr_PCF8574_D__, true, 6);

			// Detect I2C Device
			this->Network.Diagnostic.Expander_D = I2C_Device.Detect();

			// Control Device State
			if (this->Network.Diagnostic.Expander_D) {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 90, GREEN, _Console_OK_);
				#endif

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(32, 89, RED, _Console_FAIL_);
				#endif

			}

			// Wait Delay
			delay(10);

		}

		String uint64ToString(uint64_t input) {
			
			String result = "";
			uint8_t base = 16;

			do {
				
				char c = input % base;
				input /= base;

				if (c < 10)
					c +='0';
				else
					c += 'A' - 10;
			
				result = c + result;

		} while (input);

		return result;

		}

		double Round_2digit(double _Value) {
			return ((int)(_Value * 100 + 0.5) / 100.0);
		}

	public:

		// I2C Network Diagnostic Variables
		struct Struct_I2C_Network {

			// I2C Network Diagnostic Variables
			struct Struct_Diagnostic {
				bool Multiplexer = false;
				bool RTC = false;
				bool Serial_ID = false;
				bool Environment = false;
				bool Gauge = false;
				bool Charger = false;
				bool Expander_A = false;
				bool Expander_B = false;
				bool Expander_C = false;
				bool Expander_D = false;
			} Diagnostic;

			// Define Global Variables
			char Serial_ID[18];

			// I2C Environment Variables
			struct Struct_Environment {
				float Temperature = 0;
				float Humidity = 0;
			} Environment;

			// Gauge Variables
			struct Struct_Battery {
				float Instant_Voltage = 0;
				float Temperature = 0;
				float Average_Current = 0;
				float State_Of_Charge = 0;
				uint16_t Design_Capacity = 0;
				uint16_t Instant_Capacity = 0;
				uint8_t Charge_Cycle = 0;
				uint8_t Charge_Status = 0;
			} Battery;

			// RTC Time Variables
			struct Struct_Time {
				uint8_t Hour = 0;
				uint8_t Minute = 0;
				uint8_t Second = 0;
				uint8_t Day = 0;
				uint8_t Month = 0;
				uint8_t Year = 0;
				uint32_t UNIX = 0;
			} Time;

		} Network;

		/**
		 * @brief Construct a new I2C object
		 */
		I2C(void) {

		}

		void Begin(void) {

			// Begin I2C Communication
			Wire.begin();

			// Detect Multiplexer
			this->Device_TCA9548();

			// Detect RTC
			this->Device_RV3028C7();

			// Detect DS28C
			this->Device_DS28C();

			// Detect HDC2010
			this->Device_HDC2010();

			// Detect MAX17055
			this->Device_MAX17055();

			// Detect Charger
			this->Device_BQ24298();

			// Detect Expander
			this->Device_PCF8574_A();
			this->Device_PCF8574_B();
			this->Device_PCF8574_C();
			this->Device_PCF8574_D();

		}

		// Environment Functions
		void Get_Environment(void) {

			if (this->Network.Diagnostic.Environment) {

				// Define Sensor Object
				HDC2010 Sensor(true, 3, 5, true);

				// Get Temperature
				this->Network.Environment.Temperature = this->Round_2digit(Sensor.Temperature());

				// Get Humidity
				this->Network.Environment.Humidity = this->Round_2digit(Sensor.Humidity());

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_Environment_X, _Console_Environment_Y, CYAN, String(this->Network.Environment.Temperature, 2));
					Terminal.Text(_Console_Environment_X + 1, _Console_Environment_Y, CYAN, String(this->Network.Environment.Humidity, 2));
				#endif

			}

		}

		// Battery Functions
		void Get_Battery(void) {

			if (this->Network.Diagnostic.Gauge) {

				// Create Charger Object
				MAX17055 Battery_Gauge(true, 4);

				// Get Battery Parameters
				this->Network.Battery.Instant_Voltage 	= Round_2digit(Battery_Gauge.Instant_Voltage());
				this->Network.Battery.Temperature		= Round_2digit(Battery_Gauge.Temperature());
				this->Network.Battery.Average_Current 	= Round_2digit(Battery_Gauge.Average_Current());
				this->Network.Battery.State_Of_Charge 	= Round_2digit(Battery_Gauge.State_Of_Charge());
				this->Network.Battery.Design_Capacity 	= Battery_Gauge.Design_Capacity();
				this->Network.Battery.Instant_Capacity 	= Battery_Gauge.Instant_Capacity();
				this->Network.Battery.Charge_Cycle 		= Battery_Gauge.Charge_Cycle();

				// Terminal Update
				#ifdef TERMINAL
					Terminal.Text(_Console_Battery_X, _Console_Battery_Y + 18, CYAN, String(Network.Battery.Instant_Voltage, 2));
					Terminal.Text(_Console_Battery_X + 1, _Console_Battery_Y + 17, CYAN, String(Network.Battery.Temperature, 2));
					Terminal.Text(_Console_Battery_X + 2, _Console_Battery_Y + 15, CYAN, String(Network.Battery.Average_Current, 2));
					Terminal.Text(_Console_Battery_X + 3, _Console_Battery_Y + 19, CYAN, String(Network.Battery.State_Of_Charge, 2));
					Terminal.Text(_Console_Battery_X + 4, _Console_Battery_Y + 17, CYAN, String(Network.Battery.Design_Capacity));
					Terminal.Text(_Console_Battery_X + 5, _Console_Battery_Y + 17, CYAN, String(Network.Battery.Instant_Capacity));
					Terminal.Text(_Console_Battery_X + 6, _Console_Battery_Y + 20, CYAN, String(Network.Battery.Charge_Cycle));
				#endif

				// HMI Output
				#ifdef HMI

					// Define Variable
					uint8_t _Bat_Level = 0;

					// Decide Level
					if (this->Network.Battery.State_Of_Charge >= 0 and this->Network.Battery.State_Of_Charge < 20 ) _Bat_Level = 1;
					if (this->Network.Battery.State_Of_Charge >= 20 and this->Network.Battery.State_Of_Charge < 40 ) _Bat_Level = 2;
					if (this->Network.Battery.State_Of_Charge >= 40 and this->Network.Battery.State_Of_Charge < 60 ) _Bat_Level = 3;
					if (this->Network.Battery.State_Of_Charge >= 60 and this->Network.Battery.State_Of_Charge < 80 ) _Bat_Level = 4;
					if (this->Network.Battery.State_Of_Charge >= 80 and this->Network.Battery.State_Of_Charge <= 100 ) _Bat_Level = 5;

					LCD_I2C.Set_HMI_Battery(_Bat_Level, this->Network.Battery.Instant_Voltage, this->Network.Battery.Temperature, this->Network.Battery.Average_Current, this->Network.Battery.State_Of_Charge, this->Network.Battery.Design_Capacity, this->Network.Battery.Instant_Capacity);
				
				#endif

			}

		}

		// Battery Functions
		void Get_Charger(void) {

			if (this->Network.Diagnostic.Charger) {

				// Create Charger Object
				BQ24298 Charger(true, true, 5);

				// Get Charge Status
				this->Network.Battery.Charge_Status = Charger.Charge_Status();

			}

		}

		// RTC Functions
		void Get_UNIX(void) {

			// Control I2C Device
			if (this->Network.Diagnostic.RTC) {

				// RTC Object Defination	
				RV3028 RTC(true, 1);

				// Get UNIX
				this->Network.Time.UNIX = RTC.Get_UNIX_Time();

				// Terminal Update
				#ifdef TERMINAL
					Terminal.Text(2, 13, WHITE, String(this->Network.Time.UNIX));
				#endif

			}

		}
		void Get_Time(void) {

			// Control I2C Device
			if (this->Network.Diagnostic.RTC) {

				// RTC Object Defination	
				RV3028 RTC(true, 1);

				// Update Current Time Variables
				this->Network.Time.Day 		= RTC.Get_Date();
				this->Network.Time.Month 	= RTC.Get_Month();
				this->Network.Time.Year 	= RTC.Get_Year();
				this->Network.Time.Hour 	= RTC.Get_Hour();
				this->Network.Time.Minute 	= RTC.Get_Minute();
				this->Network.Time.Second 	= RTC.Get_Second();

				// Print Time Stamp
				#ifdef TERMINAL

					// Handle TimeStamp
					char _Response_Time_Array[30];	// 2022-03-24  10:36:00
					sprintf(_Response_Time_Array, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", this->Network.Time.Year, this->Network.Time.Month, this->Network.Time.Day, this->Network.Time.Hour, this->Network.Time.Minute, this->Network.Time.Second);

					// Print
					Terminal.Text(_Console_TimeStamp_X, _Console_TimeStamp_Y, CYAN, String(_Response_Time_Array));

				#endif

				// HMI Output
				#ifdef HMI

					// Print Time
					LCD_I2C.Set_HMI_Time_Stamp(this->Time.Day, this->Time.Month, this->Time.Year, this->Time.Hour + 3, this->Time.Minute, this->Time.Second);

				#endif

			}

		}
		void Set_Time(uint8_t Day, uint8_t Month, uint8_t Year, uint8_t Hour, uint8_t Minute, uint8_t Second) {

			// Control I2C Device
			if (this->Network.Diagnostic.RTC) {

				// RTC Object Defination	
				RV3028 RTC(true, 1);

				// Update Time
				RTC.Set_Time(Second, Minute, Hour, Day, Month, Year);

			}

		}
		void Set_Timer(uint16_t Interval) {

			// Control I2C Device
			if (this->Network.Diagnostic.RTC) {

				// RTC Object Defination	
				RV3028 RTC(true, 1);

				// Set Timer
				RTC.Set_Timer(true, 1, Interval, (Interval != 0 ? true : false), true, (Interval != 0 ? true : false));

			}

		}

		// Selenoid Functions
		void Set_Selenoid(uint8_t _ID, bool _State) {

			// Set Defaults
			uint16_t ON_Time = 200;
			uint16_t OFF_Time = 200;

			// Set Selenoid 1
			if (_ID == 1) {

				// Set Object
				I2C_Functions I2C_PCF8574_A(__I2C_Addr_PCF8574_A__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00000001, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 4, RED, "S01");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00000010, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 4, GREEN, "S01");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 2
			if (_ID == 2) {

				// Set Object
				I2C_Functions I2C_PCF8574_A(__I2C_Addr_PCF8574_A__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00000100, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 10, RED, "S02");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00001000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 10, GREEN, "S02");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 3
			if (_ID == 3) {

				// Set Object
				I2C_Functions I2C_PCF8574_A(__I2C_Addr_PCF8574_A__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00010000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 16, RED, "S03");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b00100000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 16, GREEN, "S03");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 4
			if (_ID == 4) {

				// Set Object
				I2C_Functions I2C_PCF8574_A(__I2C_Addr_PCF8574_A__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b01000000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 22, RED, "S04");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_A.Write_Command(0b10000000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_A.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 22, GREEN, "S04");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 5
			if (_ID == 5) {

				// Set Object
				I2C_Functions I2C_PCF8574_B(__I2C_Addr_PCF8574_B__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00000001, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 28, RED, "S05");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00000010, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 28, GREEN, "S05");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 6
			if (_ID == 6) {

				// Set Object
				I2C_Functions I2C_PCF8574_B(__I2C_Addr_PCF8574_B__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00000100, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 34, RED, "S06");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00001000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 34, GREEN, "S06");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 7
			if (_ID == 7) {

				// Set Object
				I2C_Functions I2C_PCF8574_B(__I2C_Addr_PCF8574_B__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00010000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 40, RED, "S07");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b00100000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 40, GREEN, "S07");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 8
			if (_ID == 8) {

				// Set Object
				I2C_Functions I2C_PCF8574_B(__I2C_Addr_PCF8574_B__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b01000000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 46, RED, "S08");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_B.Write_Command(0b10000000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_B.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 46, GREEN, "S08");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 9
			if (_ID == 9) {

				// Set Object
				I2C_Functions I2C_PCF8574_C(__I2C_Addr_PCF8574_C__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00000001, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 52, RED, "S09");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00000010, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 52, GREEN, "S09");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 10
			if (_ID == 10) {

				// Set Object
				I2C_Functions I2C_PCF8574_C(__I2C_Addr_PCF8574_C__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00000100, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 58, RED, "S10");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00001000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 58, GREEN, "S10");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 11
			if (_ID == 11) {

				// Set Object
				I2C_Functions I2C_PCF8574_C(__I2C_Addr_PCF8574_C__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00010000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 64, RED, "S11");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b00100000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 64, GREEN, "S11");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 12
			if (_ID == 12) {

				// Set Object
				I2C_Functions I2C_PCF8574_C(__I2C_Addr_PCF8574_C__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b01000000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 70, RED, "S12");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_C.Write_Command(0b10000000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_C.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 70, GREEN, "S12");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 13
			if (_ID == 13) {

				// Set Object
				I2C_Functions I2C_PCF8574_D(__I2C_Addr_PCF8574_D__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00000001, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 76, RED, "S13");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00000010, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 76, GREEN, "S13");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 14
			if (_ID == 14) {

				// Set Object
				I2C_Functions I2C_PCF8574_D(__I2C_Addr_PCF8574_D__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00000100, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 82, RED, "S14");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00001000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 82, GREEN, "S14");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 15
			if (_ID == 15) {

				// Set Object
				I2C_Functions I2C_PCF8574_D(__I2C_Addr_PCF8574_D__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00010000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 88, RED, "S15");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b00100000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 88, GREEN, "S15");
					#endif

				}

				// End Function
				return;

			}

			// Set Selenoid 16
			if (_ID == 16) {

				// Set Object
				I2C_Functions I2C_PCF8574_D(__I2C_Addr_PCF8574_D__, true, 6);

				// Set State
				if (!_State) {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b01000000, true);

					// On Delay
					delay(OFF_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 94, RED, "S16");
					#endif

				} else {

					// Set I/O State
					I2C_PCF8574_D.Write_Command(0b10000000, true);

					// On Delay
					delay(ON_Time);

					// Clear I/O State
					I2C_PCF8574_D.Write_Command(0b00000000, true);

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(34, 94, GREEN, "S16");
					#endif

				}

				// End Function
				return;

			}

		}
		void Clear_Selenoid(uint8_t _Filter_Count) {

			// Clear Selenoids
			for (size_t i = 1; i <= _Filter_Count; i++) {

				// Turn OFF Selenoid
				this->Set_Selenoid(i, false);

			}

		}

};