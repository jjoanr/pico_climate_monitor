/**
 * @file dht11.h
 *
 * @brief Header file for the dht11 sensor driver.
 *
 * @author jjoanr
 * @date 2024-06-16
 */

#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the gpio which will be used for communication with the sensor.
 * @param uint ID for the gpio to be used for the communication.
 */
void init_dht11(unsigned int gpio);

/**
 * @brief Sends start signal to the sensor.
 */
void send_start_signal();

/**
 * @brief Checks the reponse given by the sensor.
 * @return bool Returns true if the sensor responds correctly, false otherwise.
 */
bool check_response();

/**
 * @brief Reads the data being sent by the sensor and stores it in the given array.
 * @param[out] data Array of 5 bytes to store the sensor response.
 */
void read_data(uint8_t data[5]);

/**
 * @brief Verifies checksum of the received data.
 * @param data Array of 5 bytes received from the sensor.
 * @return bool Returns true if the checksum is correct, false otherwise.
 */
bool verify_checksum(uint8_t data[5]);

#endif /* DHT11_H */
