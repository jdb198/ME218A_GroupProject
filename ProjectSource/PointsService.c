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
#include "PointsService.h"

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
*/

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;
static int TotalPoints = 0; 
static int Num_Hits = 0;


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
bool InitPointsService(uint8_t Priority)
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
bool PostPointsService(ES_Event_t ThisEvent)
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
ES_Event_t RunPointsService(ES_Event_t ThisEvent)
{
  ES_Event_t ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
  
  if (ThisEvent.EventType == ES_ADD_POINTS){
      
      if (ThisEvent.EventParam == 1){
          TotalPoints = TotalPoints + 4;
          Num_Hits = 0;
      }else {
          TotalPoints = TotalPoints + 2;
          Num_Hits++;
      }
      // DB_printf("Total Points %d \n", TotalPoints);
       
      ThisEvent.EventType = ES_POINT_DISPLAY; 
      ThisEvent.EventParam = TotalPoints; 
      PostDisplayService(ThisEvent); 
      
  } else if (ThisEvent.EventType == ES_GHOST_JERK){
      if(TotalPoints == 0){
          TotalPoints = 0; 
      } else if (TotalPoints - ThisEvent.EventParam < 0){
          TotalPoints = 0; 
      } else {
          TotalPoints = TotalPoints - ThisEvent.EventParam;
      }
      // DB_printf("Total Points %d \n", TotalPoints);
      Num_Hits = 0;
      ThisEvent.EventType = ES_POINT_DISPLAY; 
      ThisEvent.EventParam = TotalPoints; 
      PostDisplayService(ThisEvent); 

  } else if (ThisEvent.EventType == ES_GAME_OVER){
    //post thte number of tokens to the move servos so that that can dictate the number of gears
      ThisEvent.EventParam = TotalPoints; 
      PostMoveServosService(ThisEvent);
      TotalPoints = 0; 
      Num_Hits = 0; 
      ThisEvent.EventType = ES_POINT_DISPLAY; 
      ThisEvent.EventParam = TotalPoints; 
      PostDisplayService(ThisEvent); 


  } else if (ThisEvent.EventType == ES_CHECK_FOR_POWER_UP){
      CheckPowerUpButton(Num_Hits);

      
  } else if (ThisEvent.EventType == ES_SUBTRACT_POINTS){
    if(TotalPoints == 0){
          TotalPoints = 0; 
      } else if (TotalPoints - 3 < 0){
          TotalPoints = 0; 
      } else {
          TotalPoints = TotalPoints - 3;
      }
      // DB_printf("Total Points %d \n", TotalPoints);
      Num_Hits = 0;
      ThisEvent.EventType = ES_POINT_DISPLAY; 
      ThisEvent.EventParam = TotalPoints; 
      PostDisplayService(ThisEvent); 
  }

  return ReturnEvent;
}

/***************************************************************************
 private functions
 ***************************************************************************/
void CheckPowerUpButton(int Number_of_Hits_In_A_Row){
    ES_Event_t ThisEvent;
    
    
    if (Num_Hits > 3){
        DB_printf("You got a power up \n");
        ThisEvent.EventParam = 1; 
        ThisEvent.EventType = ES_ADD_POINTS; 
        PostPointsService(ThisEvent); 
    }
    return;
}



/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

