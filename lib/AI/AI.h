// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define HMI Object
#ifndef __dWin__
#include <dWin.h>
#endif

// Define Statistical Library
#ifndef __Statistical__
#include <Statistical.h>
#endif

// Define Objects
dwin LCD_AI;
Stream_Stats<float> Pressure_Input_Stats;
Stream_Stats<float> Pressure_Output_Stats;
Stream_Stats<float> Pressure_Delta_Stats;

class AI {

	private:

		/**
		 * @brief Pressure measurement and calculation function
		 */
		void Measure_Pressure(void) {

			// Measure Input Pressure
			if (this->Parameters.Sensor_Sense.Input) {

				// Set Object
				Analog Pressure_Sensor_Input(__Pressure_Channel_Input__, __Pressure_Measure_Count__, true, __Pressure_Calibration_a__, __Pressure_Calibration_b__);

				// Measure Pressure
				this->Parameters.Pressure.Input = (float)((float)round(Pressure_Sensor_Input.Read() * 1000) / 1000) ;

				// Pressure Stats
				Pressure_Input_Stats.Add(this->Parameters.Pressure.Input);

				// Command Delay
				delay(10);

			} else {

				// Measure Pressure
				this->Parameters.Pressure.Input = 0;

			}

			// Measure Output Pressure
			if (this->Parameters.Sensor_Sense.Output) {

				// Set Object
				Analog Pressure_Sensor_Output(__Pressure_Channel_Output__, __Pressure_Measure_Count__, true, __Pressure_Calibration_a__, __Pressure_Calibration_b__);

				// Measure Pressure
				this->Parameters.Pressure.Output = (float)((float)round(Pressure_Sensor_Output.Read() * 1000) / 1000) ;

				// Pressure Stats
				Pressure_Output_Stats.Add(this->Parameters.Pressure.Output);

				// Command Delay
				delay(10);

			} else {

				// Measure Pressure
				this->Parameters.Pressure.Output = 0;

			}

			// Calculate Delta Pressure
			if (this->Parameters.Sensor_Sense.Input and this->Parameters.Sensor_Sense.Output) {

				// Set Variable
				this->Parameters.Pressure.Delta = (float)((float)round((this->Parameters.Pressure.Input - this->Parameters.Pressure.Output) * 1000) / 1000);

				// Pressure Stats
				Pressure_Delta_Stats.Add(this->Parameters.Pressure.Delta);

			} else {

				// Set Variable
				this->Parameters.Pressure.Delta = 0;

			}

			// Terminal Update
			#ifdef TERMINAL

				// Print Input Pressure
				if (this->Parameters.Pressure.Input < 0) {
					Terminal.Text(34, 109, RED, String(-1 * this->Parameters.Pressure.Input, 3));
				} else {
					Terminal.Text(34, 109, CYAN, String(this->Parameters.Pressure.Input, 3));
				}

				// Print Output Pressure
				if (this->Parameters.Pressure.Output < 0) {
					Terminal.Text(35, 109, RED, String(-1 * this->Parameters.Pressure.Output, 3));
				} else {
					Terminal.Text(35, 109, CYAN, String(this->Parameters.Pressure.Output, 3));
				}

				// Print Delta Pressure
				if (this->Parameters.Pressure.Delta < 0) {
					Terminal.Text(32, 109, RED, String(-1 * this->Parameters.Pressure.Delta, 3));
				} else {
					Terminal.Text(32, 109, CYAN, String(this->Parameters.Pressure.Delta, 3));
				}

			#endif

		}

	public:

		// I2C Network Diagnostic Variables
		struct Struct_Filter {

			// Sensor Structure
			struct Struct_Sensor {
				bool Input;
				bool Output;
			} Sensor_Sense;

			// Pressure Structure
			struct Struct_Pressure {
				float Input;
				float Output;
				float Delta;
			} Pressure;

			// Status Structure
			struct Struct_Status {
				uint8_t Device;
				bool Irrigation = false;
				uint8_t Clean_Type = 0;
			} Status;

			// Irrigation Structure
			struct Struct_Irrigation {

				// Limit Structure
				struct Struct_Limit {
					float Start_Limit = 0.2;
					float End_Limit = 0.2;
				} Limit;

				// Time Structure
				struct Struct_Time_Limit {
					char Start[30];
					char End[30];
				} Time;

			} Irrigation;

			// Clean Structure
			struct Struct_Clean {

				// Clean Process
				bool Process = false;

				// Limit Structure
				struct Struct_Limit {
					float Auto_Clean_Start = 0.5;
					uint16_t Interval = 60;
					uint16_t On_Interval = 30;
					uint16_t Off_Interval = 15;
				} Defaults;

				// Pressure Structure
				struct Struct_Pressure_Array {
					float Input[16];
					float Output[16];
				} Pressure_Array;

				// Authorization Structure
				struct Struct_Authorization {
					bool Auto = false;
					bool Timed = false;
					bool Manual = false;
					bool Remote = false;
				} Authorization;

				// Time Structure
				struct Struct_Time {
					char Start[25];
					char End[25];
				} Time;

			} Clean;

		} Parameters;

		/**
		 * @brief Construct a new AI object
		 * @param _Input_Sensor Sensor Sense
		 * @param _Output_Sensor Sensor Sense
		 */
		AI(void) {

			// Control Pressure Sensor
			this->Parameters.Sensor_Sense.Input = bitRead(PINF, 0);
			this->Parameters.Sensor_Sense.Output = bitRead(PINF, 3);

		}

		/**
		 * @brief Begin AI library.
		 */
		void Begin(void) {


		}

		/**
		 * @brief Handle filter status
		 */
		void Handle_Status(void) {

			// Measure Pressure
			this->Measure_Pressure();

			// Decide Status
			if ((this->Parameters.Pressure.Input >= 1.5) and (this->Parameters.Pressure.Output >= 0.3) and (this->Parameters.Pressure.Input < 9) and (this->Parameters.Pressure.Input > this->Parameters.Pressure.Output) and (this->Parameters.Pressure.Delta > this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Output >= 1.5)) {
				
				// Set Status
				this->Parameters.Status.Device = 100;
				
				// Set Clean Control Parameters
				this->Parameters.Clean.Authorization.Auto = true;
				this->Parameters.Clean.Authorization.Timed = true;
				this->Parameters.Clean.Authorization.Manual = true;
				this->Parameters.Clean.Authorization.Remote = true;
				
				
			} else {

				// Status 101
				if ((this->Parameters.Pressure.Input > 0.1) and (this->Parameters.Pressure.Output < 0.3) and (this->Parameters.Pressure.Input < 9) and (this->Parameters.Pressure.Input > this->Parameters.Pressure.Output)) {

					// Set Status
					this->Parameters.Status.Device = 101;

					// Set Clean Control Parameters
					this->Parameters.Clean.Authorization.Auto = false;
					this->Parameters.Clean.Authorization.Timed = false;
					this->Parameters.Clean.Authorization.Manual = false;
					this->Parameters.Clean.Authorization.Remote = false;
					
				} else {

					// Status 102
					if ((this->Parameters.Pressure.Output >= 0.3) and (this->Parameters.Pressure.Input < 9) and (this->Parameters.Pressure.Input > this->Parameters.Pressure.Output ) and (this->Parameters.Pressure.Delta> this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Output < 1.5)) {

						// Set Status
						
						this->Parameters.Status.Device = 102;

						// Set Clean Control Parameters
						this->Parameters.Clean.Authorization.Auto = true;
						this->Parameters.Clean.Authorization.Timed = true;
						this->Parameters.Clean.Authorization.Manual = true;
						this->Parameters.Clean.Authorization.Remote = true;
								
					} else {

						// Status 103
						if ((this->Parameters.Pressure.Input != 0) and (this->Parameters.Pressure.Output != 0) and (this->Parameters.Pressure.Input < 9) and (this->Parameters.Pressure.Input > 1.5) and (this->Parameters.Pressure.Output >= 1.5) and (this->Parameters.Pressure.Delta <= this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Input > this->Parameters.Pressure.Output)) {

							// Set Status
							this->Parameters.Status.Device = 103;

							// Set Clean Control Parameters
							this->Parameters.Clean.Authorization.Auto = true;
							this->Parameters.Clean.Authorization.Timed = true;
							this->Parameters.Clean.Authorization.Manual = true;
							this->Parameters.Clean.Authorization.Remote = true;
										
						} else {

							// Status 104
							if ((this->Parameters.Pressure.Input != 0) and (this->Parameters.Pressure.Output != 0) and (this->Parameters.Pressure.Input < 9) and (this->Parameters.Pressure.Input > 1.5) and (this->Parameters.Pressure.Output >= 1.5) and (this->Parameters.Pressure.Delta <= this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Input == this->Parameters.Pressure.Output)) {

								// Set Status
								this->Parameters.Status.Device = 104;

								// Set Clean Control Parameters
								this->Parameters.Clean.Authorization.Auto = false;
								this->Parameters.Clean.Authorization.Timed = true;
								this->Parameters.Clean.Authorization.Manual = false;
								this->Parameters.Clean.Authorization.Remote = false;
												
							} else {

								// Status 105
								if ((this->Parameters.Pressure.Output < 1.5) and (this->Parameters.Pressure.Input < 1.5) and (this->Parameters.Pressure.Delta <= this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Input == this->Parameters.Pressure.Output) and (this->Parameters.Pressure.Output > 0.3)) {

									// Set Status
									this->Parameters.Status.Device = 105;

									// Set Clean Control Parameters
									this->Parameters.Clean.Authorization.Auto = true;
									this->Parameters.Clean.Authorization.Timed = true;
									this->Parameters.Clean.Authorization.Manual = true;
									this->Parameters.Clean.Authorization.Remote = true;

									} else {

										// Status 106
										if ((this->Parameters.Pressure.Output < 1.5) and (this->Parameters.Pressure.Input < 1.5) and (this->Parameters.Pressure.Delta<= this->Parameters.Clean.Defaults.Auto_Clean_Start) and (this->Parameters.Pressure.Input > this->Parameters.Pressure.Output ) and (this->Parameters.Pressure.Output > 0.3)) {

											// Set Status
											this->Parameters.Status.Device = 106;

											// Set Clean Control Parameters
											this->Parameters.Clean.Authorization.Auto = true;
											this->Parameters.Clean.Authorization.Timed = true;
											this->Parameters.Clean.Authorization.Manual = true;
											this->Parameters.Clean.Authorization.Remote = true;

										} else {

											// Status 107
											if ((this->Parameters.Pressure.Input >= 0.1) and (this->Parameters.Pressure.Output >= 0.1) and (this->Parameters.Pressure.Output > this->Parameters.Pressure.Input)) {

												// Set Status
												this->Parameters.Status.Device = 107;

												// Set Clean Control Parameters
												this->Parameters.Clean.Authorization.Auto = false;
												this->Parameters.Clean.Authorization.Timed = true;
												this->Parameters.Clean.Authorization.Manual = true;
												this->Parameters.Clean.Authorization.Remote = true;

											} else {

												// Status 108
												if ((this->Parameters.Pressure.Input < 0.1) and (this->Parameters.Pressure.Output >= 0.1) and (this->Parameters.Pressure.Output > this->Parameters.Pressure.Input)) {

													// Set Status
													this->Parameters.Status.Device = 108;

													// Set Clean Control Parameters
													this->Parameters.Clean.Authorization.Auto = false;
													this->Parameters.Clean.Authorization.Timed = true;
													this->Parameters.Clean.Authorization.Manual = true;
													this->Parameters.Clean.Authorization.Remote = true;

												} else {

													// Status 109
													if ((this->Parameters.Pressure.Input >= 9) and (this->Parameters.Pressure.Delta <= this->Parameters.Clean.Defaults.Auto_Clean_Start)) {

														// Set Status
														this->Parameters.Status.Device = 109;

														// Set Clean Control Parameters
														this->Parameters.Clean.Authorization.Auto = false;
														this->Parameters.Clean.Authorization.Timed = true;
														this->Parameters.Clean.Authorization.Manual = false;
														this->Parameters.Clean.Authorization.Remote = false;

													} else {

														// Status 110
														if ((this->Parameters.Pressure.Input >= 9) and (this->Parameters.Pressure.Delta > this->Parameters.Clean.Defaults.Auto_Clean_Start)) {

															// Set Status
															this->Parameters.Status.Device = 110;

															// Set Clean Control Parameters
															this->Parameters.Clean.Authorization.Auto = true;
															this->Parameters.Clean.Authorization.Timed = true;
															this->Parameters.Clean.Authorization.Manual = true;
															this->Parameters.Clean.Authorization.Remote = true;

														} else {

															// Set IDLE Status
															if (this->Parameters.Status.Irrigation) this->Parameters.Status.Device = 200;
															if (!this->Parameters.Status.Irrigation) this->Parameters.Status.Device = 0;
															
														}
														
													}
													
												}
												
											}
											
										}
										
									}
								
							}
							
						}
						
					}

				}

			}

			// Terminal Update
			#ifdef TERMINAL

				// Print Device Status
				Terminal.Text(40, 114, GREEN, String(this->Parameters.Status.Device));

				// Print Irrigation Status
				Terminal.Text(43, 116, (this->Parameters.Status.Irrigation ? CYAN : RED), (this->Parameters.Status.Irrigation ? "X" : "-"));

				// Print Irrigation Status Text
				if (this->Parameters.Status.Clean_Type == 0) Terminal.Text(48, 105, (this->Parameters.Status.Irrigation ? GREEN : RED), F("IRRIGATION"));

				// Print Clean Text
				if (this->Parameters.Status.Clean_Type == 1) Terminal.Text(48, 105, YELLOW, "   Auto   ");
				if (this->Parameters.Status.Clean_Type == 2) Terminal.Text(48, 105, YELLOW, "  Timed   ");

			#endif

		}

};