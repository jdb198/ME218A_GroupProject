/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef PowerUpService_H
#define PowerUpService_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitPowerUpService(uint8_t Priority);
bool PostPowerUpService(ES_Event_t ThisEvent);
ES_Event_t RunPowerUpService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

