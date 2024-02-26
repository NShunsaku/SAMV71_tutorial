## Ex3: Output Pulse and Read Pulse Level

This project is an example that outputs a high or low-level signal from one pin of the Extension Header in response to commands entered from the terminal via serial communication and reads and outputs the signal level from another pin. It is based on the SAM V71 example project 'EDBG UART,' but similar to Example 2, it requires setting up serial communication using TeraTerm for command transmission.

### Main Source Code

```c
// main.c
#define PIN_PULSE_IN GPIO(GPIO_PORTC, 19)
#define PIN_PULSE_OUT GPIO(GPIO_PORTA, 6)

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

	gpio_set_pin_direction(PIN_PULSE_OUT, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PIN_PULSE_IN, GPIO_DIRECTION_IN);

	gpio_set_pin_level(PIN_PULSE_OUT, false);
	gpio_set_pin_level(PIN_PULSE_IN, false);
	bool true_or_high = false;

	while (1) {
		io_write(&EDBG_COM.io, "Input: ", 7);
		uint8_t flag_input = get_char();
		if(flag_input == '1'){
			print("\r\n");
			io_write(&EDBG_COM.io, "ON!", 3);
			gpio_set_pin_level(PIN_PULSE_OUT, true);
		}
		else if(flag_input == '2'){
			print("\r\n");
			io_write(&EDBG_COM.io, "OFF!", 4);
			gpio_set_pin_level(PIN_PULSE_OUT, false); // switch output pulse level in response to the terminal command
		}
		print("\r\n");
		true_or_high = gpio_get_pin_level(PIN_PULSE_IN); // read pulse level
		if(true_or_high) io_write(&EDBG_COM.io, "HIGH", 4);
		else io_write(&EDBG_COM.io, "LOW", 3);
		print("\r\n");
	}
}
```

### Results Output
