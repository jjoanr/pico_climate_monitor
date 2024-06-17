#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "dht11.h"
#include "display.h"
#include "ssd1306.h"

#define DHT11_PIN        22
#define DISPLAY_SDA_PIN  2
#define DISPLAY_SCL_PIN  3
#define I2C_PORT	 i2c1
#define DISPLAY_ADDR	 0x3c

// Display config structure
ssd1306_t disp;

// Function prototypes
void setup_gpios(void);

int main(void) {
	stdio_init_all();
	setup_gpios();
	// Initialize ssd1306 display
	disp.external_vcc = false;
	if(!ssd1306_init(&disp, 128, 64, 0x3c, i2c1)) {
		perror("ssd1306_init");
		return -1;
	}
	// Initialize dht11 sensor
	init_dht11(DHT11_PIN);
	// Main loop
	while (1) {
		send_start_signal();

		if (check_response()) {
			uint8_t data[5] = {0};
			read_data(data);

			if (verify_checksum(data)) {
				display_data(data, &disp);
			} else {
				printf("Checksum error\n");
			}
		} else {
			printf("No response from DHT11\n");
		}
		sleep_ms(5000); // Read every 2 minutes
	}
	return 0;
}

void setup_gpios(void) {
	// Initialize i2c at 400kHz
	i2c_init(i2c1, 400000);
	// Set i2c pins
	gpio_set_function(DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(DISPLAY_SCL_PIN, GPIO_FUNC_I2C);
	// Enable pull-ups i2c pins
	gpio_pull_up(DISPLAY_SDA_PIN);
	gpio_pull_up(DISPLAY_SCL_PIN);

}
