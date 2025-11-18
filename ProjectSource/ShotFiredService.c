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
#include "ShotFiredService.h"

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
*/

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;
static int ShotNum = 0; 
static int ShotsInRow = 0; 
static int PrevShot = 0; 
static int PowerUpPressed = 0;

static int MissGhostNum = 0;
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
bool InitShotFiredService(uint8_t Priority)
{
  ES_Event_t ThisEvent;

  MyPriority = Priority;
  /********************************************
   in here you write your initialization code
   *******************************************/
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
bool PostShotFiredService(ES_Event_t ThisEvent)
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
ES_Event_t RunShotFiredService(ES_Event_t ThisEvent)
{
  ES_Event_t ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
  
  if (ThisEvent.EventType == ES_SHOT_FIRED){ 
      CheckForShotReceived();
      
      //reset the number of shots in a row
  }
  /********************************************
   in here you write your service code
   *******************************************/
  return ReturnEvent;
}

/***************************************************************************
 private functions
 ***************************************************************************/
void CheckForShotReceived(void)
{
    ES_Event_t ThisEvent;
    //this is the dummy data for testing the services all together//
    int DummyShots[25] = {1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 
            0, 1, 1, 1, 1, 1, 1, 1};
    
    if (ShotNum <24){
        if(DummyShots[ShotNum] == 1){
            DB_printf("You hit the ghost! \n");
            ShotNum++;
            ThisEvent.EventParam = 0;
            ThisEvent.EventType = ES_ADD_POINTS;
            PostPointsService(ThisEvent); 
        } else{
            ShotNum++; 
            DB_printf("you missed the ghost \n");
            ThisEvent.EventType = ES_SUBTRACT_POINTS;
            PostPointsService(ThisEvent);
        }
    } else {
        ShotNum = 0; 
    }
    return;
    
    /* What you should actually do, call the output from the IR reciever, 
     threshold to see if there is a hit from any of the 3, if so return true*/
}




/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

