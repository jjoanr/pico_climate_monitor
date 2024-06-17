#include "dht11.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

unsigned int dht11_gpio;

void init_dht11(uint gpio) {
	dht11_gpio = gpio;
	gpio_init(dht11_gpio);
	gpio_set_dir(dht11_gpio, GPIO_OUT);
}

void send_start_signal() {
	// Sets gpio as output, puts line low and waits
	gpio_set_dir(dht11_gpio, GPIO_OUT);
	gpio_put(dht11_gpio, 0);
	sleep_ms(18);
	// Puts line high and waits
	gpio_put(dht11_gpio, 1);
	sleep_us(20);
	// Sets gpio as input, waiting to recieve data
	gpio_set_dir(dht11_gpio, GPIO_IN);
}

bool check_response() {
	absolute_time_t timeout_time;
	// Gets timestamp of 100us from current time
	timeout_time = make_timeout_time_us(100);
	// Waits to detect when the line goes low
	while(gpio_get(dht11_gpio)) {
		// Checks if enough time elapsed, if it didn't, bad response from sensor
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			printf("1st check: false\n");
			return false;
		}
	}
	// 2nd check. Waits for line to go high
	timeout_time = make_timeout_time_us(100);
	while(!gpio_get(dht11_gpio)) {
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			return false;
			printf("2nd check: false\n");
		}
	}
	timeout_time = make_timeout_time_us(100);
	// Final check, line goes low
	while(gpio_get(dht11_gpio)) {
		if(absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
			return false;
			printf("3rd check: false\n");
		}
	}
	return true;
}

void read_data(uint8_t data[5]) {
	// Outer for. Stores the 40 bits received in 5 bytes
	for (int i = 0; i < 5; i++) {
		data[i] = 0;
		for (int j = 0; j < 8; j++) {
			// Wait for the start of the bit
			while (!gpio_get(dht11_gpio));

			// Measure the length of the high signal
			absolute_time_t start_time = get_absolute_time();
			while (gpio_get(dht11_gpio));
			int64_t duration = absolute_time_diff_us(start_time, get_absolute_time());

			// If high signal was long enough, it's a 1, otherwise it's a 0
			if (duration > 40) {
				data[i] |= (1 << (7 - j));
			}
		}
	}
}

bool verify_checksum(uint8_t data[5]) {
	if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
		return true;
	}
	return false;
}
