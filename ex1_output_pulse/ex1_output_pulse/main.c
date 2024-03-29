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
 * Ex: Simple example for output pulse from GPIO Port A6
 */
#include "atmel_start.h"
#include "atmel_start_pins.h"

#define PIN_PULSE GPIO(GPIO_PORTA, 6)
#define PIN_PULSE2 GPIO(GPIO_PORTC, 19)

int main(void)
{
	atmel_start_init();
	gpio_set_pin_direction(PIN_PULSE, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PIN_PULSE2, GPIO_DIRECTION_OUT);

	while (1) {
		gpio_set_pin_level(PIN_PULSE, true);
		delay_ms(1);
		gpio_set_pin_level(PIN_PULSE2, true);
		delay_ms(1);
		gpio_set_pin_level(PIN_PULSE, false);
		delay_ms(1);
		gpio_set_pin_level(PIN_PULSE2, false);
		delay_ms(1);
	}
}
