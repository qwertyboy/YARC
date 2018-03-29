/*
 * menu.c
 *
 * Created: 3/20/2018 10:01:09 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "menu.h"
#include "lcd.h"

// menu text items
const char MT_MAIN_SCREEN[]                              = "Current profile:";
    const char MT_SETTINGS[]                             = "Settings";
        const char MT_EDIT_PROFILE[]                     = "Edit Profile";
            const char MT_PROFILE_LEAD[]                 = "Lead";
                const char MT_PROFILE_PREHEAT_TIME[]     = "Pre-heat time:";
                const char MT_PROFILE_PREHEAT_TARGET[]   = "Pre-heat target:";
                const char MT_PROFILE_SOAK_TIME[]        = "Soak time:";
                const char MT_PROFILE_SOAK_TARGET[]      = "Soak target:";
                const char MT_PROFILE_REFLOW_TEMP[]      = "Reflow temp:";
            const char MT_PROFILE_LEAD_FREE[]            = "Lead-Free";
                // pre-heat time
                // pre-heat target
                // soak time
                // soak target
                // reflow temp
        const char MT_CHANGE_UNITS[] = "Change Units";
            const char MT_UNITS[]    = "\x01 \x00\x46    \x01 \0x00\x43";
            
const char STATUS_PREHEAT[]  = "Pre-heat";
const char STATUS_SOAK[]     = "Soak";
const char STATUS_REFLOW[]   = "Reflow";
const char STATUS_COOLDOWN[] = "Cool-down";

const Profile_t Profiles[] = {
    {MT_PROFILE_LEAD, 60, 30, 90, 180, 210},
    {MT_PROFILE_LEAD_FREE, 60, 150, 90, 220, 235}
};

const char * PhaseText[] = {
    STATUS_PREHEAT,
    STATUS_SOAK,
    STATUS_REFLOW,
    STATUS_COOLDOWN
};


void EditPreheatTime(void){
    LcdClear();
    LcdPrint(MT_PROFILE_PREHEAT_TIME);
}

void EditPreheatTarget(void){
    
}

void EditSoakTime(void){
    
}

void EditSoakTarget(void){
    
}

void EditReflowTemp(void){
    
}
