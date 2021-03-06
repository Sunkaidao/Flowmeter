#include "Copter.h"

#ifdef GPS_YAW_CAL
//baiyang added in 20171023
#define NARROW_INT 3
#define GPS_SUPPLY_HEAD 1
#define COMPASS_SUPPLY_HEAD 0
//added end
#endif


//extern EKF_check_state ekf_check_state;

// performs pre-arm checks. expects to be called at 1hz.
void AP_Arming_Copter::update(void)
{
    // perform pre-arm checks & display failures every 30 seconds
    static uint8_t pre_arm_display_counter = PREARM_DISPLAY_PERIOD/2;
    pre_arm_display_counter++;
    bool display_fail = false;
    if (pre_arm_display_counter >= PREARM_DISPLAY_PERIOD) {
        display_fail = true;
        pre_arm_display_counter = 0;
    }

    if (pre_arm_checks(display_fail)) {
		//	20180417
		//	printf("AP_Arming_Copter::update\n");
		//	end
        set_pre_arm_check(true);
    }
}

// performs pre-arm checks and arming checks
bool AP_Arming_Copter::all_checks_passing(bool arming_from_gcs)
{
    if (pre_arm_checks(true)) {
		//	20180417
		//	printf("AP_Arming_Copter::all_checks_passing\n");
		//	end
        set_pre_arm_check(true);
    } else {
        return false;
    }

	//printf("AP_Arming_Copter all_checks_passing\n");

    return copter.ap.pre_arm_check && arm_checks(true, arming_from_gcs);
}

// perform pre-arm checks and set ap.pre_arm_check flag
//  return true if the checks pass successfully
// NOTE: this does *NOT* call AP_Arming::pre_arm_checks() yet!
bool AP_Arming_Copter::pre_arm_checks(bool display_failure)
{



  
//#if FXTX_AUTH == ENABLED
    //	added by Zhangyong to inform the mission palnner
	bool return_value;
	//	added end
//#endif


	//	added by zhangyong for ekf check 20180813
	//ekf_checks(true);
	//	added end


    // exit immediately if already armed
    if (copter.motors->armed()) {
        return true;
    }

    // check if motor interlock and Emergency Stop aux switches are used
    // at the same time.  This cannot be allowed.
    if (copter.check_if_auxsw_mode_used(AUXSW_MOTOR_INTERLOCK) && copter.check_if_auxsw_mode_used(AUXSW_MOTOR_ESTOP)){
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Interlock/E-Stop Conflict");
        }
        return false;
    }

    // check if motor interlock aux switch is in use
    // if it is, switch needs to be in disabled position to arm
    // otherwise exit immediately.  This check to be repeated,
    // as state can change at any time.
    if (copter.ap.using_interlock && copter.ap.motor_interlock_switch) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Motor Interlock Enabled");
        }
        return false;
    }


    // exit immediately if we've already successfully performed the pre-arm check
    if (copter.ap.pre_arm_check) {
        // run gps checks because results may change and affect LED colour
        // no need to display failures because arm_checks will do that if the pilot tries to arm
        gps_checks(false);
        return true;
    }

	//baiyang modified in 20170705
    #if CHARGINGSTATION== ENABLED
    		// exit immediately if we've already successfully performed the pre-arm check
    		if (copter.ap.pre_arm_check) {
    			// run gps checks because results may change and affect LED colour
    			// no need to display failures because arm_checks will do that if the pilot tries to arm
    			gps_checks(false);
    			if(checks_to_perform == ARMING_CHECK_NONE||chargingStation_checks(display_failure))
    				return true;
    			else
    				set_pre_arm_check(false);
    				return false;
    		   
    		}
    #else
    		// exit immediately if we've already successfully performed the pre-arm check
    		if (copter.ap.pre_arm_check) {
    			// run gps checks because results may change and affect LED colour
    			// no need to display failures because arm_checks will do that if the pilot tries to arm
    			gps_checks(false);
    			return true;
    		}
    #endif
    //modified end

 
  

    // succeed if pre arm checks are disabled
    if (checks_to_perform == ARMING_CHECK_NONE) {
		//	20180417
		//	printf("AP_Arming_Copter::pre_arm_checks\n");
		//	end
        set_pre_arm_check(true);
        set_pre_arm_rc_check(true);

		//	added by ZhangYong 20170406  to inform the mission palnner
		gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: succeed");
		//	added end

		
        return true;
    }

	//	modified by ZhangYong to inform the mission palnner
	//	if gcs mode without rc radio
	/*
    return barometer_checks(display_failure)
        & rc_calibration_checks(display_failure)
        & compass_checks(display_failure)
        & gps_checks(display_failure)
        & fence_checks(display_failure)
        & ins_checks(display_failure)
        & board_voltage_checks(display_failure)
        & logging_checks(display_failure)
        & parameter_checks(display_failure)
        & motor_checks(display_failure)
        & pilot_throttle_checks(display_failure);*/


	
	/*else
	{
		printf("pre_arm_checks %d\n", return_value);
	}*/


	//	added by ZhangYong 20160907
#if FXTX_AUTH == ENABLED	
#if PROJECTGKXN == ENABLED

	if((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform == (~ARMING_CHECK_INS)))
	{
		if((copter.g.flight_time_hour_shold > 0)&&(copter.g.flight_time_hour >= copter.g.flight_time_hour_shold))
		{
			if (display_failure) 
			{
   	             gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: Flight time exceed");
   	     	}
   	     	return false;
		}
	}
	//	added end

	//	added by ZhangYong 20170815


	if(true == copter.fs_mk.control_present)
	{
		if(true == copter.fs_mk.not_consist)
		{
			gcs().send_text(MAV_SEVERITY_CRITICAL, "remote control error, reset system");
		
			return false;
		}
	}
#endif
#endif
//	added end

	//	modified by zhangyong for payload check
	return_value =  barometer_checks(display_failure)
        & rc_calibration_checks(display_failure)
        & compass_checks(display_failure)
        & gps_checks(display_failure)
        & fence_checks(display_failure)
        & ins_checks(display_failure)
        & board_voltage_checks(display_failure)
        & logging_checks(display_failure)
        & parameter_checks(display_failure)
        & motor_checks(display_failure)
        & payload_checks(display_failure)
        & ( \
        	((!copter.ap.rc_receiver_present) && (!pilot_throttle_checks(display_failure))) || \
        	((copter.ap.rc_receiver_present) && (pilot_throttle_checks(display_failure)))\
          )
        & ekf_checks(true)
#if CHARGINGSTATION == ENABLED
        & chargingStation_checks(display_failure)
#endif
		;

	if(return_value)
	{
		gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: succeed");
	}
	
	return return_value;
}

bool AP_Arming_Copter::rc_calibration_checks(bool display_failure)
{
    // pre-arm rc checks a prerequisite
    pre_arm_rc_checks(display_failure);
    return copter.ap.pre_arm_rc_check;
}

bool AP_Arming_Copter::barometer_checks(bool display_failure)
{
    if (!AP_Arming::barometer_checks(display_failure)) {
        return false;
    }

    bool ret = true;
    // check Baro
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_BARO)) {
        // Check baro & inav alt are within 1m if EKF is operating in an absolute position mode.
        // Do not check if intending to operate in a ground relative height mode as EKF will output a ground relative height
        // that may differ from the baro height due to baro drift.
        nav_filter_status filt_status = _inav.get_filter_status();
        bool using_baro_ref = (!filt_status.flags.pred_horiz_pos_rel && filt_status.flags.pred_horiz_pos_abs);

		//	added by ZhangYong for alt check
		//printf("barometer_checks %d %d %d\n", filt_status.flags.pred_horiz_pos_rel, filt_status.flags.pred_horiz_pos_abs, using_baro_ref);
		//printf("alt %4.2f baro %d\n", _inav.get_altitude(), copter.baro_alt);
		//	added end

		
		
        if (using_baro_ref) {
            if (fabsf(_inav.get_altitude() - copter.baro_alt) > PREARM_MAX_ALT_DISPARITY_CM) {
                if (display_failure) {
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Altitude disparity");
                }
                ret = false;
            }
        }
    }
    return ret;
}

#if LOGGING_ENABLED == ENABLED
bool AP_Arming_Copter::dataflash_checks(bool display_failure)
{
	if(checks_to_perform == ARMING_CHECK_ALL)
	{
		if(_dateflash.get_num_logs() >= _dateflash.get_num_logs_max())
		{
			if (display_failure) 
			{
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: log numbers exceed max");
            }

			return false;
		}
	}

	return true;
}
#endif

bool AP_Arming_Copter::compass_checks(bool display_failure)
{
    bool ret = AP_Arming::compass_checks(display_failure);

    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_COMPASS)) {
        // check compass offsets have been set.  AP_Arming only checks
        // this if learning is off; Copter *always* checks.
        if (!_compass.configured()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Compass not calibrated");
            }
            ret = false;
        }
    }

    return ret;
}

bool AP_Arming_Copter::fence_checks(bool display_failure)
{
    #if AC_FENCE == ENABLED
    // check fence is initialised
    const char *fail_msg = nullptr;
    if (!copter.fence.pre_arm_check(fail_msg)) {
        if (display_failure && fail_msg != nullptr) {
            gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: %s", fail_msg);
        }
        return false;
    }
    #endif
    return true;
}

bool AP_Arming_Copter::ins_checks(bool display_failure)
{
    bool ret = AP_Arming::ins_checks(display_failure);

    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_INS)) {

        // get ekf attitude (if bad, it's usually the gyro biases)
        if (!pre_arm_ekf_attitude_check()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: gyros still settling");
            }
            ret = false;
        }
    }

    return ret;
}

bool AP_Arming_Copter::board_voltage_checks(bool display_failure)
{
    if (!AP_Arming::board_voltage_checks(display_failure)) {
        return false;
    }

    Parameters &g = copter.g;

    // check battery voltage
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_VOLTAGE)) {
        if (copter.failsafe.battery) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Battery failsafe");
            }
            return false;
        }

        // all following checks are skipped if USB is connected
        if (copter.ap.usb_connected) {
            return true;
        }

        // check if battery is exhausted
        if (copter.battery.exhausted(g.fs_batt_voltage, g.fs_batt_mah)) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Check Battery");
            }
            return false;
        }

        // call parent battery checks
        if (!AP_Arming::battery_checks(display_failure)) {
            return false;
        }
    }

    return true;
}

bool AP_Arming_Copter::parameter_checks(bool display_failure)
{
    // check various parameter values
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_PARAMETERS)) {

        // ensure ch7 and ch8 have different functions
        if (copter.check_duplicate_auxsw()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Duplicate Aux Switch Options");
            }
            return false;
        }

        // failsafe parameter checks
        if (copter.g.failsafe_throttle) {
            // check throttle min is above throttle failsafe trigger and that the trigger is above ppm encoder's loss-of-signal value of 900
            if (copter.channel_throttle->get_radio_min() <= copter.g.failsafe_throttle_value+10 || copter.g.failsafe_throttle_value < 910) {
                if (display_failure) {
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Check FS_THR_VALUE");
                }
                return false;
            }
        }

        // lean angle parameter check
        if (copter.aparm.angle_max < 1000 || copter.aparm.angle_max > 8000) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Check ANGLE_MAX");
            }
            return false;
        }

        // acro balance parameter check
        if ((copter.g.acro_balance_roll > copter.attitude_control->get_angle_roll_p().kP()) || (copter.g.acro_balance_pitch > copter.attitude_control->get_angle_pitch_p().kP())) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: ACRO_BAL_ROLL/PITCH");
            }
            return false;
        }

        #if RANGEFINDER_ENABLED == ENABLED && OPTFLOW == ENABLED
        // check range finder if optflow enabled
        if (copter.optflow.enabled() && !copter.rangefinder.pre_arm_check()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check range finder");
            }
            return false;
        }
        #endif

        #if FRAME_CONFIG == HELI_FRAME
        // check helicopter parameters
        if (!copter.motors->parameter_check(display_failure)) {
            return false;
        }
        #endif // HELI_FRAME

        // check for missing terrain data
        if (!pre_arm_terrain_check(display_failure)) {
            return false;
        }

        // check adsb avoidance failsafe
        if (copter.failsafe.adsb) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: ADSB threat detected");
            }
            return false;
        }

        // check for something close to vehicle
        if (!pre_arm_proximity_check(display_failure)) {
            return false;
        }
    }

    return true;
}

// check motor setup was successful
bool AP_Arming_Copter::motor_checks(bool display_failure)
{
    // check motors initialised  correctly
    if (!copter.motors->initialised_ok()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check firmware or FRAME_CLASS");
        }
        return false;
    }
    return true;
}


bool AP_Arming_Copter::ekf_checks(bool display_failure)
{
//	ekf_check_state.test_counter ++;

//	
	EKF_check_state *ekf_check_state_ptr;
	
	

//	printf("counter %d\n", ekf_check_state_ptr->test_counter);

	// exit immediately if ekf has no origin yet - this assumes the origin can never become unset
    Location temp_loc;
    if (!ahrs.get_origin(temp_loc)) 
	{
        return true;
    }

	
	// return immediately if motors are not armed, ekf check is disabled, not using ekf or usb is connected
	if (copter.ap.usb_connected || (copter.g.fs_ekf_thresh <= 0.0f)) 
	{
		return true;
	}


	if(checks_to_perform != ARMING_CHECK_ALL)
	{
		return true;
	}

	ekf_check_state_ptr = copter.get_ekf_check_state();

	

	if(true == ekf_check_state_ptr->bad_variance)
	{
		if (display_failure) 
		{	
			switch(ekf_check_state_ptr->type)
			{
			
				case EKF_VARIANCE_MAG:
					gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF variance MAG");
					break;

				case EKF_VARIANCE_VEL:
					gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF variance VEL");
					break;
							
				case EKF_VARIANCE_POS:
					gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF variance POS");
					break;
							
				default:
					gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF variance");
					break;
			}
		}
		return false;
	}

	//copter.ekf_check_state.fail_count;
	
	return true;
}


//	added by ZhangYong 20180530 for payload checks
bool AP_Arming_Copter::payload_checks(bool display_failure)
{
#if ((PROJECTGKXN == ENABLED)&&(FLOWMETER==ENABLED))
	// check flowmeter initialised  correctly
    if (copter.flowmeter.enabled()) 
	{
		//	to check the serial port configuration
		if(nullptr == copter.serial_manager.find_serial(AP_SerialManager::SerialProtocol_FlowMeter_GKXN, 0))
		{
			if (display_failure) {
            	gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check serial port protocol for flowmeter");
        	}
			
			return false;
		}
		else
		{
			if(AP_SERIALMANAGER_FLOWMETER_GKXN_BAUD != copter.serial_manager.find_baudrate(AP_SerialManager::SerialProtocol_FlowMeter_GKXN, 0))
			{
				if (display_failure) {
            		gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check serial port baud for flowmeter");
				}

				return false;
			}
		}		
		
		if(1 == copter.flowmeter.exhausted())
		{
        	if (display_failure) {
            	gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: liquid out");
        	}
			
			return false;
		}
    }
#endif

#if ((PROJECTGKXN == ENABLED)&&(SPRAYER==ENABLED))
	// check motors initialised  correctly
    if (copter.sprayer.get_enabled()) 
	{
		//	to check the aux port configuration
		if(!SRV_Channels::function_assigned(SRV_Channel::k_sprayer_pump))
		{
			if (display_failure) {
            	gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check AUX port for sprayer pump");
        	}

			return false;
		}

		

		//	to check the aux port configuration
		if(!SRV_Channels::function_assigned(SRV_Channel::k_sprayer_spinner))
		{
			if (display_failure) {
            	gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check AUX port for sprayer spinner");
        	}

			return false;
		}
    }
#endif

    return true;
}
//	added end

bool AP_Arming_Copter::pilot_throttle_checks(bool display_failure)
{
    // check throttle is above failsafe throttle
    // this is near the bottom to allow other failures to be displayed before checking pilot throttle
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_RC)) {
        if (copter.g.failsafe_throttle != FS_THR_DISABLED && copter.channel_throttle->get_radio_in() < copter.g.failsafe_throttle_value) {
            if (display_failure) {
                #if FRAME_CONFIG == HELI_FRAME
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Collective below Failsafe");
                #else
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Throttle below Failsafe");
                #endif
            }
            return false;
        }
    }

    return true;
}

// perform pre_arm_rc_checks checks and set ap.pre_arm_rc_check flag
void AP_Arming_Copter::pre_arm_rc_checks(const bool display_failure)
{
    // exit immediately if we've already successfully performed the pre-arm rc check
    if (copter.ap.pre_arm_rc_check) {
        return;
    }

    const RC_Channel *channels[] = {
        copter.channel_roll,
        copter.channel_pitch,
        copter.channel_throttle,
        copter.channel_yaw
    };

    copter.ap.pre_arm_rc_check = rc_checks_copter_sub(display_failure, channels);
}

// performs pre_arm gps related checks and returns true if passed
bool AP_Arming_Copter::gps_checks(bool display_failure)
{
    // always check if inertial nav has started and is ready
    if (!ahrs.healthy()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Waiting for Nav Checks");
        }
        return false;
    }

    // check if flight mode requires GPS
    bool mode_requires_gps = copter.mode_requires_GPS(copter.control_mode);

    // check if fence requires GPS
    bool fence_requires_gps = false;
    #if AC_FENCE == ENABLED
    // if circular or polygon fence is enabled we need GPS
    fence_requires_gps = (copter.fence.get_enabled_fences() & (AC_FENCE_TYPE_CIRCLE | AC_FENCE_TYPE_POLYGON)) > 0;
    #endif

    // return true if GPS is not required
    if (!mode_requires_gps && !fence_requires_gps) {
        AP_Notify::flags.pre_arm_gps_check = true;
        return true;
    }

	//printf("mode_requires_gps %d fence_requires_gps %d\n", mode_requires_gps, fence_requires_gps);

    // ensure GPS is ok
    if (!copter.position_ok()) 
	{
        if (display_failure) 
		{
            const char *reason = ahrs.prearm_failure_reason();

            if (reason) {
                gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: %s", reason);
            } else {
                if (!mode_requires_gps && fence_requires_gps) {
                    // clarify to user why they need GPS in non-GPS flight mode
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Fence enabled, need 3D Fix");
                } else {
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Need 3D Fix");
                }
            }
        }
        AP_Notify::flags.pre_arm_gps_check = false;
        return false;
    }

    // check for GPS glitch (as reported by EKF)
    nav_filter_status filt_status;
    if (_ahrs_navekf.get_filter_status(filt_status)) {
        if (filt_status.flags.gps_glitching) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: GPS glitching");
            }
            return false;
        }
    }

#ifdef GPS_YAW_CAL

	//baiyang added in 20171023
	if(copter.EKF2.get_ekf_heading_mode() == GPS_SUPPLY_HEAD && \
		copter.gps.Headstatus() < NARROW_INT){
	   if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Need GPS HEADING locking");
       }
	   
	   AP_Notify::flags.pre_arm_gps_check = false;
	   return false;	  
	}
	//added end
#endif

    // check EKF compass variance is below failsafe threshold
    float vel_variance, pos_variance, hgt_variance, tas_variance;
    Vector3f mag_variance;
    Vector2f offset;
    _ahrs_navekf.get_variances(vel_variance, pos_variance, hgt_variance, mag_variance, tas_variance, offset);
    if (mag_variance.length() >= copter.g.fs_ekf_thresh) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF compass variance");
        }
        return false;
    }

    // check home and EKF origin are not too far
    if (copter.far_from_EKF_origin(ahrs.get_home())) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: EKF-home variance");
        }
        AP_Notify::flags.pre_arm_gps_check = false;
        return false;
    }

    // return true immediately if gps check is disabled
    if (!(checks_to_perform == ARMING_CHECK_ALL || checks_to_perform & ARMING_CHECK_GPS)) {
        AP_Notify::flags.pre_arm_gps_check = true;
        return true;
    }

    // warn about hdop separately - to prevent user confusion with no gps lock
    if (copter.gps.get_hdop() > copter.g.gps_hdop_good) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: High GPS HDOP");
        }
        AP_Notify::flags.pre_arm_gps_check = false;
        return false;
    }

    // call parent gps checks
    if (!AP_Arming::gps_checks(display_failure)) {
        return false;
    }

    // if we got here all must be ok
    AP_Notify::flags.pre_arm_gps_check = true;
    return true;
}

// check ekf attitude is acceptable
bool AP_Arming_Copter::pre_arm_ekf_attitude_check()
{
    // get ekf filter status
    nav_filter_status filt_status = _inav.get_filter_status();

    return filt_status.flags.attitude;
}

// check we have required terrain data
bool AP_Arming_Copter::pre_arm_terrain_check(bool display_failure)
{
#if AP_TERRAIN_AVAILABLE && AC_TERRAIN
    // succeed if not using terrain data
    if (!copter.terrain_use()) {
        return true;
    }

    // check if terrain following is enabled, using a range finder but RTL_ALT is higher than rangefinder's max range
    // To-Do: modify RTL return path to fly at or above the RTL_ALT and remove this check

    if (copter.rangefinder_state.enabled && (copter.g.rtl_altitude > copter.rangefinder.max_distance_cm_orient(ROTATION_PITCH_270))) {
        gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: RTL_ALT above rangefinder max range");
        return false;
    }

    // show terrain statistics
    uint16_t terr_pending, terr_loaded;
    copter.terrain.get_statistics(terr_pending, terr_loaded);
    bool have_all_data = (terr_pending <= 0);
    if (!have_all_data && display_failure) {
        gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: Waiting for Terrain data");
    }
    return have_all_data;
#else
    return true;
#endif
}

//baiyang added in 20170703
#if CHARGINGSTATION ==ENABLED
bool AP_Arming_Copter::pre_chargingStation_check(bool display_failure)
{
    // check fence is initialised
    if (copter.chargingStation.get_Bstation_use()) {
		if(!copter.chargingStation.get_flight_permit()){
	        if (display_failure) {
	            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: ChargingStation close");
	        }
	        return false;
		}else
			return true;
    }
	
    return true;
}

bool AP_Arming_Copter::chargingStation_checks(bool display_failure)
{
	// check chargingStation
    if (!pre_chargingStation_check(display_failure)) {
        return false;
    }
    return true;
}

#endif

//added end

// check nothing is too close to vehicle
bool AP_Arming_Copter::pre_arm_proximity_check(bool display_failure)
{
#if PROXIMITY_ENABLED == ENABLED

    // return true immediately if no sensor present
    if (copter.g2.proximity.get_status() == AP_Proximity::Proximity_NotConnected) {
        return true;
    }

    // return false if proximity sensor unhealthy
    if (copter.g2.proximity.get_status() < AP_Proximity::Proximity_Good) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"PreArm: check proximity sensor");
        }
        return false;
    }

    // get closest object if we might use it for avoidance
#if AC_AVOID_ENABLED == ENABLED
    float angle_deg, distance;
    if (copter.avoid.proximity_avoidance_enabled() && copter.g2.proximity.get_closest_object(angle_deg, distance)) {
        // display error if something is within 60cm
        if (distance <= 0.6f) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: Proximity %d deg, %4.2fm", (int)angle_deg, (double)distance);
            }
            return false;
        }
    }
#endif

    return true;
#else
    return true;
#endif
}

// arm_checks - perform final checks before arming
//  always called just before arming.  Return true if ok to arm
//  has side-effect that logging is started
bool AP_Arming_Copter::arm_checks(bool display_failure, bool arming_from_gcs)
{
	EKF_check_state *ekf_check_state_ptr;
	
    // check accels and gyro are healthy
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_INS)) {
        //check if accelerometers have calibrated and require reboot
        if (_ins.accel_cal_requires_reboot()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: Accels calibrated requires reboot");
            }
            return false;
        }

        if (!_ins.get_accel_health_all()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Accels not healthy");
            }
            return false;
        }
        if (!_ins.get_gyro_health_all()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Gyros not healthy");
            }
            return false;
        }
        // get ekf attitude (if bad, it's usually the gyro biases)
        if (!pre_arm_ekf_attitude_check()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: gyros still settling");
            }
            return false;
        }
    }

    // always check if inertial nav has started and is ready
    if (!ahrs.healthy()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Waiting for Nav Checks");
        }
        return false;
    }

    // check compass health
    if (!_compass.healthy()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Compass not healthy");
        }
        return false;
    }

    if (_compass.is_calibrating()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Compass calibration running");
        }
        return false;
    }

    //check if compass has calibrated and requires reboot
    if (_compass.compass_cal_requires_reboot()) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL, "PreArm: Compass calibrated requires reboot");
        }
        return false;
    }

	//	added by zhangyong 20180713
	//	if external compass not exist, can not be armed
	if(0 == _compass.get_external(_compass.get_primary()))
	{
		if (display_failure) 
		{
        	gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: external Compasses need");
        }
		return false;
	}

	//	added end

    control_mode_t control_mode = copter.control_mode;

    // always check if the current mode allows arming
    if (!copter.mode_allows_arming(control_mode, arming_from_gcs)) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Mode not armable");
        }
        return false;
    }

    // always check gps
    if (!gps_checks(display_failure)) {
        return false;
    }

    // always check motors
    if (!motor_checks(display_failure)) {
        return false;
    }


	//	added by ZhangYong for payload arming check 20180531
	if(!payload_checks(display_failure)) 
	{
		return false;
	}
		
	//	added end

    // if we are using motor interlock switch and it's enabled, fail to arm
    // skip check in Throw mode which takes control of the motor interlock
    if (copter.ap.using_interlock && copter.motors->get_interlock()) {
        gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Motor Interlock Enabled");
        return false;
    }

    // if we are not using Emergency Stop switch option, force Estop false to ensure motors
    // can run normally
    if (!copter.check_if_auxsw_mode_used(AUXSW_MOTOR_ESTOP)){
        copter.set_motor_emergency_stop(false);
        // if we are using motor Estop switch, it must not be in Estop position
    } else if (copter.check_if_auxsw_mode_used(AUXSW_MOTOR_ESTOP) && copter.ap.motor_emergency_stop){
        gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Motor Emergency Stopped");
        return false;
    }

    // succeed if arming checks are disabled
    if (checks_to_perform == ARMING_CHECK_NONE) {
        return true;
    }

    // baro checks
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_BARO)) {
        // baro health check
        if (!barometer.all_healthy()) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Barometer not healthy");
            }
            return false;
        }
        // Check baro & inav alt are within 1m if EKF is operating in an absolute position mode.
        // Do not check if intending to operate in a ground relative height mode as EKF will output a ground relative height
        // that may differ from the baro height due to baro drift.
        nav_filter_status filt_status = _inav.get_filter_status();
        bool using_baro_ref = (!filt_status.flags.pred_horiz_pos_rel && filt_status.flags.pred_horiz_pos_abs);

		

		
        if (using_baro_ref && (fabsf(_inav.get_altitude() - copter.baro_alt) > PREARM_MAX_ALT_DISPARITY_CM)) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Altitude disparity");
            }
            return false;
        }
    }

#if AC_FENCE == ENABLED
    // check vehicle is within fence
    const char *fail_msg = nullptr;
    if (!copter.fence.pre_arm_check(fail_msg)) {
        if (display_failure && fail_msg != nullptr) {
            gcs().send_text(MAV_SEVERITY_CRITICAL, "Arm: %s", fail_msg);
        }
        return false;
    }
#endif

    // check lean angle
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_INS)) {
        if (degrees(acosf(ahrs.cos_roll()*ahrs.cos_pitch()))*100.0f > copter.aparm.angle_max) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Leaning");
            }
            return false;
        }
    }

    // check battery voltage
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_VOLTAGE)) {
        if (copter.failsafe.battery || (!copter.ap.usb_connected && copter.battery.exhausted(copter.g.fs_batt_voltage, copter.g.fs_batt_mah))) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Check Battery");
            }
            return false;
        }
    }

    // check for missing terrain data
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_PARAMETERS)) {
        if (!pre_arm_terrain_check(display_failure)) {
            return false;
        }
    }

    // check adsb
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_PARAMETERS)) {
        if (copter.failsafe.adsb) {
            if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: ADSB threat detected");
            }
            return false;
        }
    }

    // check throttle
    if ((checks_to_perform == ARMING_CHECK_ALL) || (checks_to_perform & ARMING_CHECK_RC)) {
        // check throttle is not too low - must be above failsafe throttle
        if (copter.g.failsafe_throttle != FS_THR_DISABLED && copter.channel_throttle->get_radio_in() < copter.g.failsafe_throttle_value) {
            if (display_failure) {
                #if FRAME_CONFIG == HELI_FRAME
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Collective below Failsafe");
                #else
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Throttle below Failsafe");
                #endif
            }
            return false;
        }

        // check throttle is not too high - skips checks if arming from GCS in Guided
        if (!(arming_from_gcs && (control_mode == GUIDED || control_mode == GUIDED_NOGPS))) {
            // above top of deadband is too always high
            if (copter.get_pilot_desired_climb_rate(copter.channel_throttle->get_control_in()) > 0.0f) {
                if (display_failure) {
                    #if FRAME_CONFIG == HELI_FRAME
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Collective too high");
                    #else
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Throttle too high");
                    #endif
                }
                return false;
            }
            // in manual modes throttle must be at zero
            if ((copter.mode_has_manual_throttle(control_mode) || control_mode == DRIFT) && copter.channel_throttle->get_control_in() > 0) {
                if (display_failure) {
                    #if FRAME_CONFIG == HELI_FRAME
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Collective too high");
                    #else
                    gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Throttle too high");
                    #endif
                }
                return false;
            }
        }
    }


	//	added by ZhangYong 20171220
#if FXTX_AUTH == ENABLED

	//printf("111arm_checks %d %d %d\n", fs_mk.control_present, fs_mk.not_consist, fs_mk.gcs_control);
//	printf("arming_from_gcs %d gcs_control %d\n", arming_from_gcs, copter.fs_mk.gcs_control);

	if(ARM_SOURCE_RC == arming_from_gcs)
	{
		if(0 == copter.g.failsafe_throttle)
		{
			gcs().send_text(MAV_SEVERITY_CRITICAL, "RC control, failsafe RC should be set");

			return false;
		}
	}


	if(1 == copter.fs_mk.gcs_control)
	{
		if(0 == copter.g.failsafe_gcs)
		{
			gcs().send_text(MAV_SEVERITY_CRITICAL, "GCS control, failsafe GCS should be set");
		

			return false;
		}

		

		if(copter.ap.rc_receiver_present)
		{
			if(0 == copter.g.failsafe_throttle)
			{
				gcs().send_text(MAV_SEVERITY_CRITICAL, "GCS control, failsafe RC should also be set");

				return false;
			}
			
			if(LOITER != copter.flight_modes[copter.readSwitch()])
			{
			
				gcs().send_text(MAV_SEVERITY_CRITICAL, "GCS control, set radio control switch to LOITER");

//				printf("GCS control, set radio control switch to LOITER");

				return false;
			}
		
//			printf("arm_checks %d\n", g.rc_3.rc_control_in);
			

/*			printf("radio %d, control in %d, rate %4.2f\n", copter.failsafe.radio, \
//										copter.channel_throttle->get_control_in(), \
//										copter.get_pilot_desired_climb_rate(copter.channel_throttle->get_rc_control_in()));
*/			
			if(\
				(!copter.failsafe.radio) && \
				(0 != abs(copter.get_pilot_desired_climb_rate(copter.channel_throttle->get_rc_control_in())))\
			)
			{
				gcs().send_text(MAV_SEVERITY_CRITICAL, "GCS control, RC rc3 should be middle");

//				printf("GCS control, RC rc3 should be middle %d\n", g.rc_3.rc_control_in);
				return false;
			}

			
		}
	}
	


	if(true == copter.fs_mk.control_present)
	{
		if(true == copter.fs_mk.not_consist)
		{
			gcs().send_text(MAV_SEVERITY_CRITICAL, "remote control error, reset system");
		
			return false;
		}

		if(1 == copter.fs_mk.gcs_control)
		{
			if(ARM_SOURCE_RC == arming_from_gcs)
			{
				
				gcs().send_text(MAV_SEVERITY_CRITICAL, "GCS control, RC arming not allowed");

				return false;
			}

			//	in this case, pilot will use gcs to control the uav, not use rc to save the uav

			
		}
	}
	/*
	there is no gcs control information upcoming, there is no gcs connecting,
	so this must be a radio control version
	*/

#endif
	//	added end

#if LOGGING_ENABLED == ENABLED

	 if (checks_to_perform == ARMING_CHECK_ALL)
	 {
	 	if(_dateflash.get_num_logs() >= _dateflash.get_num_logs_max())
		{
			if (display_failure) {
                gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: log numbers exceed max");
            }

			return false;
		}
	 }
#endif

	

	//	added by zhangyong for ekf variable check 
	Location temp_loc;
	
    if (ahrs.get_origin(temp_loc) && \
		(!copter.ap.usb_connected) && \
		(copter.g.fs_ekf_thresh > 0.0f) && \
		(checks_to_perform == ARMING_CHECK_ALL)) 
	{
		ekf_check_state_ptr = copter.get_ekf_check_state();

	
		if(true == ekf_check_state_ptr->bad_variance)
		{
			if (display_failure) 
			{	
				switch(ekf_check_state_ptr->type)
				{
					case EKF_VARIANCE_MAG:
						gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: EKF variance MAG");
						break;

					case EKF_VARIANCE_VEL:
						gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: EKF variance VEL");
						break;
							
					case EKF_VARIANCE_POS:
						gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: EKF variance POS");
						break;
							
					default:
						gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: EKF variance");
						break;
				}
			}

			return false;
			
		}
    }

	//	added by zhangyong 20180814
	//	compass variance check 
		// check EKF compass variance is below failsafe threshold
    float vel_variance, pos_variance, hgt_variance, tas_variance;
    Vector3f mag_variance;
    Vector2f offset;
    _ahrs_navekf.get_variances(vel_variance, pos_variance, hgt_variance, mag_variance, tas_variance, offset);
    if (mag_variance.length() >= copter.g.fs_ekf_thresh) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: EKF compass variance");
        }
        return false;
    }
	//	

	
	//	added end


    // check if safety switch has been pushed
    if (hal.util->safety_switch_state() == AP_HAL::Util::SAFETY_DISARMED) {
        if (display_failure) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Arm: Safety Switch");
        }
        return false;
    }

    // superclass method should always be the last thing called; it
    // has side-effects which would need to be cleaned up if one of
    // our arm checks failed
    return AP_Arming::arm_checks(arming_from_gcs);
}

enum HomeState AP_Arming_Copter::home_status() const
{
    return copter.ap.home_state;
}

void AP_Arming_Copter::set_pre_arm_check(bool b)
{
//	printf("set_pre_arm_check %d\n", b);
    if(copter.ap.pre_arm_check != b) {
        copter.ap.pre_arm_check = b;
        AP_Notify::flags.pre_arm_check = b;
    }
}

void AP_Arming_Copter::set_pre_arm_rc_check(bool b)
{
    if(copter.ap.pre_arm_rc_check != b) {
        copter.ap.pre_arm_rc_check = b;
    }
}
