#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <pinout.h>

/**
 * @brief Cantidad de sensores
 *
 */
#define SENSORS_COUNT 16

/**
 * @brief Valor máximo y mínimo de ADC (ESP32-S3 = 12 bits)
 *
 */
#define SENSORS_MAX 4095
#define SENSORS_MIN 0

/**
 * @brief Valor máximo de la posición sobre la línea
 * El rango de valores será de -SENSORS_POSITION_MAX a SENSORS_POSITION_MAX, con 0 como el punto central
 *
 */
#define SENSORS_POSITION_MAX 255

/**
 * @brief Tiempo de calibración de sensores en ms
 *
 */
#define SENSORS_CALIBRATION_MS 3000

void init_sensors();
void calibrate_sensors();
int get_sensor_raw(int sensor);
int get_sensor_calibrated(int sensor);
int get_sensor_position(int last_position);
long get_last_line_detected_ms();
void print_sensors_raw();
void print_sensors_calibrated();

#endif // SENSORS_H
