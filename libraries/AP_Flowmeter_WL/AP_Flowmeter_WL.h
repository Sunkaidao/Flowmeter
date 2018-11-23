#pragma once

#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_HAL/AP_HAL.h>

class AP_Flowmeter_WL
{
public:
	AP_Flowmeter_WL();
	~AP_Flowmeter_WL();
	void init();
	void update();
	uint32_t get_flow_test();
	static const struct AP_Param::GroupInfo var_info[];


protected:

private:

	
	AP_Float        _coefficient_a;//coefficient
	AP_Float		_coefficient_b;
	AP_Int16		_dose;
	AP_Int16		_Total_dose;
	bool _initialised;

	int _fd;

	uint32_t	_flowmeter_flag;
	uint32_t	_flow_s_count;
	uint32_t	_flow_total;
	uint32_t	_flow_s;
	uint32_t	_flow_test;



};

