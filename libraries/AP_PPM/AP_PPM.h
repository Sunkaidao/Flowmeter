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
	
	bool _initialised;

private:
	

	int _fd;

	uint32_t	_flowmeter_flag;
	uint32_t	_flow_s_count;
	uint32_t	_flow_total;
	uint32_t	_flow_s;
	uint32_t	_flow_test;

	AP_Float        _coefficient_a;//coefficient
	AP_Float		_coefficient_b;


};

