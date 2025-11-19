/****************************************************************************

  Header file for template service
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef LEDService_H
#define LEDService_H

#include "ES_Types.h"

// Public Function Prototype
typedef enum
{
    LED_Idle, LED_Update
}LEDService_t;


bool InitLEDService(uint8_t Priority);
bool PostLEDService(ES_Event_t ThisEvent);
ES_Event_t RunLEDService(ES_Event_t ThisEvent);

#endif /* ServTemplate_H */

