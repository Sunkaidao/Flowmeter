/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include "RangeFinder.h"

class AP_RangeFinder_Backend
{
public:
    // constructor. This incorporates initialisation as well.
	AP_RangeFinder_Backend(RangeFinder::RangeFinder_State &_state);

    // we declare a virtual destructor so that RangeFinder drivers can
    // override with a custom destructor if need be
    virtual ~AP_RangeFinder_Backend(void) {}

    // update the state structure
    virtual void update() = 0;

    virtual void handle_msg(mavlink_message_t *msg) { return; }

    void update_pre_arm_check();

    uint8_t instance() const { return state.instance; }
    enum Rotation orientation() const { return (Rotation)state.orientation.get(); }
    uint16_t distance_cm() const { return state.distance_cm; }
    uint16_t voltage_mv() const { return state.voltage_mv; }
    int16_t max_distance_cm() const { return state.max_distance_cm; }
    int16_t min_distance_cm() const { return state.min_distance_cm; }
    int16_t ground_clearance_cm() const { return state.ground_clearance_cm; }
    MAV_DISTANCE_SENSOR get_mav_distance_sensor_type() const {
        if (state.type == RangeFinder::RangeFinder_TYPE_NONE) {
            return MAV_DISTANCE_SENSOR_UNKNOWN;
        }
        return _get_mav_distance_sensor_type();
    }
    RangeFinder::RangeFinder_Status status() const {
        if (state.type == RangeFinder::RangeFinder_TYPE_NONE) {
            // turned off at runtime?
            return RangeFinder::RangeFinder_NotConnected;
        }

		//	added by zhangyong 20180201
/*		if(0 == state.enable)
//		{
//			return RangeFinder::RangeFinder_NotConnected;
		}
*/		//	added end
        return state.status;
    }
	//	modified by zhangyong 20180201
		RangeFinder::RangeFinder_Type type();
	//	modified end
    

    // true if sensor is returning data
    bool has_data() const {

//		printf("has_data %d\n", state.status);
		
        return ((state.status != RangeFinder::RangeFinder_NotConnected) &&
                (state.status != RangeFinder::RangeFinder_NoData));
    }

    // returns count of consecutive good readings
    uint8_t range_valid_count() const { return state.range_valid_count; }

    // return a 3D vector defining the position offset of the sensor
    // in metres relative to the body frame origin
    const Vector3f &get_pos_offset() const { return state.pos_offset; }

	RangeFinder::RangeFinder_State &get_state() const {return state;}
//added by xusiming in 20180508 and used for collect the status about sensor
	uint16_t RangeFinder_error_count() const {return (state.RangeFinder_error_count*100);}
	uint16_t RangeFinder_no_target_count() const {return (state.RangeFinder_no_target_count*100);}
	uint16_t RangeFinder_unvalid_num() const{return (state.RangeFinder_unvalid_num*100);}
	uint8_t RangeFinder_message_condition() const{return (state.RangeFinder_message_condition);}
	//added by xusiming in 20180827 and used for gkxn  proximity radar
	void get_gxkn_down_data(uint16_t downdata,bool warning);
	uint16_t gkxn_down_data;
	bool gkxn_down_warning;
	//added end
	//added by xusiming 20180510
//    uint8_t RangeFinder_error=0;
//	uint8_t RangeFinder_no_target=0;
//	uint8_t RangeFinder_valid=0;
//added end
protected:

    // update status based on distance measurement
    void update_status();

    // set status and update valid_count
    void set_status(RangeFinder::RangeFinder_Status status);

    RangeFinder::RangeFinder_State &state;

    // semaphore for access to shared frontend data
    AP_HAL::Semaphore *_sem;    

    virtual MAV_DISTANCE_SENSOR _get_mav_distance_sensor_type() const = 0;
};
