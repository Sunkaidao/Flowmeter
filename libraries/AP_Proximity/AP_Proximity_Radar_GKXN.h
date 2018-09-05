#pragma once

#include "AP_Proximity.h"
#include "AP_Proximity_Backend.h"
#include <AP_AHRS/AP_AHRS_View.h>
#include <AP_InertialSensor/AP_InertialSensor.h>




#define PROXIMITY_Radar_GKXN_TIMEOUT_MS            300                               // requests timeout after 0.3 seconds

class AP_Proximity_Radar_GKXN : public AP_Proximity_Backend
{

public:
    // constructor
  AP_Proximity_Radar_GKXN(AP_Proximity &_frontend, AP_Proximity::Proximity_State &_state, AP_SerialManager &serial_manager);

    // static detection function
    static bool detect(AP_SerialManager &serial_manager);

    // update state
    void update(void);

    // get maximum and minimum distances (in meters) of sensor
    float distance_max() const;
    float distance_min() const;
	int Getdata();


private:

    // check and process replies from sensor
    bool read_sensor_data();
	bool send_sensor_command();
    void update_sector_data(int16_t angle_deg, uint16_t distance_cm);
    uint16_t process_distance(uint8_t buf1, uint8_t buf2);

    // reply related variables
    AP_HAL::UARTDriver *uart = nullptr;
	uint8_t TX_Buff[8];
	struct Time_radar_GKXN_error
	{
		uint8_t Time_Head_error;
		uint8_t Time_Invalid_data;
		uint8_t Time_Checksum_error;
	}_num_error;
	enum error_radar_GKXN {
		Right_data = 0,
		Invalid_data = 1,
	};
	uint8_t message_state=0;
	uint16_t checksum = 0x00;
	int      k=0;
	uint8_t address_code=0x00;
	uint8_t frame_longth=0x00;
	uint16_t front_data_back=0;
	uint16_t back_data_back=0;
	uint16_t down_data_back=0;
	uint8_t message_status=0;
	uint16_t d1=0x00;
	uint16_t d2=0x00;
    // request related variables
    uint32_t _last_distance_received_ms;    // system time of last distance measurement received from sensor
    //uint8_t start_flag=1;           //record the first start time of the cord
    //uint32_t _start_GKXN_time;      
};

