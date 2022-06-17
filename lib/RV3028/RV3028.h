#ifndef __RV3028__
#define __RV3028__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define I2C Functions Library
#ifndef __I2C_Functions__
#include <I2C_Functions.h>
#endif

// I2C Library Definitions
#define __I2C_RV3028_Version__	"01.02.01"

class RV3028 : public I2C_Functions {

	private:


	public:

		/**
		 * @brief Construct a new RV3028 object
		 * @param _Multiplexer_Enable I2C device multiplexer enable
		 * @param _Multiplexer_Channel I2C device multiplexer channel
		 */
		RV3028(bool _Multiplexer_Enable, uint8_t _Multiplexer_Channel) : I2C_Functions(__I2C_Addr_RV3028C7__, _Multiplexer_Enable, _Multiplexer_Channel) {


		}

		/**
		 * @brief Disable trickle charger function.
		 */
		void Disable_Trickle_Charger(void) {

			// Clear Register Bit
			Clear_Register_Bit(0x37, 5, true);

		}

		/**
		 * @brief Set 24h hour format function.
		 */
		void Set_24h_Clock(void) {

			// Clear Register Bit
			Clear_Register_Bit(0x10, 1, true);

		}

		/**
		 * @brief Clear UNIX time function.
		 */
		void Clear_UNIX_Time(void) {

			// Write Variable
			Write_Register(0x1B, 0x00, true);
			Write_Register(0x1C, 0x00, true);
			Write_Register(0x1D, 0x00, true);
			Write_Register(0x1E, 0x00, true);

		}		

		/**
		 * @brief Get RTC second
		 * @return uint8_t Second value
		 */
		inline uint8_t Get_Second(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x00);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC minute
		 * @return uint8_t Minute value
		 */
		inline uint8_t Get_Minute(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x01);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC hour
		 * @return uint8_t Hour value
		 */
		inline uint8_t Get_Hour(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x02);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC day of week
		 * @return uint8_t Day of week value
		 */
		inline uint8_t Get_Week_Day(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x03);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC day
		 * @return uint8_t Day value
		 */
		inline uint8_t Get_Date(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x04);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC month
		 * @return uint8_t Month value
		 */
		inline uint8_t Get_Month(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x05);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Get RTC year
		 * @return uint8_t Year value
		 */
		inline uint8_t Get_Year(void) {

			// Read Register
			uint8_t _Raw_Data = Read_Register(0x06);

			// End Function
			return (this->BCDtoDEC(_Raw_Data));

		}

		/**
		 * @brief Set RTC time.
		 * @param _Second Second value
		 * @param _Minute Minute value
		 * @param _Hour Hour value
		 * @param _Date Date value
		 * @param _Month Month value
		 * @param _Year Year value
		 */
		void Set_Time(uint8_t _Second, uint8_t _Minute, uint8_t _Hour, uint8_t _Date, uint8_t _Month, uint8_t _Year) {

			// Calculate Day of Week
			uint8_t _DayofWeek = this->Day_of_Week(_Date, _Month, _Year);

			// Set Time
			Write_Register(0x06, this->DECtoBCD(_Year), true);
			Write_Register(0x05, this->DECtoBCD(_Month), true);
			Write_Register(0x04, this->DECtoBCD(_Date), true);
			Write_Register(0x03, this->DECtoBCD(_DayofWeek), true);
			Write_Register(0x02, this->DECtoBCD(_Hour), true);
			Write_Register(0x01, this->DECtoBCD(_Minute), true);
			Write_Register(0x00, this->DECtoBCD(_Second), true);

		}

		/**
		 * @brief Get UNIX time value.
		 * @return uint32_t UNIX time value.
		 */
		uint32_t Get_UNIX_Time(void) {
			
			// Declare Variable
			uint8_t _Raw_Array[4];

			// Read Variable
			Read_Multiple_Register(0x1B, _Raw_Array, 4, true);

			// Calculate Time
			uint32_t _UNIX_Time = ((uint32_t)_Raw_Array[3] << 24) | ((uint32_t)_Raw_Array[2] << 16) | ((uint32_t)_Raw_Array[1] << 8) | _Raw_Array[0];

			// End Function
			return(_UNIX_Time);

		}

		/**
		 * @brief Enable timer
		 */
		void Enable_Timer(void) {

			// Clear Bit
			Set_Register_Bit(0x0F, 2, true);

		}

		/**
		 * @brief Set RTC timer
		 * @param _Repeat 
		 * @param _Frequency 
		 * @param _Value 
		 * @param _Interrupt 
		 * @param _Start 
		 * @param _Clock_Output 
		 */
		void Set_Timer(bool _Repeat, uint16_t _Frequency, uint16_t _Value, bool _Interrupt, bool _Start, bool _Clock_Output) {

			// Disable Timer
			Clear_Register_Bit(0x0F, 2, true);

			// Disable Interrupt
			Clear_Register_Bit(0x10, 4, true);

			// Clear Flag
			Clear_Register_Bit(0x0E, 3, true);

			// Set Timer Value
			Write_Register(0x0A, (_Value & 0xFF), true);
			Write_Register(0x0B, (_Value >> 8), true);

			// Read Control Register
			uint8_t _CONTROL1 = Read_Register(0x0F);

			// Set Timer Repeat Bit
			if (_Repeat) {_CONTROL1 |= 1 << 7;} else {_CONTROL1 &= ~(1 << 7);}

			// Set Timer Frequency
			switch (_Frequency) {
			
				case 4096:
					_CONTROL1 &= ~3;
					break;

				case 64:
					_CONTROL1 &= ~3;
					_CONTROL1 |= 1;
					break;

				case 1:
					_CONTROL1 &= ~3;
					_CONTROL1 |= 2;
					break;

				case 60000:
					_CONTROL1 |= 3;
					break;

			}

			// Set Interrupt
			if (_Interrupt) Set_Register_Bit(0x10, 4, true);

			// Start Timer
			if (_Start) _CONTROL1 |= (1 << 2);

			// Write Register
			Write_Register(0x0F, _CONTROL1, true);

			// Clock Out
			if (_Clock_Output) Set_Register_Bit(0x12, 1, true);
			if (!_Clock_Output) Clear_Register_Bit(0x12, 1, true);

		}

		/**
		 * @brief Day of week calculation function.
		 * @param _Day 
		 * @param _Month 
		 * @param _Year 
		 * @return uint8_t 
		 */
		uint8_t Day_of_Week(uint8_t _Day, uint8_t _Month, uint16_t _Year) {

			// Declare Variables
			int _mTable;
			int _SummedDate;
			int _DoW;
			int _Leap;

			// Leap Year Calculation
			if ((fmod(_Year, 4) == 0 and fmod(_Year, 100) != 0) or (fmod(_Year, 400) == 0)) {
				_Leap = 1;
			} else {
				_Leap = 0;
			}

			// Limit results to year 1900-2299 (to save memory)
			while (_Year > 2299) {
				_Year = _Year - 400;
			}
			while (_Year < 1900) {
				_Year = _Year + 400;
			}

			// Calculating century
			int _Century = _Year / 100;
			int _Century_Table;

			// Calculating two digit year
			uint8_t _Year_Short = fmod(_Year, 100);

			// Century value based on Table
			if (_Century == 19) _Century_Table = 1;
			if (_Century == 20) _Century_Table = 0;
			if (_Century == 21) _Century_Table = 5;
			if (_Century == 22) _Century_Table = 3;

			// Jan and Feb calculations affected by leap years
			if (_Month == 1) {
				if (_Leap == 1) { 
					_mTable = 6;
				} else { 
					_mTable = 0; 
				}
			}
			if (_Month == 2) { 
				if (_Leap == 1) { 
					_mTable = 2;
				} else { 
					_mTable = 3; 
				}
			}

			// Other months not affected and have set values
			if(_Month == 10) _mTable = 0;
			if(_Month == 8) _mTable = 2;
			if(_Month == 3 or _Month == 11) _mTable = 3;
			if(_Month == 4 or _Month == 7) _mTable = 6;
			if(_Month == 5) _mTable = 1;
			if(_Month == 6) _mTable = 4;
			if(_Month == 9 or _Month == 12) _mTable = 5;

			// Enter the data into the formula
			_SummedDate = _Day + _mTable + _Year_Short + (_Year_Short / 4) + _Century_Table;
			
			// Find remainder
			_DoW = fmod(_SummedDate,7);

			// End Function
			return(_DoW);

		}

		/**
		 * @brief Binary to decimal convert function.
		 * @param _Value Binary value
		 * @return uint8_t Decimal value
		 */
		uint8_t BCDtoDEC(uint8_t _Value) {

			// Define Variable
			uint8_t _OutPut;

			// Calculate Value
			_OutPut = ((_Value / 16 * 10) + (_Value % 16));

			// End Function
			return(_OutPut);

		}

		/**
		 * @brief Decimal to binary convert function.
		 * @param _Value Decimal value
		 * @return uint8_tBinary value
		 */
		uint8_t DECtoBCD(byte _Value) {

			// Define Variable
			uint8_t _OutPut;

			// Calculate Value
			_OutPut = ((_Value / 10 * 16) + (_Value % 10));

			// End Function
			return(_OutPut);

		}

};

#endif /* defined(__RV3028__) */
