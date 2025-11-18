/****************************************************************************
 Module
   TemplateFSM.c

 Revision
   1.0.1

 Description
   This is a template file for implementing flat state machines under the
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/12 11:12 jec      revisions for Gen2 framework
 11/07/11 11:26 jec      made the queue static
 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
*/
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "GhostHuntFSM.h"
#include <proc/PIC32MX/p32mx170f256b.h>
#include "PWM_PIC32.h"

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
*/

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
// type of state variable should match htat of enum in header file
static GhostHuntFSM_t CurrentState;
static GhostHuntFSM_t  NextState;

#define ONE_SEC 1000
#define HALF_SEC (ONE_SEC / 2)
#define TWO_SEC (ONE_SEC * 2)
#define FIVE_SEC (ONE_SEC * 5)
#define SIXTY_SEC (ONE_SEC * 60)

#define PBCLK_RATE 20000000L
#define TIMER_DIV 8
#define TICS_PER_MS 2500
#define SERVO_PERIOD  (20*TICS_PER_MS)

// with the introduction of Gen2, we need a module level Priority var as well
static uint8_t MyPriority;

/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
 Function
     InitTemplateFSM

 Parameters
     uint8_t : the priorty of this service

 Returns
     bool, false if error in initialization, true otherwise

 Description
     Saves away the priority, sets up the initial transition and does any
     other required initialization for this state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 18:55
****************************************************************************/
bool InitGhostHuntFSM(uint8_t Priority)
{
  ES_Event_t ThisEvent;
  MyPriority = Priority;
  clrScrn();
  
  //initialize all inputs and outputs
  ANSELA = 0; 
  ANSELB = 0; 
  
  InitServos();
  InitShootButton();
//  InitIREmitter(); 
  InitPowerUpButton(); 
//  InitIRReciever(); 
  InitMicrophone(); 
  
  uint32_t LastPowerUpState = 0;
    
  // post the initial transition event
  ThisEvent.EventType = ES_INIT;
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
     PostTemplateFSM

 Parameters
     EF_Event_t ThisEvent , the event to post to the queue

 Returns
     boolean False if the Enqueue operation failed, True otherwise

 Description
     Posts an event to this state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
****************************************************************************/
bool PostGhostHuntFSM(ES_Event_t ThisEvent)
{
  return ES_PostToService(MyPriority, ThisEvent);
}

/****************************************************************************
 Function
    RunTemplateFSM

 Parameters
   ES_Event_t : the event to process

 Returns
   ES_Event_t, ES_NO_EVENT if no error ES_ERROR otherwise

 Description
   add your description here
 Notes
   uses nested switch/case to implement the machine.
 Author
   J. Edward Carryer, 01/15/12, 15:23
****************************************************************************/
ES_Event_t RunGhostHuntFSM(ES_Event_t ThisEvent)
{
  ES_Event_t ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
  
  switch (ThisEvent.EventType)
  {
      case ES_INIT: // If current state is initial Psedudo State
    {
//        DB_printf("WELCOME \n");
//        DB_printf("You are waiting for an input \n");
        ThisEvent.EventType = ES_WELCOME_DISPLAY;
        DB_printf("Timer started \n");
        ES_Timer_InitTimer(SERVICE1_TIMER, FIVE_SEC);
        DB_printf("Servo Timer Started \n");
        ES_Timer_InitTimer(SERVICE0_TIMER, SIXTY_SEC);
        PostDisplayService(ThisEvent);
  
    }
    break;
    
    case ES_POWER_UP:        // If current state is state one
    {
        DB_printf("You hit the Power up Button \n");
        ThisEvent.EventType = ES_CHECK_FOR_POWER_UP;
        PostPointsService(ES_CHECK_FOR_POWER_UP);
        break;

    }
    break;
    
    case ES_SHOT:        // If someone presses the shoot button 
    {
        DB_printf("You took a shot \n");
        ThisEvent.EventType = ES_SHOT_FIRED;
        PostShotFiredService(ThisEvent); 
        break;
    }
    break;
    
    case ES_SOUND:        // If current state is state one
    {
        // DB_printf("The current voltage is  %d \n", ThisEvent.EventParam);
        ThisEvent.EventType = ES_GHOST_JERK;
        PostMoveServosService(ThisEvent);
        PostPointsService(ThisEvent);
        PostSoundService(ThisEvent); 
        break;
    }
    break;
    
    case ES_TIMEOUT:        // If current state is state one
    {
            // ES_Timer_InitTimer(SERVICE0_TIMER, SIXTY_SEC);
        if (ThisEvent.EventParam == SERVICE0_TIMER){
                ThisEvent.EventType = ES_GAME_OVER;
                PostDisplayService(ThisEvent); 
                PostPointsService(ThisEvent);
                // DB_printf("GameOver \n");}
        } else if (ThisEvent.EventParam == SERVICE1_TIMER){
            ES_Timer_InitTimer(SERVICE1_TIMER, FIVE_SEC);
            ThisEvent.EventType = ES_GHOST_TIMER;
            PostMoveServosService(ThisEvent);
            DB_printf("5 second timer \n");
        }
        break;
    }
    break;
    
    // repeat state pattern as required for other states
    default:
    {}
     break;
  }            
//  CurrentState = NextState;
  // end switch on Current State
  return ReturnEvent;
}

/****************************************************************************
 Function
     QueryTemplateSM

 Parameters
     None

 Returns
     TemplateState_t The current state of the Template state machine

 Description
     returns the current state of the Template state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:21
****************************************************************************/
GhostHuntFSM_t QueryGhostHuntFSM(void)
{
  return CurrentState;
}

/***************************************************************************
 private functions
 ***************************************************************************/

void InitPowerUpButton(void) 
{
  TRISAbits.TRISA3 = 1;   // make RA3 a digital input (power up button)
  uint32_t LastPowerUpState = PORTAbits.RA3;
}
  
  void InitShootButton(void)
{
  // set up shooting button 
  TRISBbits.TRISB2 = 1; // set as input 
  uint32_t ShootButtonLastState = PORTBbits.RB2; 
  DB_printf("Shoot button initialized \n");
}
//
//void InitIRReciever(void)
//{
//    //set up IR Reciever 
//    TRISAbits.TRISA4 = 1;   // make RA4 a digital input (IR receiver)
//    uint32_t LastIRReceived = PORTAbits.RA4;
//    DB_printf("IR receiver initialized \n");
//  
//}

//void InitIREmitter(void)
//{
//      // set up IR Emitter
//
//  TRISAbits.TRISA2 = 0; // set as input 
//  
//  uint32_t LastIREmitted = PORTAbits.RA2;
////  DB_printf("IR Emitter initialized \n");
//  
//}

void InitMicrophone(void)
{
    
 TRISBbits.TRISB13 = 1;   // make RB13 an input (microphone)
 ANSELBbits.ANSB13 = 1;   // make RB13 an analog input
 ADC_ConfigAutoScan(BIT11HI);

  DB_printf("Mic initialized \n");
 
}
  
void InitServos(void)
{
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1;
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 1; 
    
    PWMSetup_BasicConfig(2);
    PWMSetup_MapChannelToOutputPin(1, PWM_RPB3);
    PWMSetup_MapChannelToOutputPin(2, PWM_RPB8);
    PWMSetup_AssignChannelToTimer(1, _Timer2_);
    PWMSetup_AssignChannelToTimer(2, _Timer2_);
    PWMSetup_SetFreqOnTimer(50, _Timer2_);  // 50 Hz servo frequency
    PWMOperate_SetDutyOnChannel(50, 1);
    PWMOperate_SetDutyOnChannel(50, 2);
    DB_printf("Servos initialized \n");
}



