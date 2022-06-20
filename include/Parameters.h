// Include Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

/**
 * @brief Main routine variable structure definition.
 */
struct FilterStat_Struct {

	/**
	 * @brief Interrupt variables.
	 */
	struct Interrupt_Struct {

		/**
		 * @brief Main interrupt enable variable.
		 */
		bool Interrupt_Enable = false;

		/**
		 * @brief Pressure measurement interrupt variable.
		 */
		bool Pressure_Measurement = false;

		/**
		 * @brief Status handle interrupt variable.
		 */
		bool Handle_Status = false;

		/**
		 * @brief Clean prosedure interrupt variable.
		 */
		bool Handle_Clean = false;

		/**
		 * @brief Time update interrupt variable.
		 */
		bool UNIX_Time_Update = false;

		/**
		 * @brief Timed prosedure interrupt variable. 
		 */
		bool RTC_Timer_Interrupt = false;

		/**
		 * @brief GSM ring interrupt variable.
		 */
		bool GSM_Interrupt = false;

	} Interrupt;

	/**
	 * @brief GSM variable structrue.
	 */
	struct IoT_Struct {

		/**
		 * @brief Server API IP constant.
		 */
		char Server_IP[15] = "54.216.226.171";

		/**
		 * @brief Server API endpoint constant.
		 */
		char Server_EndPoint[15] = "/api/v1.1/p402";

		/**
		 * @brief GSM Onlive variable.
		 */
		bool Online = false;

		/**
		 * @brief GSM remote command variable.
		 */
		uint16_t Remote_Command = 0;

		/**
		 * @brief IoT JSON variable.
		 */
		char JSON[650];

	} IoT;

} FilterStat;

/**
 * @brief Hardware library object definition.
 */
Hardware B100BB_Hardware;

/**
 * @brief I2C library object definition.
 */
I2C B100BB_I2C;

/**
 * @brief AI library object definition.
 */
AI B100BB_AI;

/**
 * @brief LCD library object definition.
 */
#ifdef HMI
	dwin LCD;
#endif

/**
 * @brief GSM library object definition.
 */
Telit_xE910 GSM;

/**
 * @brief Incomming Data Socket library object definition.
 */
GSM_Socket_Incomming Incomming_Socket((uint8_t)2);

/**
 * @brief Outgoing Data library object definition.
 */
GSM_Socket_Outgoing Outgoing_Socket((uint8_t)3, FilterStat.IoT.Server_IP, FilterStat.IoT.Server_EndPoint);
