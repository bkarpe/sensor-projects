#ifndef _SKIP_COMMUNICATION_EXT_H_
#define _SKIP_COMMUNICATION_EXT_H_

/***************************************************************************************************/
// Go! Link is also known as Skip.
// Go! Temp is also known as Jonah.
// Go! Motion is also known as Cyclops.
//
// This file contains declarations for parameter and response structures used by the Skip support
// function SendCmdAndGetResponse().
//
// Skip, Jonah, and Cyclops use the same basic protocol. Not all commands are supported by all devices.
// If a command is not supported, then SendCmdAndGetResponse() will fail and return -1.
//
/***************************************************************************************************/

//
// The currently defined commands are:
//
#define SKIP_CMD_ID_GET_STATUS 0x10
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_1BYTE 0x11
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_2BYTES 0x12
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_3BYTES 0x13
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_4BYTES 0x14
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_5BYTES 0x15
#define SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_6BYTES 0x16
#define SKIP_CMD_ID_READ_LOCAL_NV_MEM 0x17
#define SKIP_CMD_ID_START_MEASUREMENTS 0x18
#define SKIP_CMD_ID_STOP_MEASUREMENTS 0x19
#define SKIP_CMD_ID_INIT 0x1A
#define SKIP_CMD_ID_SET_MEASUREMENT_PERIOD 0x1B
#define SKIP_CMD_ID_GET_MEASUREMENT_PERIOD 0x1C
#define SKIP_CMD_ID_SET_LED_STATE 0x1D
#define SKIP_CMD_ID_GET_LED_STATE 0x1E
#define SKIP_CMD_ID_GET_SERIAL_NUMBER 0x20
//Commands defined above are supported by Skip, Jonah, and Cyclops, except that Cyclops does not support the serial # or the NV_MEM cmds.
//Skip extensions:
#define SKIP_CMD_ID_SET_VIN_OFFSET_DAC 0x1F
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_1BYTE 0x21
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_2BYTES 0x22
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_3BYTES 0x23
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_4BYTES 0x24
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_5BYTES 0x25
#define SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_6BYTES 0x26
#define SKIP_CMD_ID_READ_REMOTE_NV_MEM 0x27
#define SKIP_CMD_ID_GET_SENSOR_ID 0x28
#define SKIP_CMD_ID_SET_ANALOG_INPUT_CHANNEL 0x29
#define SKIP_CMD_ID_GET_ANALOG_INPUT_CHANNEL 0x2A
#define SKIP_CMD_ID_GET_VIN_OFFSET_DAC 0x2B
#define SKIP_CMD_ID_SPARE1 0x2C
#define SKIP_CMD_ID_SPARE2 0x2D
#define SKIP_CMD_ID_SPARE3 0x2E
#define SKIP_CMD_ID_SPARE4 0x2F
#define FIRST_SKIP_CMD_ID SKIP_CMD_ID_GET_STATUS
#define LAST_SKIP_CMD_ID SKIP_CMD_ID_SPARE4

//
/***************************************************************************************************/
//

#if defined (TARGET_OS_WIN)
#pragma pack(push)
#pragma pack(1)
#endif

#ifdef TARGET_OS_MAC
#pragma pack(1)
#endif

// The structures below define the parameter and response blocks associated with the commands defined above.
// Pointers to these structures are passed in to SendCmdAndGetResponse().
// If no parameter structure is defined for a command, then the command does not take parameters.
// If no response structure is defined for a command, then the only response associated with the command is an indication of
// success or failure. The exceptions to this rule are the SKIP_CMD_ID_READ_* commands, whose responses are simply byte arrays.

typedef struct
{
	unsigned char addr;
	unsigned char payload[6];
} GSkipWriteI2CMemParams; //Used with SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_1BYTE .. SKIP_CMD_ID_WRITE_LOCAL_NV_MEM_6BYTES .. SKIP_CMD_ID_WRITE_REMOTE_NV_MEM_6BYTES.

typedef struct
{
	unsigned char addr;
	unsigned char count;
} GSkipReadI2CMemParams; //Parameter block passed into SendCmd() with SKIP_CMD_ID_READ_LOCAL_NV_MEM and SKIP_CMD_ID_READ_REMOTE_NV_MEM.


typedef struct
{
	unsigned char reportErrorWhilePoweringUpFlag; //If 1, then send back error if the device is still powering up.
												  //If zero, then the SKIP_CMD_ID_INIT is just ignored while the device is powering up.
} GSkipInitParams;				//Parameter block passed into SendCmd() with SKIP_CMD_ID_INIT for Go Link.
//The GSkipInitParams parameter block is not supported by any Go devices except Go Link, and only Go Links with versions >= 1.6100 .
//Even with new Go Links(ver >= 1.6100), this parameter block is optional. If GSkipInitParams are not sent to Go Link,
//then the SKIP_CMD_ID_INIT command is just ignored while the device is powering up.
//The device is able to respond while it is still powering up because USB communication is established before the Go Link finishes
//powering up its slave processor. All commands other than SKIP_CMD_ID_INIT are always ignored while the device is powering up.
#define SKIP_TIMEOUT_MS_CMD_ID_INIT_WO_BUSY_STATUS 6000
#define SKIP_TIMEOUT_MS_CMD_ID_INIT_WITH_BUSY_STATUS 400
//Go Link responds fast enough to set SKIP_TIMEOUT_MS_CMD_ID_INIT_WITH_BUSY_STATUS to 100. PC's can be slow, so we use 400.

typedef struct
{
	unsigned char lsbyteLswordMeasurementPeriod;//Units are 'ticks'. Length of tick is different for Skip versus Jonah.
	unsigned char msbyteLswordMeasurementPeriod;
	unsigned char lsbyteMswordMeasurementPeriod;
	unsigned char msbyteMswordMeasurementPeriod;
} GSkipSetMeasurementPeriodParams; //Parameter block passed into SendCmd() with SKIP_CMD_ID_SET_MEASUREMENT_PERIOD.

typedef GSkipSetMeasurementPeriodParams GSkipGetMeasurementPeriodCmdResponsePayload;//This is the response payload returned by GetNextResponse()
																					//after sending SKIP_CMD_ID_GET_MEASUREMENT_PERIOD.

/***************************************************************************************************/
// Some redundant LED declarations:
#define SKIP_LED_COLOR_BLACK 0xC0
#define SKIP_LED_COLOR_RED 0x40
#define SKIP_LED_COLOR_GREEN 0x80
#define SKIP_LED_COLOR_RED_GREEN 0
#define SKIP_LED_BRIGHTNESS_MIN 0
#define SKIP_LED_BRIGHTNESS_MAX 0x10
typedef enum
{
	kLEDSettings_Off,
	kLEDSettings_Red,
	kLEDSettings_Green,
	kLEDSettings_Orange
} ELEDSettings;

static const unsigned char kLEDOff = SKIP_LED_COLOR_BLACK;
static const unsigned char kLEDRed = SKIP_LED_COLOR_RED;
static const unsigned char kLEDGreen = SKIP_LED_COLOR_GREEN;
static const unsigned char kLEDOrange = SKIP_LED_COLOR_RED_GREEN;
static const unsigned char kSkipMaxLedBrightness = SKIP_LED_BRIGHTNESS_MAX;
static const unsigned char kSkipOrangeLedBrightness = 4;

typedef struct
{
	unsigned char color;
	unsigned char brightness;
} GSkipSetLedStateParams; //Parameter block passed into SendCmd() with SKIP_CMD_ID_SET_LED_STATE.

typedef GSkipSetLedStateParams GSkipGetLedStateCmdResponsePayload; //This is the response payload returned by GetNextResponse() after sending SKIP_CMD_ID_GET_LED_STATE.

/***************************************************************************************************/
#define SKIP_ANALOG_INPUT_CHANNEL_VOFF 0
#define SKIP_ANALOG_INPUT_CHANNEL_VIN  1
#define SKIP_ANALOG_INPUT_CHANNEL_VIN_LOW 2
#define SKIP_ANALOG_INPUT_CHANNEL_VID  3
//
//SKIP_ANALOG_INPUT_CHANNEL_VIN is used for +/- 10 volt probes.
//SKIP_ANALOG_INPUT_CHANNEL_VIN_LOW is used for 5 volt probes.
//
typedef struct
{
	unsigned char analogInputChannel; //See SKIP_ANALOG_INPUT_CHANNEL definitions.
} GSkipSetAnalogInputChannelParams;//Parameter block passed into SendCmd() with SKIP_CMD_ID_SET_ANALOG_INPUT_CHANNEL.

typedef GSkipSetAnalogInputChannelParams GSkipGetAnalogInputChannelResponsePayload; //This is the response payload returned by GetNextResponse()
//after sending SKIP_CMD_ID_GET_ANALOG_INPUT_CHANNEL.

/***************************************************************************************************/
typedef struct
{
	char dacSetting;
} GSkipSetVinOffsetDacParams;//Parameter block passed into SendCmd() with SKIP_CMD_ID_SET_VIN_OFFSET_DAC.

typedef GSkipSetVinOffsetDacParams GSkipGetVinOffsetDacResponsePayload; //This is the response payload returned by GetNextResponse()
//after sending SKIP_CMD_ID_GET_VIN_OFFSET_DAC.

/***************************************************************************************************/
//The following bits are used by all Go! devices:
#define SKIP_MASK_STATUS_ERROR_CMD_NOT_RECOGNIZED 1
#define SKIP_MASK_STATUS_ERROR_CMD_IGNORED 2
#define SKIP_MASK_STATUS_ERROR_MASTER_FIFO_OVERFLOW 0X40

//The following bits are used by Skip only:
#define SKIP_MASK_STATUS_ERROR_PACKET_OVERRUN 4
#define SKIP_MASK_STATUS_ERROR_MEAS_PACKET_LOST 8
#define SKIP_MASK_STATUS_ERROR_MASTER_INVALID_SPI_PACKET 0X10
#define SKIP_MASK_STATUS_ERROR_MASTER_INVALID_MEAS_COUNTER 0X20
#define SKIP_MASK_STATUS_ERROR_MEAS_CONVERSION_LOST 0X80

//The following bits are used by Cyclops only:
//((system_status & SKIP_MASK_STATUS_BATTERY_STATE) == SKIP_MASK_STATUS_BATTERY_STATE_GOOD) => good batteries.
//((system_status & SKIP_MASK_STATUS_BATTERY_STATE) == SKIP_MASK_STATUS_BATTERY_STATE_LOW_WHILE_SAMPLING) => marginal batteries.
//((system_status & SKIP_MASK_STATUS_BATTERY_STATE) == SKIP_MASK_STATUS_BATTERY_STATE_LOW_ALWAYS) => bad batteries.
//((system_status & SKIP_MASK_STATUS_BATTERY_STATE) == SKIP_MASK_STATUS_BATTERY_STATE_MISSING) => no batteries.present.
#define SKIP_MASK_STATUS_BATTERY_STATE 0X0C
#define SKIP_MASK_STATUS_BATTERY_STATE_GOOD 0
#define SKIP_MASK_STATUS_BATTERY_STATE_LOW_WHILE_SAMPLING 4
#define SKIP_MASK_STATUS_BATTERY_STATE_LOW_ALWAYS 8
#define SKIP_MASK_STATUS_BATTERY_STATE_MISSING 0X0C

typedef struct
{
	unsigned char status;	//See SKIP_MASK_STATUS_*.
	unsigned char minorVersionMasterCPU;	//Binary coded decimal
	unsigned char majorVersionMasterCPU;	//Binary coded decimal
	unsigned char minorVersionSlaveCPU;		//Binary coded decimal - updated only by Skip and Cyclops, not by Jonah
	unsigned char majorVersionSlaveCPU;		//Binary coded decimal - updated only by Skip and Cyclops, not by Jonah
} GSkipGetStatusCmdResponsePayload; //This is the response payload returned by GetNextResponse() after sending SKIP_CMD_ID_GET_STATUS.

/***************************************************************************************************/
typedef struct
{
	unsigned char ww;			//week in year(starting at 1) in BCD format
	unsigned char yy;			//last two digits of year in BCD format
	unsigned char lsbyteLswordSerialCounter;
	unsigned char msbyteLswordSerialCounter;
	unsigned char lsbyteMswordSerialCounter;
	unsigned char msbyteMswordSerialCounter;
} GSkipGetSerialNumberCmdResponsePayload; //This is the response payload returned by GetNextResponse() after sending SKIP_CMD_ID_GET_SERIAL_NUMBER.

typedef struct
{
	unsigned char lsbyteLswordSensorId;
	unsigned char msbyteLswordSensorId;
	unsigned char lsbyteMswordSensorId;
	unsigned char msbyteMswordSensorId;
} GSkipGetSensorIdCmdResponsePayload; //This is the response payload returned by GetNextResponse() after sending SKIP_CMD_ID_GET_SENSOR_ID.

#if defined (TARGET_OS_WIN) 
#pragma pack(pop)
#endif

#ifdef TARGET_OS_MAC
#pragma pack()
#endif

#endif //_SKIP_COMMUNICATION_EXT_H_

