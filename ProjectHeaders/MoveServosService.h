/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef MoveServosService_H
#define MoveServosService_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitMoveServosService(uint8_t Priority);
bool PostMoveServosService(ES_Event_t ThisEvent);
ES_Event_t RunMoveServosService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

