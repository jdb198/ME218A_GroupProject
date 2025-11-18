/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef DisplayService_H
#define DisplayService_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitDisplayService(uint8_t Priority);
bool PostDisplayService(ES_Event_t ThisEvent);
ES_Event_t RunDisplayService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

