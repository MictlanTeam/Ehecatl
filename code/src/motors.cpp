#include <motors.h>
#include <control.h>

/**
 * @brief Inicializa los motores configurando los canales PWM
 *
 */
void init_motors() {
  // Configuración de los canales PWM del Timer - Motores
  ledcSetup(PWM_MOTOR_LEFT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_LEFT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_RIGHT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_RIGHT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);

  // Configuración del canal PWM para ESC de turbina (50Hz = protocolo servo)
  ledcSetup(PWM_FAN, PWM_FAN_HZ, PWM_FAN_RESOLUTION);

  // Asignación de los pines a los canales PWM
  ledcAttachPin(MOTOR_LEFT_A, PWM_MOTOR_LEFT_A);
  ledcAttachPin(MOTOR_LEFT_B, PWM_MOTOR_LEFT_B);
  ledcAttachPin(MOTOR_RIGHT_A, PWM_MOTOR_RIGHT_A);
  ledcAttachPin(MOTOR_RIGHT_B, PWM_MOTOR_RIGHT_B);
  ledcAttachPin(FAN_PIN, PWM_FAN);

  // Establece el valor inicial de los canales PWM
  ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_FAN, PWM_FAN_MIN);

  // Tiempo de espera para inicialización del ESC
  // El ESC necesita 2-3 segundos para calibrar
  // Este delay se puede quitar si ya se espera durante la calibración de sensores
  Serial.println("Inicializando ESC de turbina...");
  delay(2000);
  Serial.println("ESC listo");
}

/**
 * @brief Establece la velocidad de los motores
 * Motor Izquierdo: MOTOR_LEFT_A y MOTOR_LEFT_B
 * Motor Derecho: MOTOR_RIGHT_A y MOTOR_RIGHT_B
 *
 * Lógica del driver RZ7886:
 * - Para avanzar: A=HIGH, B=LOW con PWM en A
 * - Para retroceder: A=LOW, B=HIGH con PWM en B
 * - Para frenar: A=HIGH, B=HIGH
 *
 * @param velI Velocidad del motor izquierdo (-100 a 100%)
 * @param velD Velocidad del motor derecho (-100 a 100%)
 */
void set_motors_speed(float velI, float velD) {
  // Limitar velocidades
  velI = constrain(velI, -100, 100);
  velD = constrain(velD, -100, 100);

  // Solo permitir movimiento si está en carrera, pre-inicio, o recién detenido (freno gradual)
  bool motors_enabled = is_race_started() || is_race_starting() || (millis() - get_race_stopped_ms() < 1000);

  // Motor Izquierdo
  if (motors_enabled) {
    if (velI > 0) {
      // Adelante
      ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX);
      ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velI / 100));
    } else if (velI < 0) {
      // Reversa
      ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * abs(velI) / 100));
      ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX);
    } else {
      // Detenido
      ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
      ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
    }
  } else {
    // Motores deshabilitados
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
  }

  // Motor Derecho
  if (motors_enabled) {
    if (velD > 0) {
      // Adelante
      ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX);
      ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velD / 100));
    } else if (velD < 0) {
      // Reversa
      ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * abs(velD) / 100));
      ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX);
    } else {
      // Detenido
      ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
      ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
    }
  } else {
    // Motores deshabilitados
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
  }
}

/**
 * @brief Establece la velocidad de la turbina/succión usando protocolo ESC
 * Protocolo servo: 1000μs = apagado, 2000μs = máximo
 *
 * @param vel Velocidad de la turbina (0-100%)
 */
void set_fan_speed(int vel) {
  vel = constrain(vel, 0, 100);
  if (vel != 0) {
    // Mapear 0-100% a rango 102-204 (1000μs a 2000μs)
    int pwm_value = map(vel, 0, 100, PWM_FAN_MIN, PWM_FAN_MAX);
    ledcWrite(PWM_FAN, pwm_value);
  } else {
    // Apagar turbina
    ledcWrite(PWM_FAN, PWM_FAN_MIN);
  }
}

/**
 * @brief Detiene ambos motores y la turbina
 *
 */
void stop_motors() {
  ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_FAN, PWM_FAN_MIN);
}
