#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <pinout.h>

/**
 * @brief Configuración de los motores de tracción
 * Canales: 0 a 3
 * Frecuencia: 5 kHz
 * Resolución: 10 bits
 * Rango: 0 a 1023
 *
 */
#define PWM_MOTOR_LEFT_A 0
#define PWM_MOTOR_LEFT_B 1
#define PWM_MOTOR_RIGHT_A 2
#define PWM_MOTOR_RIGHT_B 3
#define PWM_MOTORS_HZ 5000
#define PWM_MOTORS_RESOLUTION 10
#define PWM_MOTORS_MAX 1023
#define PWM_MOTORS_MIN 0

/**
 * @brief Configuración del ESC de la turbina/succión
 * Canal: 4
 * Frecuencia: 50 Hz (protocolo servo/ESC)
 * Resolución: 11 bits
 * Rango: 102 a 204 (1000μs a 2000μs del protocolo servo)
 *
 */
#define PWM_FAN 4
#define PWM_FAN_HZ 50
#define PWM_FAN_RESOLUTION 11
#define PWM_FAN_MAX 204      // 2000μs (máxima velocidad)
#define PWM_FAN_MIN 102      // 1000μs (apagado/mínimo)

void init_motors();
void set_motors_speed(float velI, float velD);
void set_fan_speed(int vel);
void stop_motors();

#endif // MOTORS_H
