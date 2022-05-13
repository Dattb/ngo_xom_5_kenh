/*********************************************************************
    Project:MG82F6D17-DEMO
    Author:LZD
			MG82F6D17 SSOP20_V10 EV Board (TH194A)
			CpuCLK=12MHz, SysCLK=12MHz
	Description:
			UART0 TX&RX interrupt
	Note:

    Creat time::
    Modify::
    
*********************************************************************/


#include <Intrins.h>
#include <Absacc.h>
#include <Stdio.h>  // for printf
#include ".\include\REG_MG82F6D17.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"
#include ".\include\API_Uart_BRGRL_MG82F6D17.H"


/*************************************************
Set SysClk (MAX.50MHz) (MAX.50MHz)
Selection: 
	11059200,12000000,
	22118400,24000000,
	29491200,32000000,
	44236800,48000000
*************************************************/
#define MCU_SYSCLK		12000000
/*************************************************/
/*************************************************
Set CpuClk (MAX.36MHz)
	1) CpuCLK=SysCLK
	2) CpuClk=SysClk/2
*************************************************/
#define MCU_CPUCLK		(MCU_SYSCLK)
#define MODE1_LIGHT_PIN	P10
#define MODE2_LIGHT_PIN	P11
#define MODE3_LIGHT_PIN	P15
#define MODE4_LIGHT_PIN	P16
#define MODE5_LIGHT_PIN	P17

#define MODE1_STATE_TIME_1	18000  //s

#define MODE2_STATE_TIME_1	18000
#define MODE2_STATE_TIME_2	36000
#define MODE2_STATE_TIME_3	43200		


#define MODE3_STATE_TIME_1	21600	



#define MODE4_STATE_TIME_1	18000
#define MODE4_STATE_TIME_2	39600
#define MODE4_STATE_TIME_3	43200		


#define MODE5_STATE_TIME_1	21600
#define MODE5_STATE_TIME_2	39600
#define MODE5_STATE_TIME_3	43200		


#define LIGHT_5H		0
#define LIGHT_12H		1
#define LIGHT_TYPE		LIGHT_5H


typedef struct {
	unsigned long tickTime;
	unsigned char lightOnFlag;
}lightControlTypeDef;





enum{
	lightOn,
	lightOff,
};

void controlAllLight(unsigned char mode);
void InitPort(void);
void initShow();

void lightStartTick();
void light1Control();
void light2Control();
void light3Control();
void light4Control();
void light5Control();

/***********************************************************************************
Function:       unsigned long system_tick()
Description:    this funciton will calculate time from interrupt timer -> 65.536 ms each irq callback
Input:   
Output:     
*************************************************************************************/
unsigned long systemTick();

/*************************************************
Function:     	void DelayXms(u16 xMs)
Description:    dealy��unit:ms
Input:     		u16 xMs -> *1ms  (1~65535)
Output:     
*************************************************/
void DelayXms(u16 xMs);

/*************************************************
Function:     	void DelayXus(u16 xUs)
Description:   	dealy��unit:us
Input:     		u8 Us -> *1us  (1~255)
Output:     
*************************************************/
void DelayXus(u8 xUs);
