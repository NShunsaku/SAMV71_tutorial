/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void delay_example(void)
{
	delay_ms(5000);
}

/**
 * Example of using EDBG_COM to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_EDBG_COM[12] = "Hello World!";

static void tx_cb_EDBG_COM(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void EDBG_COM_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, tx_cb_EDBG_COM);
	/*usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&EDBG_COM, &io);
	usart_async_enable(&EDBG_COM);

	io_write(io, example_EDBG_COM, 12);
}
