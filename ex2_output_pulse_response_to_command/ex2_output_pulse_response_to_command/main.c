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

/*
 * Ex: Simple example for output pulse from GPIO Port A6 response to command from a terminal
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include <string.h>
#include "driver_init.h"

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
	io_write(&EDBG_COM.io, start_message, 13);
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


#define PIN_PULSE GPIO(GPIO_PORTA, 6)

int main(void)
{
	uint8_t recv_char;

	atmel_start_init();

	usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, tx_cb_EDBG_COM);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rx_cb_EDBG_COM);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb_EDBG_COM);
	usart_async_enable(&EDBG_COM);

	io_write(&EDBG_COM.io, start_message, 13);
	gpio_set_pin_direction(PIN_PULSE, GPIO_DIRECTION_OUT);

	while (1) {
		io_write(&EDBG_COM.io, "Input: ", 7);
		uint8_t flag_input = get_char();
		if(flag_input == '1'){
			print("\r\n");
			io_write(&EDBG_COM.io, "ON!", 3);
			gpio_set_pin_level(PIN_PULSE, true);
			delay_ms(10);
			gpio_set_pin_level(PIN_PULSE, false);
		}
		print("\r\n");		
	}
}