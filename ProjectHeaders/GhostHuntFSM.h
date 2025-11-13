/****************************************************************************

  Header file for template Flat Sate Machine
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef GhostHuntFSM_H
#define GhostHuntFSM_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */
#include "ES_Types.h"     /* gets bool type for returns */

// typedefs for the states
// State definitions for use with the query function
typedef enum
{
  WaitForInput, UnlockWaiting, _1UnlockPress,
  _2UnlockPresses, Locked, InitGame
}TemplateState_t;

// Public Function Prototypes

bool InitGhostHuntFSM(uint8_t Priority);
bool PostGhostHuntFSM(ES_Event_t ThisEvent);
ES_Event_t RunGhostHuntFSM(ES_Event_t ThisEvent);
TemplateState_t QueryGhostHuntFSM(void);

#endif /* FSMTemplate_H */

