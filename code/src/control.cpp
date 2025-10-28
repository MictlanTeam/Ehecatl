#include <control.h>

static long last_control_loop_us = 0;
static int position = 0;
static int last_error = 0;

static int base_speed = 30;
static int base_accel_speed = 60;
static int base_fan_speed = FAN_SPEED;
static int speed = 0;

static bool race_started = false;
static bool race_starting = false;
static long race_started_ms = 0;
static long race_stopped_ms = 0;

/**
 * @brief Realiza el cálculo de la corrección del controlador PID
 *
 * @param error Desplazamiento del robot respecto a la línea
 * @return float Corrección del controlador PID
 */
static float calc_correction(int error) {
  float p = PID_KP * error;
  float d = PID_KD * (error - last_error);
  last_error = error;
  return p + d;
}

/**
 * @brief Establece el estado de la carrera
 *
 * @param started Indica si la carrera ha comenzado
 */
void set_race_started(bool started) {
  race_started = started;
  if (started) {
    race_started_ms = millis();
    speed = 0;
    position = 0;
    last_error = 0;
    race_starting = false;  // Ya no está en pre-inicio
    set_led(true);          // Encender LED
    Serial.println(">>> CARRERA INICIADA <<<");
  } else {
    race_stopped_ms = millis();
    stop_motors();          // Apaga motores y turbina
    set_led(false);         // Apagar LED
    Serial.println(">>> CARRERA DETENIDA <<<");
  }
}

/**
 * @brief Establece el estado de pre-inicio (starting)
 *
 * @param starting Indica si está en pre-inicio
 */
void set_race_starting(bool starting) {
  race_starting = starting;
}

/**
 * @brief Comprueba si la carrera está en curso
 *
 * @return true En carrera
 * @return false En espera
 */
bool is_race_started() {
  return race_started;
}

/**
 * @brief Comprueba si está en pre-inicio
 *
 * @return true En pre-inicio
 * @return false No en pre-inicio
 */
bool is_race_starting() {
  return race_starting;
}

/**
 * @brief Obtiene los ms de inicio de la carrera
 *
 * @return long Tiempo de inicio de la carrera en ms
 */
long get_race_started_ms() {
  return race_started_ms;
}

/**
 * @brief Obtiene los ms de detención de la carrera
 *
 * @return long Tiempo de detención de la carrera en ms
 */
long get_race_stopped_ms() {
  return race_stopped_ms;
}

/**
 * @brief Establece la velocidad base del robot
 *
 * @param speed Velocidad base (0-100)
 */
void set_base_speed(int speed) {
  base_speed = constrain(speed, 0, 100);
  Serial.print("Velocidad base: ");
  Serial.println(base_speed);
}

/**
 * @brief Establece la aceleración inicial del robot
 *
 * @param accel_speed Aceleración inicial (0-100)
 */
void set_base_accel_speed(int accel_speed) {
  base_accel_speed = constrain(accel_speed, 0, 100);
  Serial.print("Aceleracion: ");
  Serial.println(base_accel_speed);
}

/**
 * @brief Establece la velocidad base de la turbina/succión
 *
 * @param speed Velocidad de la turbina (0-100)
 */
void set_base_fan_speed(int speed) {
  base_fan_speed = constrain(speed, 0, 100);
  Serial.print("Velocidad turbina: ");
  Serial.println(base_fan_speed);
}

/**
 * @brief Obtiene la velocidad base de la turbina
 *
 * @return int Velocidad base de la turbina
 */
int get_base_fan_speed() {
  return base_fan_speed;
}

/**
 * @brief Bucle de control inicial (pre-inicio)
 * Se ejecuta durante el último segundo de cuenta regresiva
 * Mantiene el robot centrado en la línea sin avanzar
 * Activa la turbina al 85% para preparar la succión
 *
 */
void initial_control_loop() {
  if (micros() - last_control_loop_us > CONTROL_LOOP_US || micros() < last_control_loop_us) {

    // Obtener posición de la línea
    position = get_sensor_position(position);

    // Calcular corrección PID
    int correction = calc_correction(position);

    // Aplicar solo corrección sin avanzar (giro en el lugar)
    set_motors_speed(correction, -correction);

    last_control_loop_us = micros();
  }
}

/**
 * @brief Bucle de control principal
 * Realiza el cálculo de la corrección del controlador PID y establece la velocidad de los motores
 * Esta función debe llamarse lo más frecuentemente posible
 *
 */
void control_loop() {
  if (micros() - last_control_loop_us > CONTROL_LOOP_US || micros() < last_control_loop_us) {

    // Obtener posición de la línea
    position = get_sensor_position(position);

    // Calcular corrección PID
    int correction = calc_correction(position);

    // Verificar si se perdió la línea
    if (millis() - get_last_line_detected_ms() > LINE_LOST_TIMEOUT_MS) {
      set_motors_speed(0, 0);
      set_fan_speed(0);
      set_race_started(false);
      Serial.println("LINEA PERDIDA - Robot detenido");
    } else {

      // Aceleración gradual mejorada (velocidad mínima de 20%)
      if (speed < base_speed) {
        float time_elapsed = (millis() - race_started_ms) / 1000.0f;
        speed = 20 + (base_accel_speed * time_elapsed);
        if (speed > base_speed) {
          speed = base_speed;
        }
      }

      // Aplicar velocidades con corrección PID
      float left_speed = speed + correction;
      float right_speed = speed - correction;

      set_motors_speed(left_speed, right_speed);

      // Activar turbina si está configurada
      if (base_fan_speed > 0) {
        set_fan_speed(base_fan_speed);
      }
    }

    last_control_loop_us = micros();
  }
}
