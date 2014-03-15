/*
 *  BlitzExpansion is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */
 
#include "BlitzPID.h"

BlitzPID::BlitzPID(float kp, float ki, float kd)
{
    this->m_kp = kp;
    this->m_ki = ki;
    this->m_kd = kd;
    
    this->m_set_point = 0;
    this->m_last_integral = 0;
    this->m_last_error = 0;
    this->m_last_millis = 0;
}

void BlitzPID::set(float set_point, blitz_u32 millis) {
    this->m_set_point = set_point;
    this->m_last_millis = millis;
    this->m_last_integral = 0;
    this->m_last_error = 0;
}

float BlitzPID::get_pid(float actual_value, blitz_u32 millis, float Ts) {

    if (this->m_last_millis == 0) {
        // if we have not got a last millis then we have no set point
        return 0;
    }

    float delta_time = (float)(millis - this->m_last_millis) / 1000.0;
    float error = this->m_set_point - actual_value;
    
    float p = this->m_kp * error;
    float i = Ts * this->m_ki * (this->m_last_integral + error) * delta_time;
    float d = (1.0/Ts) * this->m_kd * ((error - this->m_last_error) / delta_time);

    this->m_last_millis = millis;
    this->m_last_integral = i;
    this->m_last_error = error;
    
    return p + i + d;
}

float BlitzPID::update(float actual_value, blitz_u32 millis) {
    return this->get_pid(actual_value, millis, 1.0);
}

float BlitzPID::update(float actual_value, blitz_u32 millis, float Ts) {
    return this->get_pid(actual_value, millis, Ts);
}