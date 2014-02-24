/*
 *  BlitzExpansion is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */

#ifndef _BLITZ_PID_DEFINES_

#include "blitz_payload.h"

class BlitzPID
{
    private:
        float m_ki;
        float m_kd;
        float m_kp;
        
        float m_set_point;
        float m_last_integral;
        float m_last_error;
        
        blitz_u32 m_last_millis;
        
        
        
        float m_last_delta;
        
    public:
        BlitzPID(float kp, float ki, float kd);
        void set(float set_point, blitz_u32 millis);
        float update(float actual_value, blitz_u32 millis);
        
        
        float get_last_integral();
        float get_last_error();
        float get_last_delta();
        blitz_u32 get_last_millis();
};

#endif
