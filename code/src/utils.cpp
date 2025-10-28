#include <utils.h>

static long btn_pressed_start_ms = 0;
static bool btn_last_state = false;
static BTN_STATES btn_current_state = BTN_IDLE;

static long last_blink_ms = 0;
static bool led_blink_state = false;

/**
 * @brief Inicializa los pines de utilidades (botón, LED, start signal, turbina)
 *
 */
void init_utils() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Botón con pull-up interno
  pinMode(START_SIGNAL_PIN, INPUT);    // Señal de start
  pinMode(LED_PIN, OUTPUT);            // LED
  pinMode(FAN_PIN, OUTPUT);            // Turbina

  digitalWrite(LED_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
}

/**
 * @brief Lee el estado del botón y detecta presiones cortas y largas
 *
 * @return BTN_STATES Estado actual del botón
 */
BTN_STATES get_btn_state() {
  bool btn_reading = !digitalRead(BUTTON_PIN); // Invertido porque usa pull-up

  // Detección de flanco
  if (btn_reading && !btn_last_state) {
    // Botón recién presionado
    btn_pressed_start_ms = millis();
    btn_current_state = BTN_PRESSING;
  } else if (!btn_reading && btn_last_state) {
    // Botón recién liberado
    long press_duration = millis() - btn_pressed_start_ms;

    if (press_duration >= BTN_LONG_PRESS_MS) {
      btn_current_state = BTN_LONG_PRESSED;
    } else if (press_duration >= BTN_PRESS_MS) {
      btn_current_state = BTN_PRESSED;
    } else {
      btn_current_state = BTN_IDLE;
    }
  } else if (btn_reading) {
    // Botón sigue presionado
    btn_current_state = BTN_PRESSING;
  } else {
    // Botón no presionado
    if (btn_current_state != BTN_IDLE) {
      // Retornar el estado una vez después de soltar
      BTN_STATES temp = btn_current_state;
      btn_current_state = BTN_IDLE;
      btn_last_state = btn_reading;
      return temp;
    }
    btn_current_state = BTN_IDLE;
  }

  btn_last_state = btn_reading;
  return btn_current_state;
}

/**
 * @brief Obtiene el tiempo que lleva presionado el botón
 *
 * @return long Tiempo en ms
 */
long get_btn_pressing_ms() {
  if (btn_current_state == BTN_PRESSING) {
    return millis() - btn_pressed_start_ms;
  }
  return 0;
}

/**
 * @brief Lee la señal de start externa
 *
 * @return true Si la señal está activa
 * @return false Si la señal está inactiva
 */
bool get_start_signal() {
  return digitalRead(START_SIGNAL_PIN);
}

/**
 * @brief Enciende o apaga el LED
 *
 * @param state true=encendido, false=apagado
 */
void set_led(bool state) {
  digitalWrite(LED_PIN, state ? HIGH : LOW);
  led_blink_state = state;
}

/**
 * @brief Hace parpadear el LED
 *
 * @param interval_ms Intervalo de parpadeo en ms
 */
void blink_led(int interval_ms) {
  if (millis() - last_blink_ms >= interval_ms) {
    led_blink_state = !led_blink_state;
    digitalWrite(LED_PIN, led_blink_state ? HIGH : LOW);
    last_blink_ms = millis();
  }
}

/**
 * @brief Controla la velocidad de la turbina/succión
 *
 * @param speed Velocidad (0-100%)
 */
void set_fan(int speed) {
  speed = constrain(speed, 0, 100);
  int pwm_value = map(speed, 0, 100, 0, 255);
  analogWrite(FAN_PIN, pwm_value);
}
