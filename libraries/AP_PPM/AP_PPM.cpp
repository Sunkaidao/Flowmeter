#include <AP_BoardConfig/AP_BoardConfig.h>

#include <AP_PPM/AP_PPM.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <drivers/drv_pwm_input.h>
#include <drivers/drv_hrt.h>
#include <drivers/drv_sensor.h>
#include <uORB/topics/pwm_input.h>
#include <uORB/topics/pulse.h>

#include <stdio.h>
#include <errno.h>
#include <cmath>

//#include <drivers/boards/px4io-v2/board_config.h> 

extern "C" {
    int pwm_input_main(int, char **);
};

static int pulse_handle;


const AP_Param::GroupInfo AP_PPM::var_info[] = {

// @Param: ENABLE
    // @DisplayName: Sprayer enable/disable
    // @Description: Allows you to enable (1) or disable (0) the flowmeter
    // @Values: 0:Disabled,1:Enabled
    // @User: Standard
    AP_GROUPINFO("COE",      0,  AP_PPM, _coefficient, 0),
     AP_GROUPEND
};


AP_PPM::AP_PPM()
{
	_initialised = false;
	_coefficient=1;
	_flow_s_count=0;
	_flow_total=0;
	_flow_s=0;


	//if(
		//!= 0)
	//	{
			
			//_port->printf("\nerror");
			//return;
	//	}
//


}
AP_PPM::~AP_PPM()
{
	if (_fd != -1) {
		   //close(_fd);
	   }

}
//extern ORB_DECLARE(flowmeter_pulse);


void AP_PPM::init()
{
	AP_BoardConfig::px4_start_driver(pwm_input_main, "pwm_input", "start");

	pulse_handle= orb_subscribe(ORB_ID(flowmeter_pulse));
	printf("orb_subscribe %d\n\n",pulse_handle);


	_fd = open(PWMIN0_DEVICE_PATH, O_RDONLY);
	if (_fd == -1) {
		printf("open error \n");
		return;
	}
	if (ioctl(_fd, SENSORIOCSQUEUEDEPTH, 20) != 0)
	{
		printf("ioctl error \n");
		return;
	}
	//ioctl(_fd, SENSORIOCSQUEUEDEPTH, 20);
//	close(_fd);

	//_port = serial_manager.find_serial(AP_SerialManager::SerialProtocol_FlowMeter_GKXN, 0);
		_initialised = true;	

}

void AP_PPM::update()
{

	if(!_initialised)
	{
		
		//printf("///%f///\n",_coefficient);
		init();
		return;
	}
	
	if(orb_exists(ORB_ID(flowmeter_pulse),0)==OK)
	{
		if(pulse_handle<0)
		{
			pulse_handle= orb_subscribe(ORB_ID(flowmeter_pulse));
		}

		bool updated;
		struct _pulse_count_s rd;
		orb_check(pulse_handle, &updated);

		if(updated)
		{
			orb_copy(ORB_ID(flowmeter_pulse), pulse_handle, &rd);
			printf("Random integer is now %d\n", rd.pulse_count);
			_flow_s_count=rd.pulse_count-_flowmeter_flag;
			_flowmeter_flag= rd.pulse_count;
			_flow_total=rd.pulse_count*500*23*_coefficient/6;//10000*  q pulse count
			_flow_s=_flow_s_count*23*_coefficient/12;//10000*  q frequency  L/s
		}
		else
		{
			_flow_s_count=0;
			_flow_s=0;

		}
	}
	printf("_flow_s_count is %d\n", _flow_s_count);
	printf("_flow_total is %d\n", _flow_total);
	printf("_flow_s is %d\n", _flow_s);
	

/*
	struct pwm_input_s pwm;
	//printf("///%f///\n",_pulse_count);

	//printf("\\%d\\\n",sizeof(pwm));
	while (::read(_fd, &pwm, sizeof(pwm)) == sizeof(pwm))
	{
		pulse_count_flag++;
		printf("pulse_count_flag :%d\n",pulse_count_flag*_coefficient);
	}
*/
}
