/*
 * buzzer.cpp
 *
 *  Created on: Aug 24, 2020
 *      Author: smroz
 */

#include "buzzer.hpp"

int32_t buzzerTime;

void beep(int times)
{
	int i;
	buzzerTime = 140*times;//this function takes about 140ms to finish when int times = 1, we have this line here is because of the function void shortBeep(int duration, int freq) below.
	                       //we set buzzerTime to 140 multiply how many times to beep to prevent the buzzer being turned off at the first millisecond.
	setBuzzerFrequency(2000);  //set buzzer beep at 4000Hz
	for(i=0; i<times; i++)
	{
		beep_on;
		HAL_Delay(70);
		beep_off;
		HAL_Delay(70);
	}
}


//this is the kinda of beep that you can use when the mouse is running.
//for example, if you want to mouse to beep for 100ms, we turn on the buzzer and set the buzzerTime as the beeping duration
//we also call buzzerTime--; inside of SysTick_Handler inside of stm32f4xx_it.c . so the buzzerTime will reduce by 1 every milliseoncd
//since SysTick_Handler runs every 1 ms.  We also have to include inside of SysTick_Handler something like
//         if(buzzerTime < 0)
//		       beep_off;
//in order to turn off the buzzer after the designated time elaspes. By this way, the MCU doesn't have to wait and't can't do anything else until the buzzer is off.
//this can be use to tell some information such as when the wall-to-no-wall or post-to-no-post transition is detected when mouse is running.
//this is a smarter way than the funtion void beep(int times). However, you can still use void beep(int times) when the mouse stops before anything else going to happen.
void shortBeep(int duration, int freq)
{
	//beep_on;
	beep_on;
	setBuzzerFrequency(freq);
	buzzerTime = duration;
}



void tone(int duration, int freq)
{
	setBuzzerFrequency(freq);  //set buzzer beep at 4000Hz
	beep_on;
	HAL_Delay(duration);
	beep_off;

}
