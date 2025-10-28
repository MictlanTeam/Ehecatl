#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <sensors.h>
#include <motors.h>
#include <utils.h>

/**
 * @brief Constantes del controlador PID
 *
 */
#define PID_KP 0.2
#define PID_KD 0.80

/**
 * @brief Tiempo de espera entre ejecuciones del bucle de control en microsegundos
 *
 */
#define CONTROL_LOOP_US 1000

/**
 * @brief Tiempo máximo sin detectar línea antes de detener el robot (ms)
 *
 */
#define LINE_LOST_TIMEOUT_MS 250

/**
 * @brief Velocidad de la turbina durante la carrera (0-100%)
 *
 */
#define FAN_SPEED 80

void set_base_speed(int speed);
void set_base_accel_speed(int accel_speed);
void set_base_fan_speed(int speed);
int get_base_fan_speed();
void set_race_started(bool started);
void set_race_starting(bool starting);
bool is_race_started();
bool is_race_starting();
long get_race_started_ms();
long get_race_stopped_ms();
void initial_control_loop();
void control_loop();

#endif // CONTROL_H
