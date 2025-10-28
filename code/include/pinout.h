#ifndef PINOUT_H
#define PINOUT_H

/**
 * @brief Pin del LED de estado
 *
 */
#define LED_PIN 5

/**
 * @brief Pin del botón de inicio
 *
 */
#define BUTTON_PIN 13

/**
 * @brief Pin de la señal de start (entrada)
 *
 */
#define START_SIGNAL_PIN 6

/**
 * @brief Pin de control de la turbina/succión
 *
 */
#define FAN_PIN 12

/**
 * @brief Pines de control de multiplexadores (compartidos)
 *
 */
#define MUX_A 3
#define MUX_B 11
#define MUX_C 10

/**
 * @brief Pines de lectura de sensores
 *
 */
#define SENSOR_1_8 8    // Lee sensores 1-8
#define SENSOR_9_16 7   // Lee sensores 9-16

/**
 * @brief Pines del driver RZ7886 - Motor Izquierdo
 *
 */
#define MOTOR_LEFT_A 1
#define MOTOR_LEFT_B 4

/**
 * @brief Pines del driver RZ7886 - Motor Derecho
 *
 */
#define MOTOR_RIGHT_A 9
#define MOTOR_RIGHT_B 2

#endif // PINOUT_H
