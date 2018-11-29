#include <AP_BoardConfig/AP_BoardConfig.h>

#include <AC_Sprayer/AC_Sprayer.h>

#include <AP_Flowmeter_WL/AP_Flowmeter_WL.h>
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



const AP_Param::GroupInfo AP_Flowmeter_WL::var_info[] = {

// @Param: ENABLE
    // @DisplayName: Sprayer enable/disable
    // @Description: Allows you to enable (1) or disable (0) the flowmeter
    // @Values: 0:Disabled,1:Enabled
    // @User: Standard
    AP_GROUPINFO("COE_A",      0,  AP_Flowmeter_WL, _coefficient_a, 1),
	AP_GROUPINFO("COE_B",	   1,  AP_Flowmeter_WL, _coefficient_b, 0),
	//AP_GROUPINFO("_DOSE",	   2,  AP_Flowmeter_WL, _dose, 0),
	AP_GROUPINFO("_TO_DOSE",	   3,  AP_Flowmeter_WL, _Total_dose, 0),


     AP_GROUPEND
};


AP_Flowmeter_WL::AP_Flowmeter_WL()
{
	_initialised = false;

}
AP_Flowmeter_WL::~AP_Flowmeter_WL()
{
	if (_fd != -1) {
		   
	   }

}



void AP_Flowmeter_WL::init()
{
	flowmeter_wl.delay=0;

	flowmeter_wl._flow_s_count=0;
	flowmeter_wl._flow_s=0;
	flowmeter_wl._flow_test=0;
	flowmeter_wl._remaining_flow=0;
	flowmeter_wl._status=1;
	flowmeter_wl._rtf_flag=1;
	



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

uint8_t AP_Flowmeter_WL::update(uint8_t spraying)
{

	if(!_initialised)
	{
		
		//printf("///%f///\n",_coefficient);
		init();
		printf("return 2\n");
		return 2;
	}
	
	if(spraying==1)
	{

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
				flowmeter_wl._flow_s_count=rd.pulse_count-flowmeter_wl._flowmeter_flag;
				flowmeter_wl._flowmeter_flag= rd.pulse_count;
				flowmeter_wl._flow_s=(flowmeter_wl._flow_s_count*_coefficient_a+_coefficient_b);//10000*  q frequency  L/s
				flowmeter_wl._flow_test=(flowmeter_wl._flowmeter_flag*_coefficient_a+_coefficient_b);//ml
				flowmeter_wl._status=0;
			}
			else
			{
				
				flowmeter_wl._flow_s_count=0;
				flowmeter_wl._flow_s=_coefficient_b;
				flowmeter_wl._flow_test=(flowmeter_wl._flowmeter_flag*_coefficient_a+_coefficient_b);//ml
			
				flowmeter_wl._status=1;
				//return 0;
				
			}
		}
	}
	else
	{
		flowmeter_wl._status=0;
		//return 1;
	}
	
	//remaining
	
	flowmeter_wl._remaining_flow=_Total_dose*1000-flowmeter_wl._flow_test;
	printf("Random integer is %d\n", flowmeter_wl._flowmeter_flag);
	printf("_flow_s_count is %d\n", flowmeter_wl._flow_s_count);
	printf("_flow_s_test is %d\n", flowmeter_wl._flow_s);
	printf("_flow_test is %d\n\n", flowmeter_wl._flow_test);
	printf("_Total_dose is %d\n", _Total_dose);
	printf("_remaining_flow is %d\n\n", flowmeter_wl._remaining_flow);	
	printf("status is :%d\n",flowmeter_wl._status);
	printf("_rtf_flag is :%d\n",flowmeter_wl._rtf_flag);
	
	if(flowmeter_wl._status>flowmeter_wl._rtf_flag)
	{
		
		printf("delay is :%d\n",flowmeter_wl.delay++);
		if(flowmeter_wl.delay>5)
		{
			flowmeter_wl._rtf_flag=flowmeter_wl._status;
			printf("return 1\n");
			flowmeter_wl.delay=0;
			return 1;
		}
	}

	else
	{
		flowmeter_wl._rtf_flag=flowmeter_wl._status;
		//printf("return 0\n");
		//return 0;
	}
	printf("delay is :%d\n",flowmeter_wl.delay);
	return 0;
}
uint32_t AP_Flowmeter_WL::get_flow_test()
{
	return flowmeter_wl._flow_test;
}


