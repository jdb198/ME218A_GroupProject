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
  
  CurrentState = InitGame;
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
  NextState = CurrentState;

  switch (CurrentState)
  {
      case InitGame: // If current state is initial Psedudo State
    {
      if (ThisEvent.EventType == ES_INIT)    // only respond to ES_Init
      {
        // Begining of the game, Print welcome and begin overall game timer
          DB_printf("WELCOME \n"); 
//          if(true == ES_Timer_InitTimer(SERVICE0_TIMER, SIXTY_SEC)){
//              // this represents the game being over
//              DB_printf("GAMEOVER /n");
//              // post to reset game
//          }
//          if (true == ES_Timer_InitTimer(SERVICE0_TIMER, FIVE_SEC)) {
//              // this is the timer for the ghost to mose every five seconds 
//              DB_printf("5 second timer reached - GHOST MOVE");
//              //post to ghost movement service
//              
//          }
        
      }
      NextState = WaitForInput;
      DB_printf("You are waiting for an input \n");
    }
    break;

    case WaitForInput:        // If current state is state one
    {
        ThisEvent.EventType = ES_NEW_KEY; 
        if (ThisEvent.EventParam == 's'){
            // example of someone taking a shot. 
            DB_printf("You took a shot \n"); 
            ThisEvent.EventType = ES_SHOT_FIRED;
            PostShotFiredService(ThisEvent); 
            //post to shot service to determine if missed or made
        } else if (ThisEvent.EventParam == 'a') {
            //This represents an audible sound being made 
            DB_printf("You made a sound and scared the ghost \n");
            // post to points and subtract 5
        } else if (ThisEvent.EventParam == 'p') {
            //this represents the power up button being shot
            DB_printf("You hit the power up button \n");
            //check for enough points hit in a row. 
        }
    }
    break;
    
    
    // repeat state pattern as required for other states
    default:
      ;
  }            
  CurrentState = NextState;
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

