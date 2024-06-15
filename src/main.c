#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#define DHT11_PIN 22

// Function definitions
void setup_gpio();
void send_start_signal();
bool check_response();
void read_data(uint8_t data[5]);

int main(void) {
	stdio_init_all();
	setup_gpio();
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

void setup_gpio() {
	gpio_init(DHT11_PIN);
	gpio_set_dir(DHT11_PIN, GPIO_OUT);
}

bool check_response() {
	absolute_time_t timeout_time;
	// Gets timestamp of 100us from current time
	timeout_time = make_timeout_time_us(100);
	while(gpio_get(DHT11_PIN)) {
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			printf("1st check: false)");
			return false;
		}
	}
	timeout_time = make_timeout_time_us(100);
	while(!gpio_get(DHT11_PIN)) {
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			return false;
			printf("2st check: false)");
		}
	}
	timeout_time = make_timeout_time_us(100);
	while(gpio_get(DHT11_PIN)) {
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			return false;
			printf("3rd check: false)");
		}
	}
	return true;
}

void send_start_signal() {
	gpio_set_dir(DHT11_PIN, GPIO_OUT);
	gpio_put(DHT11_PIN, 0);
	sleep_ms(18);

	gpio_put(DHT11_PIN, 1);
	sleep_us(20);

	gpio_set_dir(DHT11_PIN, GPIO_IN);
}

void read_data(uint8_t data[5]) {
	for (int i = 0; i < 5; i++) {
		data[i] = 0;
		for (int j = 0; j < 8; j++) {
			// Wait for the start of the bit
			while (!gpio_get(DHT11_PIN));

			// Measure the length of the high signal
			absolute_time_t start_time = get_absolute_time();
			while (gpio_get(DHT11_PIN));
			int64_t duration = absolute_time_diff_us(start_time, get_absolute_time());

			// If high signal was long, it's a 1, otherwise it's a 0
			if (duration > 40) {
				data[i] |= (1 << (7 - j));
			}
		}
	}
}
