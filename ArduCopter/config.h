#pragma once

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
//  DO NOT EDIT this file to adjust your configuration.  Create your own
//  APM_Config.h and use APM_Config.h.example as a reference.
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
///
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Default and automatic configuration details.
//
// Notes for maintainers:
//
// - Try to keep this file organised in the same order as APM_Config.h.example
//
#include "defines.h"

///
/// DO NOT EDIT THIS INCLUDE - if you want to make a local change, make that
/// change in your local copy of APM_Config.h.
///
#include "APM_Config.h"


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// HARDWARE CONFIGURATION AND CONNECTIONS
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_HAL_BOARD
#error CONFIG_HAL_BOARD must be defined to build ArduCopter
#endif

//////////////////////////////////////////////////////////////////////////////
// HIL_MODE                                 OPTIONAL

#ifndef HIL_MODE
 #define HIL_MODE        HIL_MODE_DISABLED
#endif

#define MAGNETOMETER ENABLED

// run at 400Hz on all systems
# define MAIN_LOOP_RATE    400
# define MAIN_LOOP_SECONDS 0.0025f
# define MAIN_LOOP_MICROS  2500

#ifndef ARMING_DELAY_SEC
    # define ARMING_DELAY_SEC 2.0f
#endif

//////////////////////////////////////////////////////////////////////////////
// FRAME_CONFIG
//
#ifndef FRAME_CONFIG
 # define FRAME_CONFIG   MULTICOPTER_FRAME
#endif

/////////////////////////////////////////////////////////////////////////////////
// TradHeli defaults
#if FRAME_CONFIG == HELI_FRAME
  # define RC_FAST_SPEED                        125
  # define WP_YAW_BEHAVIOR_DEFAULT              WP_YAW_BEHAVIOR_LOOK_AHEAD
  # define THR_MIN_DEFAULT                      0
  # define AUTOTUNE_ENABLED                     DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// PWM control
// default RC speed in Hz
#ifndef RC_FAST_SPEED
   #   define RC_FAST_SPEED 490
#endif

//////////////////////////////////////////////////////////////////////////////
// Rangefinder
//

//	modified by ZhangYong 20170915
//#ifndef RANGEFINDER_ENABLED
// # define RANGEFINDER_ENABLED 
//#endif
//	modified end

#ifndef RANGEFINDER_ENABLED
 # define RANGEFINDER_ENABLED ENABLED
#endif

#ifndef RANGEFINDER_HEALTH_MAX
 # define RANGEFINDER_HEALTH_MAX 3          // number of good reads that indicates a healthy rangefinder
#endif

#ifndef RANGEFINDER_GAIN_DEFAULT
 # define RANGEFINDER_GAIN_DEFAULT 0.8f     // gain for controlling how quickly rangefinder range adjusts target altitude (lower means slower reaction)
#endif

#ifndef THR_SURFACE_TRACKING_VELZ_MAX
 # define THR_SURFACE_TRACKING_VELZ_MAX 150 // max vertical speed change while surface tracking with rangefinder
#endif

#ifndef RANGEFINDER_TIMEOUT_MS
 # define RANGEFINDER_TIMEOUT_MS  1000      // desired rangefinder alt will reset to current rangefinder alt after this many milliseconds without a good rangefinder alt
#endif

#ifndef RANGEFINDER_WPNAV_FILT_HZ
 # define RANGEFINDER_WPNAV_FILT_HZ   0.25f // filter frequency for rangefinder altitude provided to waypoint navigation class
#endif

#ifndef RANGEFINDER_TILT_CORRECTION         // by disable tilt correction for use of range finder data by EKF
 # define RANGEFINDER_TILT_CORRECTION ENABLED
#endif

#ifndef RANGEFINDER_GLITCH_ALT_CM
 # define RANGEFINDER_GLITCH_ALT_CM  200      // amount of rangefinder change to be considered a glitch
#endif

#ifndef RANGEFINDER_GLITCH_NUM_SAMPLES
 # define RANGEFINDER_GLITCH_NUM_SAMPLES  3   // number of rangefinder glitches in a row to take new reading
#endif

//////////////////////////////////////////////////////////////////////////////
// Proximity sensor
//
//	modified by ZhangYong 20170915
//#ifndef PROXIMITY_ENABLED
// # define PROXIMITY_ENABLED ENABLED
//#endif

//	modified end

#ifndef PROXIMITY_ENABLED
 # define PROXIMITY_ENABLED ENABLED
#endif

#ifndef MAV_SYSTEM_ID
 # define MAV_SYSTEM_ID          1
#endif


//////////////////////////////////////////////////////////////////////////////
// Battery monitoring
//
#ifndef FS_BATT_VOLTAGE_DEFAULT
 # define FS_BATT_VOLTAGE_DEFAULT       10.5f       // default battery voltage below which failsafe will be triggered
#endif

#ifndef FS_BATT_MAH_DEFAULT
 # define FS_BATT_MAH_DEFAULT             0         // default battery capacity (in mah) below which failsafe will be triggered
#endif

#ifndef BOARD_VOLTAGE_MIN
 # define BOARD_VOLTAGE_MIN             4.3f        // min board voltage in volts for pre-arm checks
#endif

#ifndef BOARD_VOLTAGE_MAX
 # define BOARD_VOLTAGE_MAX             5.8f        // max board voltage in volts for pre-arm checks
#endif

// prearm GPS hdop check
#ifndef GPS_HDOP_GOOD_DEFAULT
 # define GPS_HDOP_GOOD_DEFAULT         140     // minimum hdop that represents a good position.  used during pre-arm checks if fence is enabled
#endif

// GCS failsafe
#ifndef FS_GCS
 # define FS_GCS                        ENABLED
#endif
#ifndef FS_GCS_TIMEOUT_MS
 # define FS_GCS_TIMEOUT_MS             5000    // gcs failsafe triggers after 5 seconds with no GCS heartbeat
#endif

// Radio failsafe while using RC_override
#ifndef FS_RADIO_RC_OVERRIDE_TIMEOUT_MS
 # define FS_RADIO_RC_OVERRIDE_TIMEOUT_MS  1000    // RC Radio failsafe triggers after 1 second while using RC_override from ground station
#endif

// Radio failsafe
#ifndef FS_RADIO_TIMEOUT_MS
 #define FS_RADIO_TIMEOUT_MS            500     // RC Radio Failsafe triggers after 500 miliseconds with No RC Input
#endif

// missing terrain data failsafe
#ifndef FS_TERRAIN_TIMEOUT_MS
 #define FS_TERRAIN_TIMEOUT_MS          5000     // 5 seconds of missing terrain data will trigger failsafe (RTL)
#endif

#ifndef PREARM_DISPLAY_PERIOD
# define PREARM_DISPLAY_PERIOD 30
#endif

// pre-arm baro vs inertial nav max alt disparity
#ifndef PREARM_MAX_ALT_DISPARITY_CM
 # define PREARM_MAX_ALT_DISPARITY_CM       100     // barometer and inertial nav altitude must be within this many centimeters
#endif

//////////////////////////////////////////////////////////////////////////////
//  EKF Failsafe
#ifndef FS_EKF_ACTION_DEFAULT
 # define FS_EKF_ACTION_DEFAULT         FS_EKF_ACTION_LAND  // EKF failsafe triggers land by default
#endif
#ifndef FS_EKF_THRESHOLD_DEFAULT
 # define FS_EKF_THRESHOLD_DEFAULT      0.8f    // EKF failsafe's default compass and velocity variance threshold above which the EKF failsafe will be triggered
#endif

#ifndef EKF_ORIGIN_MAX_DIST_M
 # define EKF_ORIGIN_MAX_DIST_M         50000   // EKF origin and waypoints (including home) must be within 50km
#endif

//////////////////////////////////////////////////////////////////////////////
//  MAGNETOMETER
#ifndef MAGNETOMETER
 # define MAGNETOMETER                   ENABLED
#endif

#ifndef COMPASS_CAL_STICK_GESTURE_TIME
 #define COMPASS_CAL_STICK_GESTURE_TIME 2.0f // 2 seconds
#endif
#ifndef COMPASS_CAL_STICK_DELAY
 #define COMPASS_CAL_STICK_DELAY 5.0f
#endif

//////////////////////////////////////////////////////////////////////////////
//  OPTICAL_FLOW & VISUAL ODOMETRY
//	modified by ZhangYong 20170915
//#ifndef OPTFLOW
// # define OPTFLOW       ENABLED
//#endif
//#ifndef VISUAL_ODOMETRY_ENABLED
//# define VISUAL_ODOMETRY_ENABLED ENABLED
//#endif

//	modified end
//	

//	noted by ZhangYong 2017110
//	if we disabled optflow firmware will die after boot 
#ifndef OPTFLOW
 # define OPTFLOW       ENABLED
#endif
#ifndef VISUAL_ODOMETRY_ENABLED
# define VISUAL_ODOMETRY_ENABLED DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
//  Auto Tuning
//	modified by ZhangYong 20170915
//#ifndef AUTOTUNE_ENABLED
// # define AUTOTUNE_ENABLED  ENABLED
//#endif
//#endif
//	modified end

#ifndef AUTOTUNE_ENABLED
 # define AUTOTUNE_ENABLED  DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
//  Crop Sprayer
//	modified by ZhangYong 20170915

//#ifndef SPRAYER
// # define SPRAYER  ENABLED
//#endif
//	endif





//////////////////////////////////////////////////////////////////////////////
//  Pass OSD
//	added by ZhangYong 20170712
#ifndef PJTPASSOSD
 # define PJTPASSOSD DISABLED
#endif
//	added ebd


//////////////////////////////////////////////////////////////////////////////
//  PROJECT GKXN
//	added by ZhangYong 20170712
#ifndef PROJECTGKXN
 # define PROJECTGKXN ENABLED
#endif
//	added ebd

//////////////////////////////////////////////////////////////////////////////
//  RADAR NALEI

#ifndef RNGRADAR
 # define RNGRADAR ENABLED
#endif 


#ifndef SPRAYER
 # define SPRAYER  ENABLED
#endif


//////////////////////////////////////////////////////////////////////////////
//  PROJECT BCB



//	added by ZhangYong 20170720
#ifndef PROJECTBCB
 # define PROJECTBCB DISABLED
#endif

#if PROJECTBCB == ENABLED
#ifndef BCBMONITOR
 # define BCBMONITOR DISABLED
#endif

#ifndef BCBPMBUS
 # define BCBPMBUS DISABLED
#endif
#endif

//	added ebd




//////////////////////////////////////////////////////////////////////////////
//  GKXN FLOWMETER
#if PROJECTGKXN == ENABLED
#ifndef FLOWMETER
 # define FLOWMETER  ENABLED
#endif
#endif



//////////////////////////////////////////////////////////////////////////////
// Precision Landing with companion computer or IRLock sensor
//	modified by ZhangYong 20170915
//#ifndef PRECISION_LANDING
// # define PRECISION_LANDING ENABLED
//#endif

//	modified
#ifndef PRECISION_LANDING
 # define PRECISION_LANDING DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
//	gripper
//	modified by ZhangYong 20170915
//#ifndef GRIPPER_ENABLED
// # define GRIPPER_ENABLED ENABLED
//#endif

//	modified end

#ifndef GRIPPER_ENABLED
 # define GRIPPER_ENABLED DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// Parachute release
//	modified by ZhangYong 20170915
//#ifndef PARACHUTE
// # define PARACHUTE ENABLED
//#endif

//	modified end

#ifndef PARACHUTE
 # define PARACHUTE  DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// ADSB support
//	modified by ZhangYong 20170915
//#ifndef ADSB_ENABLED
// # define ADSB_ENABLED ENABLED
//#endif

#ifndef ADSB_ENABLED
# define ADSB_ENABLED DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// Nav-Guided - allows external nav computer to control vehicle
#ifndef NAV_GUIDED
 # define NAV_GUIDED    ENABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// RADIO CONFIGURATION
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// FLIGHT_MODE
//

#ifndef FLIGHT_MODE_1
 # define FLIGHT_MODE_1                  STABILIZE
#endif
#ifndef FLIGHT_MODE_2
 # define FLIGHT_MODE_2                  STABILIZE
#endif
#ifndef FLIGHT_MODE_3
 # define FLIGHT_MODE_3                  STABILIZE
#endif
#ifndef FLIGHT_MODE_4
 # define FLIGHT_MODE_4                  STABILIZE
#endif
#ifndef FLIGHT_MODE_5
 # define FLIGHT_MODE_5                  STABILIZE
#endif
#ifndef FLIGHT_MODE_6
 # define FLIGHT_MODE_6                  STABILIZE
#endif


//////////////////////////////////////////////////////////////////////////////
// Throttle Failsafe
//
#ifndef FS_THR_VALUE_DEFAULT
 # define FS_THR_VALUE_DEFAULT             975
#endif

//////////////////////////////////////////////////////////////////////////////
// Takeoff
//
#ifndef PILOT_TKOFF_ALT_DEFAULT
 # define PILOT_TKOFF_ALT_DEFAULT           0     // default final alt above home for pilot initiated takeoff
#endif


//////////////////////////////////////////////////////////////////////////////
// Landing
//
#ifndef LAND_SPEED
 # define LAND_SPEED    50          // the descent speed for the final stage of landing in cm/s
#endif
#ifndef LAND_START_ALT
 # define LAND_START_ALT 1000         // altitude in cm where land controller switches to slow rate of descent
#endif
#ifndef LAND_REPOSITION_DEFAULT
 # define LAND_REPOSITION_DEFAULT   1   // by default the pilot can override roll/pitch during landing
#endif
#ifndef LAND_WITH_DELAY_MS
 # define LAND_WITH_DELAY_MS        4000    // default delay (in milliseconds) when a land-with-delay is triggered during a failsafe event
#endif
#ifndef LAND_CANCEL_TRIGGER_THR
 # define LAND_CANCEL_TRIGGER_THR   700     // land is cancelled by input throttle above 700
#endif
#ifndef LAND_RANGEFINDER_MIN_ALT_CM
#define LAND_RANGEFINDER_MIN_ALT_CM 200
#endif

//////////////////////////////////////////////////////////////////////////////
// Landing Detector
//
#ifndef LAND_DETECTOR_TRIGGER_SEC
 # define LAND_DETECTOR_TRIGGER_SEC         1.0f    // number of seconds to detect a landing
#endif
#ifndef LAND_DETECTOR_MAYBE_TRIGGER_SEC
 # define LAND_DETECTOR_MAYBE_TRIGGER_SEC   0.2f    // number of seconds that means we might be landed (used to reset horizontal position targets to prevent tipping over)
#endif
#ifndef LAND_DETECTOR_ACCEL_LPF_CUTOFF
# define LAND_DETECTOR_ACCEL_LPF_CUTOFF     1.0f    // frequency cutoff of land detector accelerometer filter
#endif
#ifndef LAND_DETECTOR_ACCEL_MAX
# define LAND_DETECTOR_ACCEL_MAX            1.0f    // vehicle acceleration must be under 1m/s/s
#endif

//////////////////////////////////////////////////////////////////////////////
// CAMERA TRIGGER AND CONTROL
//
//	modified by ZhangYong 20170915
//#ifndef CAMERA
// # define CAMERA        ENABLED
//#endif

//	modified end
#ifndef CAMERA
 # define CAMERA        DISABLED
#endif

//////////////////////////////////////////////////////////////////////////////
// MOUNT (ANTENNA OR CAMERA)
//
//	modified by ZhangYong 20170915
//#ifndef MOUNT
// # define MOUNT        ENABLED
//#endif

#ifndef MOUNT
 # define MOUNT         DISABLED
#endif


//	added by zhangYong 20161109 for auth
//////////////////////////////////////////////////////////////////////////////
// AUTH ()
//
#ifndef FXTX_AUTH
 #define FXTX_AUTH 	ENABLED
#endif

#if FXTX_AUTH == ENABLED
#define AUTH_KEY_POS	0
#define AUTH_KEY_LEN	10
#define AUTH_ID_POS		(AUTH_KEY_POS + AUTH_KEY_LEN)
#define AUTH_ID_LEN		20
#define AUTH_ENTER_POS	(AUTH_ID_POS + AUTH_ID_LEN)
#define AUTH_ID_ARRAY_LEN	(AUTH_ENTER_POS + 4)

enum auth_state {
	auth_state_failed = 0,
	auth_state_denied = 1,
	auth_state_success = 2
};

union auth_id_para {
	float	serial[5];
	uint8_t 	 data[20];
};

//	added by ZhangYong globle variable
struct current_gps_week_ms
{
	uint32_t time_week_ms;              ///< GPS time (milliseconds from start of GPS week)
	uint16_t time_week;                 ///< GPS week number
};

#endif

//	




//#endif



//	added end

//	added by ZhangYong for land detector log 20161114

struct land_detector_log
{
	uint8_t motor_at_lower;
	uint8_t acc_stationary;
	uint8_t descent_rate;
	uint8_t rngfnd_check;
	uint8_t disarm_onland_cfg;
	uint8_t mode_disarm_onland;
	uint8_t land_cmplete;
	uint8_t override;
	uint8_t active;
//	bool acc_stationary;
//	bool descent_rate;
//	bool rngfnd_check;
//	bool disarm_onland_cfg;
//	bool mode_disarm_onland;
//	bool land_cmplete;
	uint32_t cnt;
};

//	added end


//	added by ZhangYong to log the motor calculation






struct PACKED PassOSD_data_struct {
	uint8_t stx0;							//	0
	uint8_t stx1;							//	1
	uint8_t len;							//	2
	uint8_t seq;							//	3
		
	uint16_t source_id;						//	4-5
	uint16_t destination_id;				//	6-7

	uint8_t comp_id;						//	8

	uint8_t message_id;						//	9-10
		
	float voltage_battery;					//	11-14
	float current_total_mah;				//	15-18

	uint8_t fix_type;						//	19
	uint8_t num_sats;						//	20

	uint8_t control_mode;					//	21

	float velocity;							//	22-25
	float climbrate;						//	26-29

	float altitude;							//	30-33

	uint32_t flight_time_sec;				//	34-37
	float yaw;								//	38-41

	uint8_t crc0;							//	42
	uint8_t crc1;							//	43

	uint8_t etx0;							//	44
	uint8_t etx1;							//	45

};

union PACKED PassOSD_data_status {
	PassOSD_data_struct data;
	uint8_t bytes[];
};

//	added 





//////////////////////////////////////////////////////////////////////////////
// Flight mode definitions
//

// Acro Mode
#ifndef ACRO_RP_P
 # define ACRO_RP_P                 4.5f
#endif

#ifndef ACRO_YAW_P
 # define ACRO_YAW_P                4.5f
#endif

#ifndef ACRO_LEVEL_MAX_ANGLE
 # define ACRO_LEVEL_MAX_ANGLE      3000
#endif

#ifndef ACRO_BALANCE_ROLL
 #define ACRO_BALANCE_ROLL          1.0f
#endif

#ifndef ACRO_BALANCE_PITCH
 #define ACRO_BALANCE_PITCH         1.0f
#endif

#ifndef ACRO_RP_EXPO_DEFAULT
 #define ACRO_RP_EXPO_DEFAULT       0.3f
#endif

#ifndef ACRO_Y_EXPO_DEFAULT
 #define ACRO_Y_EXPO_DEFAULT        0.0f
#endif

#ifndef ACRO_THR_MID_DEFAULT
 #define ACRO_THR_MID_DEFAULT       0.0f
#endif

// RTL Mode
#ifndef RTL_ALT_FINAL
 # define RTL_ALT_FINAL             0       // the altitude the vehicle will move to as the final stage of Returning to Launch.  Set to zero to land.
#endif

#ifndef RTL_ALT
 # define RTL_ALT 				    1500    // default alt to return to home in cm, 0 = Maintain current altitude
#endif

#ifndef RTL_ALT_MIN
 # define RTL_ALT_MIN               200     // min height above ground for RTL (i.e 2m)
#endif

#ifndef RTL_CLIMB_MIN_DEFAULT
 # define RTL_CLIMB_MIN_DEFAULT     0       // vehicle will always climb this many cm as first stage of RTL
#endif

#ifndef RTL_ABS_MIN_CLIMB
 # define RTL_ABS_MIN_CLIMB         250     // absolute minimum initial climb
#endif

#ifndef RTL_CONE_SLOPE_DEFAULT
 # define RTL_CONE_SLOPE_DEFAULT    3.0f    // slope of RTL cone (height / distance). 0 = No cone
#endif

#ifndef RTL_MIN_CONE_SLOPE
 # define RTL_MIN_CONE_SLOPE        0.5f    // minimum slope of cone
#endif

#ifndef RTL_LOITER_TIME
 # define RTL_LOITER_TIME           5000    // Time (in milliseconds) to loiter above home before beginning final descent
#endif

// AUTO Mode
#ifndef WP_YAW_BEHAVIOR_DEFAULT
 # define WP_YAW_BEHAVIOR_DEFAULT   WP_YAW_BEHAVIOR_LOOK_AT_NEXT_WP_EXCEPT_RTL
#endif

#ifndef AUTO_YAW_SLEW_RATE
 # define AUTO_YAW_SLEW_RATE    60              // degrees/sec
#endif

#ifndef YAW_LOOK_AHEAD_MIN_SPEED
 # define YAW_LOOK_AHEAD_MIN_SPEED  100             // minimum ground speed in cm/s required before copter is aimed at ground course
#endif

// Super Simple mode
#ifndef SUPER_SIMPLE_RADIUS
 # define SUPER_SIMPLE_RADIUS       1000
#endif

//////////////////////////////////////////////////////////////////////////////
// Stabilize Rate Control
//
#ifndef ROLL_PITCH_YAW_INPUT_MAX
 # define ROLL_PITCH_YAW_INPUT_MAX      4500        // roll, pitch and yaw input range
#endif
#ifndef DEFAULT_ANGLE_MAX
 # define DEFAULT_ANGLE_MAX         4500            // ANGLE_MAX parameters default value
#endif
#ifndef ANGLE_RATE_MAX
 # define ANGLE_RATE_MAX            18000           // default maximum rotation rate in roll/pitch axis requested by angle controller used in stabilize, loiter, rtl, auto flight modes
#endif

//////////////////////////////////////////////////////////////////////////////
// Loiter position control gains
//
#ifndef POS_XY_P
 # define POS_XY_P             	1.0f
#endif

//////////////////////////////////////////////////////////////////////////////
// Stop mode defaults
//
#ifndef BRAKE_MODE_SPEED_Z
 # define BRAKE_MODE_SPEED_Z     250 // z-axis speed in cm/s in Brake Mode
#endif
#ifndef BRAKE_MODE_DECEL_RATE
 # define BRAKE_MODE_DECEL_RATE  750 // acceleration rate in cm/s/s in Brake Mode
#endif

//////////////////////////////////////////////////////////////////////////////
// Velocity (horizontal) gains
//
#ifndef VEL_XY_P
 # define VEL_XY_P              1.0f
#endif
#ifndef VEL_XY_I
 # define VEL_XY_I              0.5f
#endif
#ifndef VEL_XY_IMAX
 # define VEL_XY_IMAX           1000
#endif
#ifndef VEL_XY_FILT_HZ
 # define VEL_XY_FILT_HZ        5.0f
#endif

//////////////////////////////////////////////////////////////////////////////
// PosHold parameter defaults
//
#ifndef POSHOLD_ENABLED
 # define POSHOLD_ENABLED               ENABLED // PosHold flight mode enabled by default
#endif
#ifndef POSHOLD_BRAKE_RATE_DEFAULT
 # define POSHOLD_BRAKE_RATE_DEFAULT    8       // default POSHOLD_BRAKE_RATE param value.  Rotation rate during braking in deg/sec
#endif
#ifndef POSHOLD_BRAKE_ANGLE_DEFAULT
 # define POSHOLD_BRAKE_ANGLE_DEFAULT   3000    // default POSHOLD_BRAKE_ANGLE param value.  Max lean angle during braking in centi-degrees
#endif

//////////////////////////////////////////////////////////////////////////////
// Throttle control gains
//

#ifndef THR_DZ_DEFAULT
# define THR_DZ_DEFAULT         100             // the deadzone above and below mid throttle while in althold or loiter
#endif

#ifndef ALT_HOLD_P
 # define ALT_HOLD_P            1.0f
#endif

// Velocity (vertical) control gains
#ifndef VEL_Z_P
 # define VEL_Z_P       5.0f
#endif

// Accel (vertical) control gains
#ifndef ACCEL_Z_P
 # define ACCEL_Z_P     0.50f
#endif
#ifndef ACCEL_Z_I
 # define ACCEL_Z_I     1.00f
#endif
#ifndef ACCEL_Z_D
 # define ACCEL_Z_D     0.0f
#endif
#ifndef ACCEL_Z_IMAX
 # define ACCEL_Z_IMAX  800
#endif
#ifndef ACCEL_Z_FILT_HZ
 # define ACCEL_Z_FILT_HZ   20.0f
#endif

// default maximum vertical velocity and acceleration the pilot may request
#ifndef PILOT_VELZ_MAX
 # define PILOT_VELZ_MAX    250     // maximum vertical velocity in cm/s
#endif
#ifndef PILOT_ACCEL_Z_DEFAULT
 # define PILOT_ACCEL_Z_DEFAULT 250 // vertical acceleration in cm/s/s while altitude is under pilot control
#endif

// max distance in cm above or below current location that will be used for the alt target when transitioning to alt-hold mode
#ifndef ALT_HOLD_INIT_MAX_OVERSHOOT
 # define ALT_HOLD_INIT_MAX_OVERSHOOT 200
#endif
// the acceleration used to define the distance-velocity curve
#ifndef ALT_HOLD_ACCEL_MAX
 # define ALT_HOLD_ACCEL_MAX 250    // if you change this you must also update the duplicate declaration in AC_WPNav.h
#endif

#ifndef AUTO_DISARMING_DELAY
# define AUTO_DISARMING_DELAY  10
#endif

//////////////////////////////////////////////////////////////////////////////
// Throw mode configuration
//
#ifndef THROW_HIGH_SPEED
# define THROW_HIGH_SPEED       500.0f  // vehicle much reach this total 3D speed in cm/s (or be free falling)
#endif
#ifndef THROW_VERTICAL_SPEED
# define THROW_VERTICAL_SPEED   50.0f   // motors start when vehicle reaches this total 3D speed in cm/s
#endif

//////////////////////////////////////////////////////////////////////////////
// Dataflash logging control
//
#ifndef LOGGING_ENABLED
 # define LOGGING_ENABLED                ENABLED
#endif

// Default logging bitmask
#ifndef DEFAULT_LOG_BITMASK
 # define DEFAULT_LOG_BITMASK \
    MASK_LOG_ATTITUDE_MED | \
    MASK_LOG_GPS | \
    MASK_LOG_PM | \
    MASK_LOG_CTUN | \
    MASK_LOG_NTUN | \
    MASK_LOG_RCIN | \
    MASK_LOG_IMU | \
    MASK_LOG_CMD | \
    MASK_LOG_CURRENT | \
    MASK_LOG_RCOUT | \
    MASK_LOG_OPTFLOW | \
    MASK_LOG_COMPASS | \
    MASK_LOG_CAMERA | \
    MASK_LOG_MOTBATT
#endif

//////////////////////////////////////////////////////////////////////////////
// Fence, Rally and Terrain and AC_Avoidance defaults
//

// Enable/disable Fence
#ifndef AC_FENCE
 #define AC_FENCE ENABLED
#endif

//	modified by ZhangYong 20170915
//#ifndef AC_RALLY
// #define AC_RALLY   ENABLED
//#endif

//#ifndef AC_TERRAIN
//#define AC_TERRAIN ENABLED
//#endif

//#if AC_TERRAIN && !AC_RALLY
// #error Terrain relies on Rally which is disabled
//#endif

//#ifndef AC_AVOID_ENABLED
// #define AC_AVOID_ENABLED   ENABLED
//#endif

//	modified end
#ifndef AC_RALLY
 #define AC_RALLY   DISABLED
#endif

#ifndef AC_TERRAIN
 #define AC_TERRAIN DISABLED
#endif

#if AC_TERRAIN && !AC_RALLY
 #error Terrain relies on Rally which is disabled
#endif

#ifndef AC_AVOID_ENABLED
 #define AC_AVOID_ENABLED   ENABLED
#endif

#if AC_AVOID_ENABLED && !PROXIMITY_ENABLED
  #error AC_Avoidance relies on PROXIMITY_ENABLED which is disabled
#endif
#if AC_AVOID_ENABLED && !AC_FENCE
  #error AC_Avoidance relies on AC_FENCE which is disabled
#endif

//////////////////////////////////////////////////////////////////////////////
// Developer Items
//

//use this to completely disable FRSKY TELEM
//	modified by ZhangYong 20170915
//fndef FRSKY_TELEM_ENABLED
//#  define FRSKY_TELEM_ENABLED          ENABLED
//ndif

//	modified end
#ifndef FRSKY_TELEM_ENABLED
  #  define FRSKY_TELEM_ENABLED          DISABLE
#endif

#ifndef ADVANCED_FAILSAFE
# define ADVANCED_FAILSAFE DISABLED
#endif


//baiyang added in 20170620
///////////////////////////////////////////////////////////////////////////////
//DGPS relate
//

//Use GPS #HEADINGA messages
#ifndef DGPS_HEADINGA
  #  define DGPS_HEADINGA          ENABLED
#endif

//This macro is defined in AP_NavEKF2_core.h,in this file, AP_NavEKF2_core.h can not be seen AP_NavEKF2.h
//Use the #HEADINGA message to resolve the heading,can not use this method, this method compile switch does not work
/*
* #ifndef GPS_YAW_CAL
* # define GPS_YAW_CAL   ENABLED
* #endif
*/

//added end


#ifndef RF_TASK
#  define RF_TASK	ENABLED
#endif

#if RF_TASK == ENABLED

//baiyang added in 20170712
///////////////////////////////////////////////////////////////////////////////
//charging Station
//
#ifndef CHARGINGSTATION
  #  define CHARGINGSTATION	ENABLED
#endif

//baiyang added in 20170712
///////////////////////////////////////////////////////////////////////////////
//AB Mode
//
#ifndef ABMODE
  #  define ABMODE	ENABLED
#endif

#endif

// baiyang added in 20170830
//////////////////////////////////////////////////////////////////////////////
//roflying fence
#ifndef RF_FENCE
#  define RF_FENCE	DISABLE
#endif

// baiyang added in 20170830
//////////////////////////////////////////////////////////////////////////////
//roflying fence
#ifndef PTZ_CONTROL
#  define PTZ_CONTROL	DISABLED
#endif




//baiyang added in 20170829
/////////////////////////////////////////////////////////////////////////////////////
//Enabled UserCode.cpp
#define USERHOOK_INIT
#define USERHOOK_MEDIUMLOOP
#define USERHOOK_50HZLOOP
#define USERHOOK_SUPERSLOWLOOP





