/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef SoundService_H
#define SoundService_H

#include "ES_Types.h"

// Public Function Prototypes

bool InitSoundService(uint8_t Priority);
bool PostSoundService(ES_Event_t ThisEvent);
ES_Event_t RunSoundService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

