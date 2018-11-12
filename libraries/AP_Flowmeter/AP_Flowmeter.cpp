#include <AP_Flowmeter/AP_Flowmeter.h>
//#include <AP_HAL.h>
//#include <Pwm_input>
//.\..\module\px4firmware\src\drivers
/*
#include <AP_BoardConfig/AP_BoardConfig.h>
#include <board_config.h>
#include <stdio.h>
*/

//PWMIN pwmin;


extern const AP_HAL::HAL& hal;


const AP_Param::GroupInfo AP_Flowmeter::var_info[] = {
    // @Param: ENABLE
    // @DisplayName: Sprayer enable/disable
    // @Description: Allows you to enable (1) or disable (0) the flowmeter
    // @Values: 0:Disabled,1:Enabled
    // @User: Standard
    AP_GROUPINFO("ENABLE",      0,  AP_Flowmeter, _enabled, 0),
//added by xusiming in 20180514 
//depend on the email in 20180511 and the document in 2017
    // @Param: VOLUME
    // @DisplayName: warning number about volume
    // @Description: if the number reach the deadline that we will send waring message 
    // @Values:0~10000
    // @User: Standard
    AP_GROUPINFO("VOLUME",      1,  AP_Flowmeter, _volume, 0),
    // @Param: HIGH
    // @DisplayName: warning number about high
    // @Description:  if the number reach the deadline that we will send waring message 
    // @Values:0~10000
    // @User: Standard
    AP_GROUPINFO("HIGH",      2,  AP_Flowmeter, _high, 0),
    // @Param: TYPE
    // @DisplayName: the waring message type
    // @Description: 0 bit
    // @Values:0~10000
    // @User: Standard
    AP_GROUPINFO("ALARM",      3,  AP_Flowmeter, _alarm, 0),
    // @Param: TYPE
    // @DisplayName: the time delay
    // @Description: the delay warning 
    // @Values:0~126
    // @User: Standard
    AP_GROUPINFO("TIME",      4,  AP_Flowmeter, _time, 0),
//added end

    AP_GROUPEND
};


AP_Flowmeter::AP_Flowmeter()
{
	_initialised = false;
	
	AP_Param::setup_object_defaults(this, var_info);
/*
//sunkaidao added in 181023
	gpio=GPIO_GPIO0_INPUT;
	stm32_gpiosetevent(gpio, true, false, false, irq_handler0);
//added end 
*/
//	printf("0.%d", _initialised);
	
}

AP_Flowmeter::~AP_Flowmeter()
{}


void AP_Flowmeter::init(const AP_SerialManager& serial_manager)
{
	uint8_t i;

	if(0 == _enabled)
		return;

	_port = serial_manager.find_serial(AP_SerialManager::SerialProtocol_FlowMeter_GKXN, 0);

	if(_port != nullptr)
	{
	
		for(i=0;i<4;i++)
		{
			Tx_Buff[i]=0;
		}
		for(i=0;i<11;i++)
		{
			Rx_Buff[i]=0;
		}
		_Flo_data.high=0;
		_Flo_data.volume=0;
		_Flo_data.warning=0;
		_Flo_data.mode=0;
		_Flo_data.time=_time;
		_Flo_data.packet_cnt = 0;
		_Flo_data.flag=0;
  	 	 state_tim=0;
   	 	_num_error.Time_Head_error=0;
   		_num_error.Time_Invalid_data=0;
    	_num_error.Time_Parity_error=0;

		_initialised = true;

		
		
	}

//	printf("1.%d\n", _initialised);
}
/*
//sunkaidao added in 181024
int AP_Flowmeter::irq_handler0(int irq, void *context)
{
	irq_handler(0);
	return 0;
}

void AP_Flowmeter::irq_handler(uint8_t instance)
{
	_pulse_count++;
}
*/


void AP_Flowmeter::SendCMD(uint8_t CMD)
{
	uint8_t checkByte=0;
	uint8_t i;
	
	Tx_Buff[0]=0x55;
	Tx_Buff[1]=0x03;
	Tx_Buff[2]=CMD;
	checkByte=0x55+0x03+CMD;
	Tx_Buff[3]=checkByte;
	
	for(i=0;i<4;i++)
    	_port->write(Tx_Buff[i]);
}

int AP_Flowmeter::GetDate()
{
	uint16_t j;
	uint8_t checksum = 0x00;
	_Flo_data.warning=0;


	if((Rx_Buff[0] != 0x55) && (Rx_Buff[1] != 0x03) && (Rx_Buff[2] != 11))
	{
		_num_error.Time_Head_error++;
		return	Head_error;
	}
	if( Rx_Buff[9] != 0)
	{
		_num_error.Time_Invalid_data++;
		return	Invalid_data;
	}
	for(j=0;j<10; j++)
		checksum += Rx_Buff[j];
	if(checksum != Rx_Buff[10])
	{
	    _num_error.Time_Parity_error++;
		return	Parity_error;
	}
	_Flo_data.high =(Rx_Buff[3]<<8)+Rx_Buff[4];
	_Flo_data.volume =(Rx_Buff[5]<<8)+Rx_Buff[6];
	_Flo_data.flag = Rx_Buff[7];
	_Flo_data.mode = Rx_Buff[8];

	
	if((_alarm&0x01)==0x01)
	{
		if(_Flo_data.flag!=0)
		{
			_Flo_data.warning=1;
		}
	}
	if(((_alarm>>1)&0x01)==0x01)
	{
		if (_Flo_data.volume<_volume)
		{
			_Flo_data.warning=1;
		}
	}
	if(((_alarm>>2)&0x01)==0x01)
	{
		if (_Flo_data.high<_high)
		{
				_Flo_data.warning=1;
		}
	}
	
	_Flo_data.packet_cnt ++;
	
	return Right_data;
}
void AP_Flowmeter::get_Flowmeter_data()
{
	uint16_t i;
	int16_t numc;
	
	//	wite someting to flsuh the buffer of UART
	

	numc = _port->available();

//	printf("%d\n", numc);
	
	if(0 == numc)
		return;

	for (i = 0; i < numc; i++) {        // Process bytes received
        Rx_Buff[i] = _port->read();

//		printf("0x%x\n", Rx_Buff[i]);
	}

	
	if(GetDate()==Right_data)
	{
//		printf("liquid high 	  is %f\n",_Flo_data.high);
//		printf("liquid volume  is %f\n",_Flo_data.volume);
//		printf("liquid warning is %x\n",_Flo_data.warning);
//		printf("liquid mode 	  is %x\n",_Flo_data.mode);
	}
}

void AP_Flowmeter::update(const AP_SerialManager& serial_manager)
{
	

//	printf("2.%d\n", _initialised);
	if(0 == _enabled)
		return;

	//	to support warm plug
	if(!_initialised)
	{
		init(serial_manager);
	}

	if(!_initialised)
		return;
//	hal.uartD->printf("the pulse count:\n");
/*
	int _fd = open(PWMIN0_DEVICE_PATH, O_RDONLY);
	struct pwm_input_s pwm;
	while (::read(_fd, &pwm, sizeof(pwm)) == sizeof(pwm)){

	}
*/


/*
	get_Flowmeter_data();

//	if(state_tim%100==0)
//	{
		//hal.uartD->printf("Have output the CMD!\n");
	SendCMD(Read_Data);
		//hal.uartD->printf("Please input: \n");
//		state_tim=0;
//	}
//	printf("_Flo_data.warning=%d\n",_Flo_data.warning);
//	state_tim++;
*/
}


uint8_t AP_Flowmeter::get_warning()
{
	if(0 == _enabled)
		return 0;

	if(0 == _initialised)
		return 0;

	//printf("_Flo_data.warning=%d\n",_Flo_data.warning);
	return _Flo_data.warning;
}


bool AP_Flowmeter::exhausted()
{
	static uint8_t lcl_cnt = 0;

	if(0 == _enabled)
	{
		return false;
	}

	if(!_initialised)

	{
		return false;
	}
	if(1 == _Flo_data.warning)
	{
		if(sprayer_state==1)
			{
				lcl_cnt ++;
				if(lcl_cnt > (AP_FLOWMETER_EXHAUSTED_SHRESHOLD*_time))
				{
				lcl_cnt = 0;
				_Flo_data.time=0;
				return true;
				}
				else
				{
					if(lcl_cnt%3==0)
					{
						if(_Flo_data.time<=0)
							_Flo_data.time=0;
						else _Flo_data.time=_time-(lcl_cnt/3);
					}
				}
		}
	}
	else if(0 == _Flo_data.warning)
	{
		lcl_cnt = 0;
		_Flo_data.time=_time;
	}
	else
	{
//		printf("unsupportted value %d\n", _Flo_data.warning);
	}


	return false;
	
}


uint8_t AP_Flowmeter::get_packet_cnt()
{
	if(0 == _enabled)
	{
		return false;
	}

	if(!_initialised)

	{
		return false;
	}
	
	return _Flo_data.packet_cnt;
}

uint16_t AP_Flowmeter::get_volume()
{
	if(0 == _enabled)
		return 0;

	if(0 == _initialised)
		return 0;
	return _Flo_data.volume;
}
uint16_t AP_Flowmeter::get_high()
{
	if(0 == _enabled)
			return 0;
	
	if(0 == _initialised)
			return 0;

	return _Flo_data.high;
}
//added by xusiming in 20180821 and used for counting down
void AP_Flowmeter::get_sprayer_state(uint8_t state)
{
	sprayer_state=state;
}
uint8_t AP_Flowmeter::get_time()
{
	if(0 == _enabled)
				return 0;
		
	if(0 == _initialised)
				return 0;
	return _Flo_data.time;
}
//added end

