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
#include <stdio.h>
#include <errno.h>
#include <cmath>

//#include <drivers/boards/px4io-v2/board_config.h> 

extern "C" {
    int pwm_input_main(int, char **);
};

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
	_pwm_init_flag=0;


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

void AP_PPM::init()
{
	pulse_count_flag=0;
	_period=0;
	_pulse_width=0;
	if(_pwm_init_flag==0)
	AP_BoardConfig::px4_start_driver(pwm_input_main, "pwm_input", "start");
	_pwm_init_flag=1;

	_fd = open(PWMIN0_DEVICE_PATH, O_RDONLY);
		if (_fd != -1) {
			return;
		}
		
	ioctl(_fd, SENSORIOCSQUEUEDEPTH, 20);
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

	//pulse_count_flag=_pulse_count;
	//printf("%d\n",pulse_count_flag);

	struct pwm_input_s pwm;
	//printf("///%f///\n",_pulse_count);

	//printf("\\%d\\\n",sizeof(pwm));
	while (::read(_fd, &pwm, sizeof(pwm)) == sizeof(pwm))
	{
		pulse_count_flag++;
		printf("pulse_count_flag :%d\n",pulse_count_flag*_coefficient);
	}
	
/*
	_port->printf("\nperiod %d\n",pwm.period);
	_port->printf("\npulse_width %d\n",pwm.pulse_width);
	_port->printf("\ntimestamp %d\n",pulse_count);
*/



	//_port->printf("test");

}
