int main(void)
{
	UART_INIT();
	PIN_INIT();
	TIMER1_INIT();
	
	sei();

	while (1){};
}