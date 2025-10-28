#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <pinout.h>

/**
 * @brief Estados del botón
 *
 */
enum BTN_STATES {
  BTN_IDLE,           // Botón no presionado
  BTN_PRESSING,       // Botón siendo presionado
  BTN_PRESSED,        // Botón presionado brevemente (>250ms)
  BTN_LONG_PRESSED    // Botón presionado largo tiempo (>1000ms)
};

/**
 * @brief Tiempo mínimo para considerar presión del botón (ms)
 *
 */
#define BTN_DEBOUNCE_MS 50
#define BTN_PRESS_MS 250
#define BTN_LONG_PRESS_MS 1000

void init_utils();
BTN_STATES get_btn_state();
long get_btn_pressing_ms();
bool get_start_signal();
void set_led(bool state);
void blink_led(int interval_ms);
void set_fan(int speed);

#endif // UTILS_H
