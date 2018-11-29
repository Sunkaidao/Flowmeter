#pragma once

#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_Param/AP_Param.h>

class AP_Flowmeter_WL
{
public:
	AP_Flowmeter_WL();
	~AP_Flowmeter_WL();
	void init();
	uint8_t update(uint8_t spraying);
	uint32_t get_flow_test();
	static const struct AP_Param::GroupInfo var_info[];


protected:

private:

	
	AP_Float        _coefficient_a;//coefficient Y="A"X+B
	AP_Float		_coefficient_b;//coefficient	Y=AX+"B"
	//AP_Int16		_dose;			
	AP_Int16		_Total_dose;//Total amount
	bool _initialised;

	int _fd;
	struct flowmeter_WL_data{
  
	uint32_t	_flowmeter_flag;//read pulse
	uint32_t	_flow_s_count;//pulse /s
	uint8_t		delay;//
	uint32_t	_flow_s;//flow ml/s
	uint32_t	_flow_test;//flow ml
	uint32_t	_remaining_flow;//emaining ml
	bool		_status;
	bool		_rtf_flag;//RTF flag
	}flowmeter_wl;

};

