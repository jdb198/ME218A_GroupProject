/****************************************************************************
 Module
   EventCheckers.c

 Revision
   1.0.1

 Description
   This is the sample for writing event checkers along with the event
   checkers used in the basic framework test harness.

 Notes
   Note the use of static variables in sample event checker to detect
   ONLY transitions.

 History
 When           Who     What/Why
 -------------- ---     --------
 08/06/13 13:36 jec     initial version
****************************************************************************/

// this will pull in the symbolic definitions for events, which we will want
// to post in response to detecting events
#include "ES_Configure.h"
// This gets us the prototype for ES_PostAll
#include "ES_Framework.h"
// this will get us the structure definition for events, which we will need
// in order to post events in response to detecting events
#include "ES_Events.h"
// if you want to use distribution lists then you need those function
// definitions too.
#include "ES_PostList.h"
// This include will pull in all of the headers from the service modules
// providing the prototypes for all of the post functions
#include "ES_ServiceHeaders.h"
// this test harness for the framework references the serial routines that
// are defined in ES_Port.c
#include "ES_Port.h"
// include our own prototypes to insure consistency between header &
// actual functionsdefinition
#include "EventCheckers.h"

//#define long32_word0(0x000003FF & x)

/* Constants */
uint32_t ConstSound = 511; //512;

// This is the event checking function sample. It is not intended to be
// included in the module. It is only here as a sample to guide you in writing
// your own event checkers
#if 0
/****************************************************************************
 Function
   Check4Lock
 Parameters
   None
 Returns
   bool: true if a new event was detected
 Description
   Sample event checker grabbed from the simple lock state machine example
 Notes
   will not compile, sample only
 Author
   J. Edward Carryer, 08/06/13, 13:48
****************************************************************************/
bool Check4Lock(void)
{
  static uint8_t  LastPinState = 0;
  uint8_t         CurrentPinState;
  bool            ReturnVal = false;

  CurrentPinState = LOCK_PIN;
  // check for pin high AND different from last time
  // do the check for difference first so that you don't bother with a test
  // of a port/variable that is not going to matter, since it hasn't changed
  if ((CurrentPinState != LastPinState) &&
      (CurrentPinState == LOCK_PIN_HI)) // event detected, so post detected event
  {
    ES_Event ThisEvent;
    ThisEvent.EventType   = ES_LOCK;
    ThisEvent.EventParam  = 1;
    // this could be any of the service post functions, ES_PostListx or
    // ES_PostAll functions
    ES_PostAll(ThisEvent);
    ReturnVal = true;
  }
  LastPinState = CurrentPinState; // update the state for next time

  return ReturnVal;
}

#endif

/****************************************************************************
 Function
   Check4Keystroke
 Parameters
   None
 Returns
   bool: true if a new key was detected & posted
 Description
   checks to see if a new key from the keyboard is detected and, if so,
   retrieves the key and posts an ES_NewKey event to TestHarnessService0
 Notes
   The functions that actually check the serial hardware for characters
   and retrieve them are assumed to be in ES_Port.c
   Since we always retrieve the keystroke when we detect it, thus clearing the
   hardware flag that indicates that a new key is ready this event checker
   will only generate events on the arrival of new characters, even though we
   do not internally keep track of the last keystroke that we retrieved.
 Author
   J. Edward Carryer, 08/06/13, 13:48
****************************************************************************/
bool Check4Keystroke(void)
{
  if (IsNewKeyReady())   // new key waiting?
  {
    ES_Event_t ThisEvent;
    ThisEvent.EventType   = ES_NEW_KEY;
    ThisEvent.EventParam  = GetNewKey();
    ES_PostAll(ThisEvent);
    return true;
  }
  return false;
}

bool Check4PowerUp(void) 
{
    bool ReturnVal = false; 
    ES_Event_t ThisEvent;
    uint32_t CurrentPowerUpState = PORTAbits.RA3;
    uint32_t LastPowerUpState = 0; 
    uint16_t InitTime = ES_Timer_GetTime();
    
    if (CurrentPowerUpState == 0) {
        for (int i=0; i < 100; i++){} // delay bounce
        if (PORTAbits.RA3 == 1){
                ThisEvent.EventType = ES_POWER_UP;
                ThisEvent.EventParam = PORTAbits.RA3;
                PostGhostHuntFSM(ThisEvent); 
                ReturnVal = true;
        }
    }
    CurrentPowerUpState = LastPowerUpState; 
    return ReturnVal;
}

bool Check4Shot(void)
{
    bool ReturnVal = false; 
    ES_Event_t ThisEvent; 
    uint32_t CurrentShotState = PORTBbits.RB2;
    uint32_t LastShotState = 0; 
    if(CurrentShotState == 0){
        for (int i=0; i < 100; i++){} // delay bounce
        if (PORTBbits.RB2 == 1){
            ThisEvent.EventType = ES_SHOT;
            ThisEvent.EventParam = PORTBbits.RB2;
            PostGhostHuntFSM(ThisEvent);
            ReturnVal = true;
        }
    }
    CurrentShotState = LastShotState; 
    return ReturnVal;
}

bool Check4Sound(void)
{
    bool ReturnVal = false; 
    ES_Event_t ThisEvent; 
    
//    ADC_ConfigAutoScan(BIT13HI);
    uint32_t ConversionResults[1];
    ADC_MultiRead(ConversionResults);

   
    uint32_t CurrentSound = ConversionResults[0];
    
    if((CurrentSound) > 600){
        float SendVal = CurrentSound/100;
        SendVal = (int)SendVal;

        ThisEvent.EventType = ES_SOUND; 
        ThisEvent.EventParam = SendVal; 
        PostGhostHuntFSM(ThisEvent);
        ReturnVal = true;
        for (int i=0; i < 100000; i++){} // delay bounce
    }
    return ReturnVal; 
}
