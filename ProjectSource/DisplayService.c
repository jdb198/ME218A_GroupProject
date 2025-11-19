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
#include "DisplayService.h"
#include <stdio.h>
#include <string.h>
/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
 */

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;

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
bool InitDisplayService(uint8_t Priority) {
    ES_Event_t ThisEvent;

    MyPriority = Priority;
    /********************************************
     in here you write your initialization code
     *******************************************/
    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == true) {
        return true;
    } else {
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
bool PostDisplayService(ES_Event_t ThisEvent) {
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
ES_Event_t RunDisplayService(ES_Event_t ThisEvent) {
    ES_Event_t ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
//    if (ThisEvent.EventType == ES_INIT){
//          while (false == DM_TakeInitDisplayStep()){}
//
//    }
    if (ThisEvent.EventType == ES_WELCOME_DISPLAY) {
        
        static const char welcome[] = "WELCOME";

        for (int i = 0; i < strlen(welcome); i++) {
          
            ThisEvent.EventType = ES_LED_CHAR;
            ThisEvent.EventParam = welcome[i]; // 
            PostLEDService(ThisEvent);
        }
    }

    else if (ThisEvent.EventType == ES_POINT_DISPLAY) {

        char NumAsStr[] = "000   ";
        snprintf(NumAsStr, sizeof(NumAsStr), "%d   ", ThisEvent.EventParam);
        DM_ClearDisplayBuffer();
        for (int i = 0; i <strlen(NumAsStr); i++){
            ThisEvent.EventType = ES_LED_CHAR;
            ThisEvent.EventParam = NumAsStr[i]; // 
            PostLEDService(ThisEvent);
        }

    } else if (ThisEvent.EventType == ES_GAME_OVER) {
      
        static const char gameover[] = "GAMEOVER";
        DM_ClearDisplayBuffer(); // calling dm display functions from outside the led service coudl be bad
        for (int i = 0; i < strlen(gameover); i++) {
            ThisEvent.EventType = ES_LED_CHAR;
            ThisEvent.EventParam = gameover[i]; // 
            PostLEDService(ThisEvent);
        }

        //return ReturnEvent;
    }
return ReturnEvent;

}

/***************************************************************************
 private functions
 ***************************************************************************/

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

