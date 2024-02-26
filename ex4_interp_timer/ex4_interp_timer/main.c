/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include <string.h>

static uint8_t start_message[13] = "Start Project";
volatile static uint32_t data_arrived = 0;

static void tx_cb_EDBG_COM(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	gpio_toggle_pin_level(LED0);
}

static void rx_cb_EDBG_COM(const struct usart_async_descriptor *const io_descr)
{
	/* Receive completed */
	data_arrived = 1;
}

static void err_cb_EDBG_COM(const struct usart_async_descriptor *const io_descr)
{
	/* error handle */
	io_write(&EDBG_COM.io, start_message, 12);
}


void put_char( char c )
{
	while (io_write(&EDBG_COM.io, &c, 1) != 1) {
	}
	while( 1 )
	if( usart_async_get_status(&EDBG_COM.io, NULL) == ERR_NONE)
	break ;
}


void print( char *msg)
{
	for(int i = 0 ; i < strlen(msg); i++ )
	put_char( msg[i] );
}

uint8_t get_ch( void )
{
	uint8_t recv_char;
	while( data_arrived == 0 )
	return 0;
	while (io_read(&EDBG_COM.io, &recv_char, 1) == 1)
	break ;
	data_arrived = 0 ;
	
	return recv_char ;
}

uint8_t get_char( void )
{
	uint8_t recv_char;
	while( data_arrived == 0 )
	;
	while (io_read(&EDBG_COM.io, &recv_char, 1) == 1)
	break ;
	put_char( recv_char );
	data_arrived = 0 ;
	
	return recv_char ;
}

void get_str( char *ss )
{
	uint8_t c ;
	int i = 0 ;

	ss[0] = 0 ;
	while( 1 ){
		c = get_char();
		if( c == '\n'){
			ss[i] = 0 ;
			return ;
			}else if((c == '\b') && (i != 0)){
			i--;
			print(" \b");
			}else if( c != '\b') {
			ss[i++] = c ;
		}
	}
}

void put_str_nonblock( char *msg )
{
	io_write(&EDBG_COM.io, msg, strlen( msg ));
}


struct timer_descriptor       TIMER_0;
volatile static struct timer_task    TIMER_0_task;

static void TIMER_0_task_cb(const struct timer_task *const timer_task){
	io_write(&EDBG_COM.io, "\r\nInterruption!\r\n", 21);
}

int main(void)
{
	uint8_t recv_char;
	
	atmel_start_init();

	usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, tx_cb_EDBG_COM);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rx_cb_EDBG_COM);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb_EDBG_COM);
	usart_async_enable(&EDBG_COM);

	io_write(&EDBG_COM.io, start_message, 13);
	print("\r\n");

	TIMER_0_task.interval = 100;
	TIMER_0_task.cb = TIMER_0_task_cb;
	TIMER_0_task.mode = TIMER_TASK_REPEAT;
	timer_add_task(&TIMER_0, &TIMER_0_task);
	timer_start(&TIMER_0);

	while (1) {
		print("within loop...\r\n");
	}
}
