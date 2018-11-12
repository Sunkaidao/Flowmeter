#pragma once

#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_HAL/AP_HAL.h>

class AP_PPM
{
public:
	AP_PPM();
	~AP_PPM();
	void init();
	void update();
	static const struct AP_Param::GroupInfo var_info[];


protected:
	AP_HAL::UARTDriver *_port;
	
	bool _initialised;

private:
	int _fd;
	uint8_t pulse_count_flag;
	uint32_t _period;
	uint32_t _pulse_width;
	bool _pwm_init_flag;

	AP_Float         _coefficient;//coefficient

};

