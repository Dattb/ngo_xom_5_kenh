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
#define _MAIN_C

#include <Intrins.h>
#include <Absacc.h>

#include <Stdio.h>  // for printf

#include ".\include\REG_MG82F6D17.H"
#include ".\include\Type.h"
#include ".\include\API_Macro_MG82F6D17.H"
#include ".\include\API_Uart_BRGRL_MG82F6D17.H"
#include "rd_control.h"
#define TIMER0_IRQ_TIME		1		//ms


 unsigned long timeLine = 0;


//#define MCU_CPUCLK		(MCU_SYSCLK/2)

#define TIMER_1T_1ms_TH	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_1ms_TL	((65536-(u16)(float)(1000*((float)(MCU_SYSCLK)/(float)(1000000)))) %256)

#define TIMER_12T_1ms_TH	((65536-(u16)(float)(4000*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(4000*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)

// #define LED_G_0		P33
// #define LED_R		P34
// #define LED_G_1		P35


#define SFR_Page_(x)		SFRPI = x;


#define UART0_RX_BUFF_SIZE   32   		 
#define UART0_TX_BUFF_SIZE   32   		 
xdata u8 RcvBuf[UART0_RX_BUFF_SIZE];
u8 Uart0RxIn =0;
u8 Uart0RxOut =0;
xdata u8 TxBuf[UART0_TX_BUFF_SIZE];
u8 Uart0TxIn =0;
u8 Uart0TxOut =0;
bit bUart0TxFlag;

u8 LedTime;


/***********************************************************************************
Function:   	void INT_UART0(void)
Description:	UART0 Interrupt handler
Input:   
Output:     
*************************************************************************************/
void INT_UART0(void) interrupt INT_VECTOR_UART0
{
	_push_(SFRPI);		   

	SFR_Page_(0);		   
	if(TI0)					
	{
	   TI0 = 0;	   
		if(Uart0TxIn==Uart0TxOut)
		{
			bUart0TxFlag=FALSE;
		}
		else
		{
			S0BUF=TxBuf[Uart0TxOut];
			bUart0TxFlag=TRUE;
			Uart0TxOut++;
			if(Uart0TxOut>=UART0_TX_BUFF_SIZE)
			{
				Uart0TxOut=0;
			}
		}
	}
	if(RI0)					
	{
		RI0 = 0;				
		RcvBuf[Uart0RxIn] = S0BUF;
		Uart0RxIn++;
		if(Uart0RxIn >=UART0_RX_BUFF_SIZE)
		{
			Uart0RxIn =0;
		}
		
	}
	_pop_(SFRPI);		   
}

/***********************************************************************************
Function:   	void INT_T0(void)
Description:	T0 Interrupt handler  time is :1 ms
Input:   
Output:     
*************************************************************************************/
void INT_T0(void) interrupt INT_VECTOR_T0
{
	// TH0=TIMER_12T_1ms_TH;
	// TL0=TIMER_12T_1ms_TL;
	TL0 = 0;
	TH0 = 0;
	timeLine++;
	//if(LedTime!=0) LedTime--;
}

/***********************************************************************************
Function:		void Uart0SendByte(u8 tByte)
Description:	Uart0 send byte
Input:			u8 tByte: the data to be send
Output:     
*************************************************************************************/
void Uart0SendByte(u8 tByte)
{
	u8 i;
	
	if(bUart0TxFlag==FALSE)
	{
		Uart0TxOut=0;
		Uart0TxIn=1;
		TxBuf[0]=tByte;
		TI0=1;
	}
	else
	{
		i=Uart0TxIn;
		TxBuf[i]=tByte;
		i++;
		if(i>=UART0_TX_BUFF_SIZE)
		{
			i=0;
		}
		while(i==Uart0TxOut)
		{
		}
		ES0=0;
		Uart0TxIn=i;
		ES0=1;
	}
}



/***********************************************************************************
Function:		void Uart0SendStr(BYTE* PStr)
Description:	Uart0 send string
Input: 			u8* PStr:the string to be send
Output:     
*************************************************************************************/
void Uart0SendStr(u8* PStr)
{
	while(*PStr != 0)
	{
		Uart0SendByte(*PStr);
		PStr ++;
	}
}





/***********************************************************************************
Function:   	void InitInterrupt()
Description:	Initialize Interrupt
Input:   
Output:   		
*************************************************************************************/
void InitInterrupt(void)
{
	INT_EnTIMER0();						// Enable Timer0 interrupt
	INT_EnUART0();						// Enable UART0 interrupt
	INT_EnAll();						// Enable global interrupt
	
}	


/***********************************************************************************
Function:   	void InitUart0_S0BRG()
Description:	Initialize Uart0, The source of baud rate was S0BRG
Input:   
Output:   		
*************************************************************************************/
void InitUart0_S0BRG(void)
{
	UART0_SetAccess_S0CR1();			// Enable access S0CR1 
	UART0_SetMode8bitUARTVar();			// UART0 Mode: 8-bit, Variable B.R.
	UART0_EnReception();				// Enable reception
	UART0_SetBRGFromS0BRG();			// B.R. source: S0BRG
	UART0_SetS0BRGBaudRateX2();			// S0BRG x2
	UART0_SetS0BRGSelSYSCLK();			// S0BRG clock source: SYSCLK

	// Sets B.R. value
	UART0_SetS0BRGValue(S0BRG_BRGRL_9600_2X_12000000_1T);

	UART0_EnS0BRG();					// Enable S0BRG
}


/***********************************************************************************
Function:   	void InitTimer0()
Description:	Initialize Timer0
Input:   
Output:   		
*************************************************************************************/
void InitTimer0(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// TIMER0 Mode: 16-bit
	TM_SetT0Clock_SYSCLKDiv12();			// TIMER0 Clock source: SYSCLK/12
	TM_SetT0Gate_Disable();					// TIMER0 disable gate
	
	TM_SetT0LowByte(TIMER_12T_1ms_TL);		// Set TL0 value
	TM_SetT0HighByte(TIMER_12T_1ms_TH);		// Set TH0 value
	
	TM_EnableT0();							// Enable TIMER0
}

/***********************************************************************************
Function:       void InitSystem(void)
Description:    Initialize MCU
Input:   
Output:     
*************************************************************************************/
unsigned long sendUartTime = 0;
void InitSystem(void)
{
	InitPort();
	InitTimer0();
	InitUart0_S0BRG();
	InitInterrupt();
	Uart0RxIn=0;
	Uart0RxOut=0;
	Uart0TxIn=0;
	Uart0TxOut=0;
	bUart0TxFlag=0;
	initShow();
}




unsigned long timeLineToTest = 0;
unsigned long timeLineToTestOld= 0;
void main()
{
	InitSystem();
	Uart0SendStr("Start!\n");
	while(1)
	{
		light1Control();
		light2Control();
		light3Control();
		light4Control();
		light5Control();
	}
}


