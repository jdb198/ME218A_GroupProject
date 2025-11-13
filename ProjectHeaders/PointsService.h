/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef ServTemplate_H
#define ServTemplate_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitPointsService(uint8_t Priority);
bool PostPointsService(ES_Event_t ThisEvent);
ES_Event_t RunPointsService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

