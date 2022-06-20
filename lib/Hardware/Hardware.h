// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Include Libraries
#include "Sounds.h"
#include <SD.h>

// Microcontroller Modules Hardware
class Hardware {

	private:

		/**
		 * @brief Hardware Pin-Out Definitions.
		 */
		inline void Set_Pinout(void) {

            // B100BB Pin Defination
            #ifdef __B100BB__

                /*  PORT A
                    PA0 - Input / Pull Down [Stage 1] 		- AD0		- [22]
                    PA1 - Input / Pull Down [Stage 2] 		- AD1		- [23]
                    PA2 - Input / Pull Down [Stage 3] 		- AD2		- [24]
                    PA3 - Input / Pull Down [Stage 4] 		- AD3		- [25]
                    PA4 - Input / Pull Down [Stabilizer] 	- AD4		- [26]
                    PA5 - Input / Pull Down [Hydrosyclone]	- AD5		- [27]
                    PA6 - Output / Pull Down [NC] 			- AD6		- [28]
                    PA7 - Output / Pull Down [NC] 			- AD7		- [29]
                */
                DDRA |= 0b11000000; PORTA &= 0b00111111;
                DDRA &= 0b11000000; PORTA &= 0b11000000;

                /*  PORT B
                    PB0 - SS
                    PB1 - SCK
                    PB2 - MOSI
                    PB3 - MISO
                    PB4 - Input / Pull Up [RTC Interrupt] 	- PCINT4 	- [10]
                    PB5 - Output / Pull Down [NC] 			- PCINT5 	- [11]
                    PB6 - Output / Pull Down [NC] 			- PCINT6 	- [12]
                    PB7 - Output / Pull Down [NC] 			- PCINT7 	- [13]
                */
                DDRB |= 0b11100000; PORTB &= 0b00011111;
                DDRB &= 0b11101111; PORTB |= 0b00010000;

                /*  PORT C
                    PC0 - Output / Pull Down [SD Mux Dir.]	- AD8		- [37]
                    PC1 - Input / Pull Down [SD Card Det.]	- AD9		- [36]
                    PC2 - Output / Pull Down [NC] 			- AD10		- [35]
                    PC3 - Output / Pull Down [NC] 			- AD11		- [34]
                    PC4 - Output / Pull Down [NC] 			- AD12		- [33]
                    PC5 - Output / Pull Down [NC] 			- AD13		- [32]
                    PC6 - Output / Pull Down [NC] 			- AD14		- [31]
                    PC7 - Output / Pull Down [NC] 			- AD15		- [30]
                */
                DDRC |= 0b00000001;	PORTC &= 0b11111110;
                DDRC &= 0b11111101;	PORTC &= 0b11111101;
                DDRC |= 0b11111100;	PORTC &= 0b00000011;

                /*  PORT D
                    PD0 - SCL
                    PD1 - SDA
                    PD2 - RXD1
                    PD3 - TXD1
                    PD4 - Output / Pull Down [NC] 			- ICP1		- [85]
                    PD5 - Output / Pull Down [MCU LED Blue]	- XCK1		- [84]
                    PD6 - Output / Pull Down [MCU LED Green]- T1		- [83]
                    PD7 - Output / Pull Down [MCU LED Red]	- T0		- [82]
                */
                DDRD |= 0b11110000; PORTD &= 0b00001111;

                /*  PORT E
                    PE0 - RXD0
                    PE1 - TXD0
                    PE2 - Output / Pull Down [NC]			- AIN0		- [-]
                    PE3 - Output / Pull Down [Buzzer EN1]	- AIN1		- [5]
                    PE4 - Output / Pull Down [Buzzer EN2]	- INT4		- [2]
                    PE5 - Output / Pull Down [PWM Buzzer]	- INT5		- [3]
                    PE6 - Output / Pull Down [NC]			- INT6		- [-]
                    PE7 - Output / Pull Down [NC]			- INT7		- [-]
                */
                DDRE |= 0b11111100; PORTE &= 0b11000000;

                /*  PORT F
                    PF0 - Input / Pull Down [Sns 1 Sense]	- ADC0		- [A0]
                    PF1 - Sensor Data 1						- ADC1		- [A1]
                    PF2 - Sensor Data 2						- ADC2		- [A2]
                    PF3 - Input / Pull Down [Sns 2 Sense]	- ADC3		- [A3]
                    PF4 - Output / Pull Down [NC]			- ADC4		- [A4]
                    PF5 - Output / Pull Down [NC]			- ADC5		- [A5]
                    PF6 - Output / Pull Down [NC]			- ADC6		- [A6]
                    PF7 - Output / Pull Down [NC]			- ADC7		- [A7]
                */
                DDRF &= 0b11110000; PORTF &= 0b11110110;
                DDRF |= 0b11110000; PORTF &= 0b00001111;

                /*  PORT G
                    PG0 - Output / Pull Down [FOTA Pwr En]	- WR		- [41]
                    PG1 - Output / Pull Down [NC] 			- RD		- [40]
                    PG2 - Output / Pull Down [NC] 			- ALE		- [39]
                    PG3 - Output / Pull Down [NC] 			- TOSC2		- [-]
                    PG4 - Output / Pull Down [NC] 			- TOSC1		- [-]
                    PG5 - Output / Pull Down [NC] 			- OC0B		- [4]
                */
                DDRG |= 0b00111111; PORTG &= 0b11000000;

                /*  PORT J
                    PJ0 - RXD3
                    PJ1 - TXD3
                    PJ2 - Input / Pull Down [GSM Ring]		- PCINT11	- [-]
                    PJ3 - Input / Pull Down [GSM PwrMon]	- PCINT12	- [-]
                    PJ4 - Output / Pull Down [GSM CommEn]	- PCINT13	- [-]
                    PJ5 - Output / Pull Down [GSM ShtDwn]	- PCINT14	- [-]
                    PJ6 - Output / Pull Down [GSM On Off]	- PCINT15	- [-]
                    PJ7 - Output / Pull Down [NC]			-  			- [-]
                */
                DDRJ &= 0b11110011; PORTJ |= 0b00000100;
                DDRJ |= 0b11110000; PORTJ &= 0b00000111;

                /*  PORT K
                    PK0 - Input / Pull Down [220V Sense 1]  - PCINT16	- [89]
                    PK1 - Input / Pull Down [220V Sense 2]  - PCINT17	- [88]
                    PK2 - Input / Pull Down [220V Sense 3]  - PCINT18	- [87]
                    PK3 - Input / Pull Down [220V Sense 4]  - PCINT19	- [86]
                    PK4 - Input / Pull Down [220V Sense 5]  - PCINT20	- [85]
                    PK5 - Input / Pull Down [220V Sense 6]  - PCINT21	- [84]
                    PK6 - Input / Pull Down [220V Sense 7]  - PCINT22	- [83]
                    PK7 - Input / Pull Down [220V Sense 8]  - PCINT23	- [82]
                */
                DDRK = 0b00000000; PORTK = 0b00000000;

                /*  PORT L
                    PL0 - Output / Pull Down [Start Relay]	- ICP4		- [49]
                    PL1 - Output / Pull Down [Stop Relay]	- ICP5		- [48]
                    PL2 - Output / Pull Down [Alarm Relay]	- T5		- [47]
                    PL3 - Output / Pull Down [Res. Relay]	- OC5A		- [46]
                    PL4 - Output / Pull Down [NC]			- OC5B		- [45]
                    PL5 - Output / Pull Down [NC]			- OC5C		- [44]
                    PL6 - Output / Pull Down [NC]			- 			- [43]
                    PL7 - Output / Pull Down [NC]			- 			- [42]
                */
                DDRL |= 0b11111111; PORTL &= 0b00000000;

            #endif
            
        }

		/**
		 * @brief SD Card Connection Function.
		 */
		void SD_Connect(void) {

			// Control for SD Card
			if (this->Module.SD.Card_Sense) {

				// Turn SD MUX Enable HIGH
				PORTC |= 0b00000001;

				// Mux Activation delay
				delay(5);

				// Begin SD
				if (SD.begin(53)) {

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(_Console_I2C_Diagnostic_X + 6, _Console_I2C_Diagnostic_Y + 1, GREEN, _Console_OK_);
					#endif

					// Set Diagnostic
					this->Module.SD.Status = true;

					// Activate LED
					this->LED(__WHITE__, 1, 200);

				} else {

					// Print Diagnostic
					#ifdef TERMINAL
						Terminal.Text(_Console_I2C_Diagnostic_X + 6, _Console_I2C_Diagnostic_Y, RED, _Console_FAIL_);
					#endif

					// Set Diagnostic
					this->Module.SD.Status = false;

				}

				// Turn SD MUX Enable LOW
				PORTC &= 0b11111110;

			} else {

				// Print Diagnostic
				#ifdef TERMINAL
					Terminal.Text(_Console_I2C_Diagnostic_X + 6, _Console_I2C_Diagnostic_Y, YELLOW, "NoSD");
				#endif

			}

		}

		/**
		 * @brief AVR 1sn Timer Interrupt.
		 */
		void AVR_Timer_1sn(void) {

			// Timer-0 : 8-bit		- Delay, Millis, Micros, AnalogWrite(5,6)
			// Timer-1 : 16-bit		- Servo functions, AnalogWrite(9,10)
			// Timer-2 : 8-bit		- Tone, AnalogWrite(3,11)
			// Timer-3 : 16-bit
			// Timer-4 : 16-bit
			// Timer-5 : 16-bit	 	- Module Timer

			// Set Timer Interval 1 Sn
			uint8_t _Interval = 1;

			// Clear Registers
			TCCR5A = 0x00;
			TCCR5B = 0x00;

			// Clear Counter
			TCNT5 = 0;

			// Set Counter Value
			OCR5A = (F_CPU / ((1 / _Interval) * 1024)) - 1;

			// Set CTC Mod
			TCCR5B |= (1 << WGM52);

			// Set Prescalar (1024)
			TCCR5B |= (1 << CS52) | (1 << CS50);

			// Start Timer
			TIMSK5 |= (1 << OCIE5A);

		}

		/**
		 * @brief AVR PIN Change Interrupt Functions.
		 * @param _Mask Interrupt Mask. 
		 * @param _State Interrupt State.
		 */
		void AVR_Pin_Interrupt(const uint8_t _Mask, const bool _State) {

			/*

				PCICR Register
				--------------

				8   7   6   5   4   3     2     1     0
				-   -   -   -   -   -   PCIE2 PCIE1 PCIE0
				
				PCINT0  - PCINT7   : PCIE0 Mask
				PCINT8  - PCINT15  : PCIE1 Mask
				PCINT16 - PCINT23  : PCIE2 Mask

			*/

			// RTC Alarm Interrupt Mask
			if (_Mask == 1) {

				// Set State
				if (_State) PCICR |= (1 << PCIE0);
				if (!_State) PCICR &= ~(1 << PCIE0);

				// Set Pin Change Interrupt Mask 0
				PCMSK0 |= (1 << PCINT4);

			}
			
			// GSM Ring and Power Mon Interrupt Mask
			if (_Mask == 2) {

				// Set State
				if (_State) PCICR |= (1 << PCIE1);
				if (!_State) PCICR &= ~(1 << PCIE1);

				// Set Pin Change Interrupt Mask 1
				PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);	// GSM Ring and Power Mon Interrupt

			}

			// Input Pins Interrupt Mask
			if (_Mask == 3) {

				// Set State
				if (_State) PCICR |= (1 << PCIE2);
				if (!_State) PCICR &= ~(1 << PCIE2);

				// Set Pin Change Interrupt Mask 2
				PCMSK2 |= (1 << PCINT16) | (1 << PCINT17) | (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);

			}

		}

		/**
		 * @brief FOTA MCU Power Switch Functions
		 * @param _State Switch State.
		 */
		void FOTA_Power(const bool _State) {

			// Turn FOTA Power Enable HIGH
			if (_State) PORTG |= 0b00000001;

			// Turn FOTA Power Enable LOW
			if (!_State) PORTG &= 0b11111110;

		}

		// Buzzer Functions
		void _bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration) {

			if(silentDuration==0) silentDuration=1;

			if(initFrequency < finalFrequency) {

				for (size_t i = initFrequency; i < finalFrequency; i = i * prop) _tone(i, noteDuration, silentDuration);

			} else {

				for (size_t i = initFrequency; i > finalFrequency; i = i / prop) _tone(i, noteDuration, silentDuration);

			}

		}
		void _tone (float noteFrequency, long noteDuration, int silentDuration) {

			if (silentDuration == 0) silentDuration = 1;

			// Play Tone
			tone(3, noteFrequency, noteDuration);

			// Delay Note
			delay(noteDuration);

			// Delay Silent
			delay(silentDuration);

		}

		// Rotary Functions
		void Get_Rotary(void) {
			
			this->Module.Setting.Filter_Count = 0b00001111 & PINA;

			// Print Diagnostic
			#ifdef TERMINAL
				Terminal.Text(_Console_Rotary_X, _Console_Rotary_Y, YELLOW, String(this->Module.Setting.Filter_Count));
			#endif

		}

		// Switch Functions
		void Get_HydroSyclone(void) {

			// Get Setting
			this->Module.Setting.HydroSyclone = bitRead(PINA,5);

			// Print Diagnostic
			#ifdef TERMINAL
				if (this->Module.Setting.HydroSyclone) {
					Terminal.Text(_Console_HydroSyclone_X, _Console_HydroSyclone_Y, CYAN, "X");
				} else {
					Terminal.Text(_Console_HydroSyclone_X, _Console_HydroSyclone_Y, CYAN, "-");
				}
			#endif

		}
		void Get_Stabilizer(void) {

			// Get Setting
			this->Module.Setting.Stabiliser = bitRead(PINA,4);

			// Print Diagnostic
			#ifdef TERMINAL
				if (this->Module.Setting.Stabiliser) {
					Terminal.Text(_Console_Stabilizer_X, _Console_Stabilizer_Y, CYAN, "X");
				} else {
					Terminal.Text(_Console_Stabilizer_X, _Console_Stabilizer_Y, CYAN, "-");
				}
			#endif

		}

	public:

		/**
		 * @brief B100xx Module Variables Structure Definition.
		 */
		struct Module_Variables {

			// Setting Variables
			struct Struct_Setting {
				uint8_t Filter_Count = 0;
				bool HydroSyclone = false;
				bool Stabiliser = false;
			} Setting;

			// AVR Timer Variable
			uint16_t Timer_Counter = 0;

			// SD Variables
			struct Struct_SD {
				bool Card_Sense = false;
				bool Status = false;
			} SD;

		} Module;

		/**
		 * @brief Construct a new Hardware object
		 */
		Hardware(void)  {

			// Set Module Pinout
			this->Set_Pinout();

			// Set Buzzer Sound Level to High
			PORTE &= 0b11100111;
			PORTE |= 0b00011000;

			// Control SD Card
			this->Module.SD.Card_Sense = bitRead(PINC, 1);

		}

		/**
		 * @brief Hardware boot function
		 */
		void Begin(void) {

			// Boot Delay
			delay(50);

			// Play Sound
			this->Buzzer(Boot);

			// Start Serial Communications
			#ifdef TERMINAL

				// Start Serial Communications
				Serial_Terminal.begin(115200);

				// Start Console Object
				Terminal.Begin(Serial_Terminal);

				// Draw B100 Console
				Terminal.FilterStat();

				// Print Firmware Version
				Terminal.Text(_Console_Version_X, _Console_Version_Y, CYAN, __PowerStat_Firmware__);

				// Print Hardware Version
				Terminal.Text(_Console_Version_X + 1, _Console_Version_Y, CYAN, __PowerStat_Hardware__);
				
			#endif

			// Start GSM Serial
			Serial_GSM.begin(115200);

			// Read Settings
			this->Get_Rotary();
			this->Get_HydroSyclone();
			this->Get_Stabilizer();

			// Connect SD
			this->SD_Connect();

			// Stop Interrupts
			cli();

			// Pin Change Interrupt Definations (RTC = 1, GSM = 2, PIN = 3)
			this->AVR_Pin_Interrupt(1, true);
			this->AVR_Pin_Interrupt(2, true);

			// Timer Interupt Definations
			this->AVR_Timer_1sn();

			// Start Interrupts
			sei();

		}

		/**
		 * @brief Buzzer control function
		 * @param soundName Sound name
		 */
		void Buzzer(int soundName) {
		
			// If Buzzer is Active
			#ifdef BUZZER
				
				switch(soundName) {

					case Boot:
						_tone(NOTE_E5,50,30);
						_tone(NOTE_E6,55,25);
						_tone(NOTE_A6,60,10);
						break;

					case S_DISCONNECTION:
						_tone(NOTE_E5,50,30);
						_tone(NOTE_A6,55,25);
						_tone(NOTE_E6,50,60);
						break;

					case S_BUTTON_PUSHED:
						_bendTones (NOTE_E6, NOTE_G6, 1.03, 20, 2);
						delay(30);
						_bendTones (NOTE_E6, NOTE_D7, 1.04, 10, 2);
						break;

					case S_MODE1:
						_bendTones (NOTE_E6, NOTE_A6, 1.02, 30, 10);    //1318.51 to 1760
						break;

					case S_MODE2:
						_bendTones (NOTE_G6, NOTE_D7, 1.03, 30, 10);    //1567.98 to 2349.32
						break;

					case S_MODE3:
						_tone(NOTE_E6,50,100);   //D6
						_tone(NOTE_G6,50,80);    //E6
						_tone(NOTE_D7,300,0);    //G6
						break;

					case S_SURPRISE:
						_bendTones(800, 2150, 1.02, 10, 1);
						_bendTones(2149, 800, 1.03, 7, 1);
						break;

					case S_JUMP:
						_bendTones(880, 2000, 1.04, 8, 3);   //A5 = 880
						delay(200);
						break;

					case S_OHOOH:
						_bendTones(880, 2000, 1.04, 8, 3);   //A5 = 880
						delay(200);
						for (int i=880; i<2000; i=i*1.04) _tone(NOTE_B5,5,10);
						break;

					case S_OHOOH2:
						_bendTones(1880, 3000, 1.03, 8, 3);
						delay(200);
						for (int i=1880; i<3000; i=i*1.03) _tone(NOTE_C6,10,10);
						break;

					case S_CUDDLY:
						_bendTones(700, 900, 1.03, 16, 4);
						_bendTones(899, 650, 1.01, 18, 7);
						break;

					case S_SLEEPING:
						_bendTones(100, 500, 1.04, 10, 10);
						delay(500);
						_bendTones(400, 100, 1.04, 10, 1);
						break;

					case S_HAPPY:
						_bendTones(1500, 2500, 1.05, 20, 8);
						_bendTones(2499, 1500, 1.05, 25, 8);
						break;

					case S_SUPER_HAPPY:
						_bendTones(2000, 6000, 1.05, 8, 3);
						delay(50);
						_bendTones(5999, 2000, 1.05, 13, 2);
						break;

					case S_HAPPY_SHORT:
						_bendTones(1500, 2000, 1.05, 15, 8);
						delay(100);
						_bendTones(1900, 2500, 1.05, 10, 8);
						break;

					case S_SAD:
						_bendTones(880, 669, 1.02, 20, 200);
						break;

					case S_CONFUSED:
						_bendTones(1000, 1700, 1.03, 8, 2);
						_bendTones(1699, 500, 1.04, 8, 3);
						_bendTones(1000, 1700, 1.05, 9, 10);
						break;

					case S_FART1:
						_bendTones(1600, 3000, 1.02, 2, 15);
						break;

					case S_FART2:
						_bendTones(2000, 6000, 1.02, 2, 20);
						break;

					case S_FART3:
						_bendTones(1600, 4000, 1.02, 2, 20);
						_bendTones(4000, 3000, 1.02, 2, 20);
						break;
						
				}
				
			#endif

		}

		// LED Functions
		void LED(const uint8_t _Color, const uint8_t _Blink, const uint16_t _Interval) {

			// If Lights are active
			#ifdef LIGHTS

				// Red Color
				if (_Color == __RED__) {

					// Blink
					for (size_t i = 0; i < _Blink; i++) {

						// Turn ON Red LED
						PORTD |= 0b10000000;

						// Delay
						delay(_Interval);

						// Turn OFF Red LED
						PORTD &= 0b01111111;

						// Delay
						delay(_Interval);

					}

				}
				
				// Green Color
				if (_Color == __GREEN__) {

					// Blink
					for (size_t i = 0; i < _Blink; i++) {

						// Turn ON Green LED
						PORTD |= 0b01000000;

						// Delay
						delay(_Interval);

						// Turn OFF Green LED
						PORTD &= 0b10111111;

						// Delay
						delay(_Interval);

					}

				}

				// Green Color
				if (_Color == __BLUE__) {

					// Blink
					for (size_t i = 0; i < _Blink; i++) {

						// Turn ON Blue LED
						PORTD |= 0b00100000;

						// Delay
						delay(_Interval);

						// Turn OFF Blue LED
						PORTD &= 0b11011111;

						// Delay
						delay(_Interval);

					}

				}

				// Purple Color
				if (_Color == __PURPLE__) {

					// Blink
					for (size_t i = 0; i < _Blink; i++) {

						// Turn ON Purple LED
						PORTD |= 0b10100000;

						// Delay
						delay(_Interval);

						// Turn OFF Purple LED
						PORTD &= 0b01011111;

						// Delay
						delay(_Interval);

					}

				}

				// White Color
				if (_Color == __WHITE__) {

					// Blink
					for (size_t i = 0; i < _Blink; i++) {

						// Turn ON White LED
						PORTD |= 0b11100000;

						// Delay
						delay(_Interval);

						// Turn OFF White LED
						PORTD &= 0b01111111;

						// Delay
						delay(_Interval);

					}

				}

				// Turn OFF all LED
				PORTD &= 0b00011111;

			#endif

		}

		// Timer Functions
		void Timer_Count(void) {

			// Set Timer Counter
			this->Module.Timer_Counter += 1;

			// Handle Max
			if (this->Module.Timer_Counter == 65534) this->Module.Timer_Counter = 0;

		}
		bool Timer_Control(const uint8_t _Interval) {

			// Timer Counter
			if (this->Module.Timer_Counter % _Interval == 0) {

				// End Function
				return(true);

			} else {

				// End Function
				return(false);

			}

		}

};