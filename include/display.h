/**
 * @file display.h
 *
 * @brief Header file for the display functions.
 *
 * @author jjoanr
 * @date 2024-06-17
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"

/**
 * @brief Displays the data received by parameter.
 * @param data Humidity & Temperature data.
 *	       1st and 2nd byte -> integer + decimal humidity data
 *	       3rd and 4th byte -> integer + decimal temperature data
 *	       5th byte		-> not used (checksum)
 */
void display_data(uint8_t data[5], ssd1306_t *disp);

#endif /* DISPLAY_H */
