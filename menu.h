/*
 * menu.h
 *
 * Created: 3/17/2018 4:52:45 PM
 *  Author: Nathan
 */ 


#ifndef MENU_H_
#define MENU_H_

#define NUM_PROFILES        2
#define MAIN_MENU_LEN       2
#define PROFILE_OPTIONS_LEN 5
// inputs to change menu levels
#define MENU_NEXT_LEVEL     1
#define MENU_PREV_LEVEL     2
// menu states
#define MENU_STATE_MAIN          0
#define MENU_STATE_SETTINGS      1
#define MENU_STATE_LIST_PROFILES 2
#define MENU_STATE_EDIT_PROFILE  3
#define MENU_STATE_CHANGE_UNITS  4

typedef struct Profile{
    const char * profileName;
    uint16_t preHeatTime;
    uint16_t preHeatTemp;
    uint16_t soakTime;
    uint16_t soakTemp;
    uint16_t reflowTemp;
} Profile_t;

typedef struct MenuLevel{
    const uint8_t * title;
    const uint8_t * item0;
    const uint8_t * item1;
    const uint8_t * item2;
    const uint8_t * item3;
    const uint8_t * item4;
    void (* func_ptr)(void);
} MenuLevel_t;


extern const Profile_t Profiles[];
extern const char * PhaseText[];

#endif /* MENU_H_ */