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
const uint8_t MT_MAIN_SCREEN[]                          = "Current profile:";
    const uint8_t MT_SETTINGS[]                             = "Settings";
        const uint8_t MT_EDIT_PROFILE[]                     = "Edit Profile";
            const uint8_t MT_PROFILE_LEAD[]                 = "Leaded";
                const uint8_t MT_PROFILE_PREHEAT_TIME[]     = "Pre-heat time:";
                const uint8_t MT_PROFILE_PREHEAT_TARGET[]   = "Pre-heat target:";
                const uint8_t MT_PROFILE_SOAK_TIME[]        = "Soak time:";
                const uint8_t MT_PROFILE_SOAK_TARGET[]      = "Soak target:";
                const uint8_t MT_PROFILE_REFLOW_TEMP[]      = "Reflow temp:";
            const uint8_t MT_PROFILE_LEAD_FREE[]            = "Lead-Free";
                // pre-heat time
                // pre-heat target
                // soak time
                // soak target
                // reflow temp
        const uint8_t MT_CHANGE_UNITS[] = "Change Units";
            const uint8_t MT_UNITS[]    = "\x01 \x00\x46    \x01 \0x00\x43";

const Profile_t Profiles[] = {
    {MT_PROFILE_LEAD, 60, 150, 90, 180, 210},
    {MT_PROFILE_LEAD_FREE, 60, 150, 90, 220, 235}
};


void EditPreheatTime(void){
    lcdClear();
    lcdPrint(MT_PROFILE_PREHEAT_TIME);
}

void EditPreheatTarget(void){
    
}

void EditSoakTime(void){
    
}

void EditSoakTarget(void){
    
}

void EditReflowTemp(void){
    
}
