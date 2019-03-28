/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    examen1.c
 * @brief   Application entry point.
 */

#include "MK64F12.h"
#include "rgb.h"
#include "Delay.h"
#include "PIT.h"


void secuencia_arranque();
#define SYSTEM_CLOCK (21000000U)
#define PIT_DELAY (1.0F)
#include "switches_k64.h"


int main(void) {

	init_sw2(NO_PRIORITY_TH,NO_PRIORITY,NO_INT_EDGE_DEFINED);
	init_sw3(NO_PRIORITY_TH,NO_PRIORITY,NO_INT_EDGE_DEFINED);

	init_rgb();
	secuencia_arranque();

	uint8_t pit_inter_status = FALSE;
	init_rgb();
	set_PIT_timer_with_interrupt(PIT_1,SYSTEM_CLOCK , PIT_DELAY, PIT_CH1_IRQ, PRIORITY_11);
	uint8_t two_seconds=0;
	uint8_t inicia_periodo=0;/* tras sw3 es presionado*/
	uint8_t veces_sw3_presionado=0;
	uint8_t color_led[0]={OFF,BLUE,GREEN,RED,YELLOW,PURPLE};

	while(1) {

		/* interrupcion del pit a cada segundo*/
		pit_inter_status= PIT_get_interrupt_flag_status(PIT_1);
		if(TRUE == pit_inter_status)
		{
			if( TRUE== inicia_periodo)
			{
				two_seconds++;
			}
			else
			{
				two_seconds=0;
			}


			PIT_clear_interrupt_flag(PIT_1);
		}
		if(2< two_seconds)
		{
			/*prender el led de los segundos presionados*/
			two_seconds=0;
			inicia_periodo=0;// AQUI FINALIZA PERIODO
			/*AQUI SE ENCIENDE EL LED SELECCIONADO POR EL NUMERO DE VECES QUE FUE PRESIONADO SW3*/
			rgb_color(color_led[veces_sw3_presionado],ON);
		}
		sw2_pressed();//reads sw2 state
		if(sw2_one_shot())
		{
			rgb_color(WHITE,OFF);
		}
		sw3_pressed();//reads sw2 state
		if(sw3_one_shot())
		{
			if(0== inicia_periodo)
			{
				inicia_periodo=1;
				veces_sw3_presionado=0;
			}
			else
			{
				veces_sw3_presionado++;
				if(5	< veces_sw3_presionado)
					veces_sw3_presionado=0;
			}
		}


	}
	return 0 ;
}


void secuencia_arranque()
{

	rgb_color(WHITE,TOOGLE);
	delay(25000);
	rgb_color(WHITE,TOOGLE);
	delay(25000);
	rgb_color(WHITE,TOOGLE);
	delay(25000);
	rgb_color(WHITE,TOOGLE);
	delay(25000);

}


