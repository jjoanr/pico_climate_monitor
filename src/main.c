#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "dht11.h"

#define DHT11_PIN 22

int main(void) {
	stdio_init_all();
	init_gpio(DHT11_PIN);
	// Main loop
	while (1) {
		send_start_signal();

		if (check_response()) {
			uint8_t data[5] = {0};
			read_data(data);

			// Verify checksum
			if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
				printf("Humidity: %d.%d%%\n", data[0], data[1]);
				printf("Temperature: %d.%d°C\n", data[2], data[3]);
			} else {
				printf("Checksum error\n");
			}
		} else {
			printf("No response from DHT11\n");
		}

		sleep_ms(5000); // Read every 2 seconds
	}
	return 0;
}
