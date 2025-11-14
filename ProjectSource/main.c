/****************************************************************************
 Module
     main.c
 Description
     starter main() function for Events and Services Framework applications
 Notes

*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ES_Port.h"

#include <xc.h>

#include "PIC32_SPI_HAL.h"
#include <string.h>


void main(void)
{
  ES_Return_t ErrorType = Success;

  _HW_PIC32Init(); // basic PIC hardware init
  // Your hardware initialization function calls go here
  SPISetup_BasicConfig(SPI_SPI1);
  SPISetup_SetLeader(SPI_SPI1, SPI_SMP_MID); 
  SPISetup_MapSSOutput(SPI_SPI1, SPI_RPA0); 
  SPISetup_MapSDOutput(SPI_SPI1, SPI_RPA1);
  SPISetEnhancedBuffer(SPI_SPI1, 1);
  SPISetup_SetBitTime(SPI_SPI1, 10000);
  SPISetup_SetActiveEdge(SPI_SPI1, SPI_CLK_LO); 
  SPISetup_SetClockIdleState(SPI_SPI1, SPI_CLK_HI);// changed from high
  SPISetup_SetXferWidth(SPI_SPI1, SPI_16BIT);
  SPI1STATbits.SPIROV = 0;
  SPISetup_EnableSPI(SPI_SPI1);
  SPI1BUF; //reset the buffer 
    
//  while (false == DM_TakeInitDisplayStep()){}


  // now initialize the Events and Services Framework and start it running
  ErrorType = ES_Initialize(ES_Timer_RATE_1mS);
  if (ErrorType == Success)
  {
    ErrorType = ES_Run();
  }
  //if we got to here, there was an error
  switch (ErrorType)
  {
    case FailedPost:
    {
      ErrorType = 0; // a hack to get a readable assert message
      assert( FailedPost == ErrorType );
      //DB_printf("Failed on attempt to Post\n");
    }
    break;
    case FailedPointer:
    {
      ErrorType = 0; // a hack to get a readable assert message
      assert( FailedPointer  == ErrorType );
//      DB_printf("Failed on NULL pointer\n");
    }
    break;
    case FailedInit:
    {
      ErrorType = 0; // a hack to get a readable assert message
      assert( FailedInit  == ErrorType );
//      DB_printf("Failed Initialization\n");
    }
    break;
    default:
    {
      ErrorType = 0; // a hack to get a readable assert message
      assert( FailedOther  == ErrorType );
//      DB_printf("Other Failure\n");
    }
    break;
  }
  for ( ; ;)
  {
    ;
  }
}

