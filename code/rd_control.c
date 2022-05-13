
#include "rd_control.h"

xdata lightControlTypeDef rdLight1;
xdata lightControlTypeDef rdLight2;
xdata lightControlTypeDef rdLight3;
xdata lightControlTypeDef rdLight4;
xdata lightControlTypeDef rdLight5;
extern unsigned long timeLine;

void initShow(){
	controlAllLight(1);
	DelayXms(1000);
	controlAllLight(0);
	DelayXms(1000);
	controlAllLight(1);
	DelayXms(1000);
	controlAllLight(0);
	DelayXms(1000);
	controlAllLight(1);
	DelayXms(1000);
	controlAllLight(0);
	DelayXms(1000);
	controlAllLight(1);
	rdLight1.lightOnFlag = lightOn;
	rdLight2.lightOnFlag = lightOn;
	rdLight3.lightOnFlag = lightOn;
	rdLight4.lightOnFlag = lightOn;
	rdLight5.lightOnFlag = lightOn;
	lightStartTick();
}

void controlAllLight(unsigned char mode){
	MODE1_LIGHT_PIN = mode;
	MODE2_LIGHT_PIN = mode;
	MODE3_LIGHT_PIN = mode;
	MODE4_LIGHT_PIN = mode;
	MODE5_LIGHT_PIN = mode;
}

void lightStartTick(){
	rdLight1.tickTime = systemTick();
	rdLight2.tickTime = systemTick();
	rdLight3.tickTime = systemTick();
	rdLight4.tickTime = systemTick();
	rdLight5.tickTime = systemTick();
}

void light1Control(){
	if(rdLight1.lightOnFlag == lightOn){
		if(systemTick() - rdLight1.tickTime <= MODE1_STATE_TIME_1){
			MODE1_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight1.tickTime > MODE1_STATE_TIME_1){
			MODE1_LIGHT_PIN = 0;
			rdLight1.lightOnFlag = lightOff;
		}
	}
}

void light2Control(){
	if(rdLight2.lightOnFlag == lightOn){
		if(systemTick() - rdLight2.tickTime <= MODE2_STATE_TIME_1){
			MODE2_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight2.tickTime <= MODE2_STATE_TIME_2){
			MODE2_LIGHT_PIN = 0;
		}
		else if(systemTick() - rdLight2.tickTime <= MODE2_STATE_TIME_3){
			MODE2_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight2.tickTime > MODE2_STATE_TIME_3){
			MODE2_LIGHT_PIN = 0;
			rdLight2.lightOnFlag = lightOff;
		}
	}			
}


void light3Control(){
	if(rdLight3.lightOnFlag == lightOn){
		if(systemTick() - rdLight3.tickTime <= MODE3_STATE_TIME_1){
			MODE3_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight3.tickTime > MODE3_STATE_TIME_1){
			MODE3_LIGHT_PIN = 0;
			rdLight3.lightOnFlag = lightOff;
		}
	}		
}

void light4Control(){
	if(rdLight4.lightOnFlag == lightOn){
		if(systemTick() - rdLight4.tickTime <= MODE4_STATE_TIME_1){
			MODE4_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight4.tickTime <= MODE4_STATE_TIME_2){
			MODE4_LIGHT_PIN = 0;
		}
		else if(systemTick() - rdLight4.tickTime <= MODE4_STATE_TIME_3){
			MODE4_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight4.tickTime > MODE4_STATE_TIME_3){
			MODE4_LIGHT_PIN = 0;
			rdLight4.lightOnFlag = lightOff;
		}
	}			
}

void light5Control(){
	if(rdLight5.lightOnFlag == lightOn){
		if(systemTick() - rdLight5.tickTime <= MODE5_STATE_TIME_1){
			MODE5_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight5.tickTime <= MODE5_STATE_TIME_2){
			MODE5_LIGHT_PIN = 0;
		}
		else if(systemTick() - rdLight5.tickTime <= MODE5_STATE_TIME_3){
			MODE5_LIGHT_PIN = 1;
		}
		else if(systemTick() - rdLight5.tickTime > MODE5_STATE_TIME_3){
			MODE5_LIGHT_PIN = 0;
			rdLight5.lightOnFlag = lightOff;
		}
	}			
}


#define TIME_ONE_TICK	1000		//MS
unsigned long systemTick(){
	return	(unsigned long)((float)(timeLine) / TIME_ONE_TICK * (65.536));
}

/***********************************************************************************
Function:   	void InitPort()
Description:	Initialize IO Port
Input:   
Output:   		
*************************************************************************************/
void InitPort(void)
{
	//	PORT_SetP3QuasiBi(BIT4);		// set P30,P31,P33,P34,P35 as Quasi-Bidirectional
	PORT_SetP1PushPull(BIT0|BIT1|BIT5|BIT6|BIT7);		
}


void DelayXus(u8 xUs)
{
	while(xUs!=0)
	{
#if (MCU_CPUCLK>=11059200)
		_nop_();
#endif
#if (MCU_CPUCLK>=14745600)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=16000000)
		_nop_();
#endif

#if (MCU_CPUCLK>=22118400)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=24000000)
		_nop_();
		_nop_();
#endif		
#if (MCU_CPUCLK>=29491200)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=32000000)
		_nop_();
		_nop_();
#endif

		xUs--;
	}
}


void DelayXms(u16 xMs)
{
	while(xMs!=0)
	{
		CLRWDT();
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		xMs--;
		
	}
}