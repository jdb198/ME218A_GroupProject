/****************************************************************************
 Module
   TemplateService.c

 Revision
   1.0.1

 Description
   This is a template file for implementing a simple service under the
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/16/12 09:58 jec      began conversion from TemplateFSM.c
****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
*/
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "MoveServosService.h"
#include <stdio.h>
#include <stdlib.h>
#include "PWM_PIC32.h"

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
*/

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;
uint32_t random_angle_to_pulsewidth();
void delay_ms(uint32_t ms);

#define ONE_SEC 1000
#define HALF_SEC (ONE_SEC / 2)
#define TWO_SEC (ONE_SEC * 2)
#define FIVE_SEC (ONE_SEC * 5)
#define SIXTY_SEC (ONE_SEC * 60)
#define PBCLK_RATE 20000000L
#define TIMER_DIV 8
#define TICS_PER_MS 2500
#define SERVO_PERIOD  (20*TICS_PER_MS)

/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
 Function
     InitTemplateService

 Parameters
     uint8_t : the priorty of this service

 Returns
     bool, false if error in initialization, true otherwise

 Description
     Saves away the priority, and does any
     other required initialization for this service
 Notes

 Author
     J. Edward Carryer, 01/16/12, 10:00
****************************************************************************/
bool InitMoveServosService(uint8_t Priority)
{
  ES_Event_t ThisEvent;

  MyPriority = Priority;
  
  PWMSetup_BasicConfig(4);
  PWMSetup_MapChannelToOutputPin(1, PWM_RPB3);
  PWMSetup_MapChannelToOutputPin(2, PWM_RPB8);
  PWMSetup_AssignChannelToTimer(1, _Timer2_);
  PWMSetup_AssignChannelToTimer(2, _Timer2_);
  PWMSetup_SetFreqOnTimer(50, _Timer2_);  // 50 Hz servo frequency
  PWMOperate_SetDutyOnChannel(50, 1);
  PWMOperate_SetDutyOnChannel(50, 2);
  /********************************************
   in here you write your initialization code
   *******************************************/
  // post the initial transition event
  ThisEvent.EventType = ES_INIT_SERVOS;
  if (ES_PostToService(MyPriority, ThisEvent) == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/****************************************************************************
 Function
     PostTemplateService

 Parameters
     EF_Event_t ThisEvent ,the event to post to the queue

 Returns
     bool false if the Enqueue operation failed, true otherwise

 Description
     Posts an event to this state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
****************************************************************************/
bool PostMoveServosService(ES_Event_t ThisEvent)
{
  return ES_PostToService(MyPriority, ThisEvent);
}

/****************************************************************************
 Function
    RunTemplateService

 Parameters
   ES_Event_t : the event to process

 Returns
   ES_Event, ES_NO_EVENT if no error ES_ERROR otherwise

 Description
   add your description here
 Notes

 Author
   J. Edward Carryer, 01/15/12, 15:23
****************************************************************************/
ES_Event_t RunMoveServosService(ES_Event_t ThisEvent)
{
  ES_Event_t ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

  switch (ThisEvent.EventType)
  {
    case ES_WELCOME_DISPLAY:        // If current state is initial Psedudo State
    {}
    break;

    case ES_GHOST_JERK:        // If current state is state one
    {
        DB_printf("The ghost has moved out of fright \n");
        int32_t down_ticks = 1750;
        PWMOperate_SetPulseWidthOnChannel(down_ticks, 1);
        PWMOperate_SetPulseWidthOnChannel(down_ticks, 2);
    /* AYTAN ADD CODE HERE */

    }
    break;
    
    case ES_GHOST_TIMER:        // If current state is state one
    {
        DB_printf("The ghost has moved after 5 seconds \n");
      //srand(12345);   // seed random generator
        uint32_t pulse1 = random_angle_to_pulsewidth();
        uint32_t pulse2 = random_angle_to_pulsewidth();

        PWMOperate_SetPulseWidthOnChannel(pulse1, 1);
        PWMOperate_SetPulseWidthOnChannel(pulse2, 2);

    }
    break;
    
    case ES_GAME_OVER:        // If current state is state one
    {
        DB_printf("Dispense the total number of gears depending on the total points \n");
    /* AYTAN ADD CODE HERE */
    }
    break;

    // repeat state pattern as required for other states
     default:
    {}
     break;
  }                                  // end switch on Current State
  return ReturnEvent;
}


/***************************************************************************
 private functions
 ***************************************************************************/




uint32_t random_angle_to_pulsewidth()
{
   // delay_ms();
    uint32_t angle = rand() % 181;   // 0?180 degrees

    // Desired pulse in microseconds
    uint32_t pulse_us = 700 + ((angle * (2400 - 700)) / 180);

    // Convert µs -> timer ticks: TICS_PER_MS = 2500 ticks/ms
    // 1 ms = 1000 µs, so ticks = pulse_us * 2500 / 1000
    uint32_t pulse_ticks = (pulse_us * TICS_PER_MS) / 1000;

    return pulse_ticks;
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) {}
}

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

