/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef ShotFiredService_H
#define ShotFiredService_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitShotFiredService(uint8_t Priority);
bool PostShotFiredService(ES_Event_t ThisEvent);
ES_Event_t RunShotFiredService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

