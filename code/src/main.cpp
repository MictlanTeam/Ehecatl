#include <Arduino.h>
#include <pinout.h>
#include <sensors.h>
#include <motors.h>
#include <control.h>
#include <utils.h>

/**
 * @brief Configuración del robot
 *
 */
#define TEST_DURATION_MS 30000   // Tiempo máximo de prueba: 30 segundos
#define START_DELAY_MS 3000      // Delay antes de iniciar: 3 segundos

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==============================================");
  Serial.println("  EHECATL - Line Follower Robot");
  Serial.println("  Basado en PX-01 Basic");
  Serial.println("==============================================");
  Serial.println();

  // Inicializar componentes
  Serial.println("Inicializando componentes...");
  init_utils();
  init_sensors();
  init_motors();

  Serial.println();
  Serial.println("==============================================");
  Serial.println("  CONFIGURACION DE PINES");
  Serial.println("==============================================");
  Serial.println("Motor Izquierdo:");
  Serial.println("  PWMA: Pin 1");
  Serial.println("  PWMB: Pin 4");
  Serial.println();
  Serial.println("Motor Derecho:");
  Serial.println("  PWMA: Pin 9");
  Serial.println("  PWMB: Pin 2");
  Serial.println();
  Serial.println("Sensores (Multiplexadores):");
  Serial.println("  MUX_A: Pin 3");
  Serial.println("  MUX_B: Pin 11");
  Serial.println("  MUX_C: Pin 10");
  Serial.println("  Lectura 1-8:  Pin 8");
  Serial.println("  Lectura 9-16: Pin 7");
  Serial.println();
  Serial.println("Utilidades:");
  Serial.println("  LED:     Pin 5");
  Serial.println("  Boton:   Pin 13");
  Serial.println("  Start:   Pin 6");
  Serial.println("  Turbina: Pin 12");
  Serial.println("==============================================");
  Serial.println();

  // LED parpadeando indicando calibración
  Serial.println("Presiona el boton para calibrar sensores...");
  while (get_btn_state() != BTN_PRESSED && get_btn_state() != BTN_LONG_PRESSED) {
    blink_led(500);
  }
  set_led(false);
  delay(500);

  // Calibrar sensores
  calibrate_sensors();

  // Configurar velocidades
  set_base_speed(30);        // Velocidad base: 30%
  set_base_accel_speed(60);  // Aceleración: 60%
  set_base_fan_speed(80);    // Turbina: 80%

  Serial.println("Sistema listo!");
  Serial.println();
  Serial.println("==============================================");
  Serial.println("  MODOS DE INICIO");
  Serial.println("==============================================");
  Serial.println("  1. Presion LARGA del boton (>1s)");
  Serial.println("  2. Senal de START (Pin 6)");
  Serial.println("  3. Comando serial: s");
  Serial.println();
  Serial.println("  OTROS COMANDOS:");
  Serial.println("  x - Detener carrera");
  Serial.println("  r - Mostrar sensores RAW");
  Serial.println("  c - Mostrar sensores calibrados");
  Serial.println("  v[num] - Cambiar velocidad base (ej: v40)");
  Serial.println("  a[num] - Cambiar aceleracion (ej: a70)");
  Serial.println("  f[num] - Cambiar velocidad turbina (ej: f90)");
  Serial.println("  cal - Re-calibrar sensores");
  Serial.println("==============================================");
  Serial.println();

  // LED indicando listo para iniciar
  set_led(true);
  delay(500);
  set_led(false);
}

void loop() {
  // Verificar si NO está en carrera
  if (!is_race_started()) {

    // Leer estado del botón
    BTN_STATES btn_state = get_btn_state();

    // Mientras el botón está presionado, parpadear LED rápido
    if (btn_state == BTN_PRESSING && get_btn_pressing_ms() >= 250) {
      blink_led(125);
    }

    // Si se soltó el botón después de presión larga, iniciar carrera
    if (btn_state == BTN_LONG_PRESSED) {
      Serial.println();
      Serial.println("Boton LARGO detectado!");
      set_led(true);

      // Cuenta regresiva con parpadeos (3 segundos)
      Serial.println("Iniciando en:");
      long starting_ms = millis();
      long countdown_duration = 3000;  // 3 segundos

      while (millis() < (starting_ms + countdown_duration)) {
        long elapsed = millis() - starting_ms;

        // Primeros 2 segundos: parpadeo lento
        if (elapsed < countdown_duration - 1000) {
          // Mostrar cuenta regresiva
          int seconds_remaining = 3 - (elapsed / 1000);
          static int last_second = -1;
          if (seconds_remaining != last_second) {
            Serial.print("  ");
            Serial.print(seconds_remaining);
            Serial.println("...");
            last_second = seconds_remaining;
          }

          // Parpadeo cada 500ms
          if ((elapsed % 1000) < 500) {
            set_led(true);
          } else {
            set_led(false);
          }

        } else {
          // Último segundo: PRE-INICIO INTELIGENTE
          set_led(true);
          set_race_starting(true);

          // Activar turbina al 85% para preparar succión
          if (get_base_fan_speed() > 0) {
            set_fan_speed(get_base_fan_speed() * 0.85f);
            Serial.println("  Pre-inicio: Turbina activada al 85%");
          }

          // Ejecutar control PID sin avanzar (mantiene centrado)
          initial_control_loop();
        }
      }

      Serial.println("  GO!");
      set_led(false);
      set_race_started(true);
    }

    // Verificar señal de START externa
    if (get_start_signal()) {
      Serial.println();
      Serial.println("Senal de START detectada!");
      delay(100);  // Pequeño delay para estabilizar
      set_race_started(true);
    }

    // Procesar comandos del serial
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();

      if (command == "s") {
        // Iniciar carrera con delay
        Serial.println();
        Serial.println("Iniciando en 3 segundos...");
        for (int i = 3; i > 0; i--) {
          Serial.print(i);
          Serial.println("...");
          delay(1000);
        }
        set_race_started(true);

      } else if (command == "r") {
        // Mostrar sensores RAW
        print_sensors_raw();

      } else if (command == "c") {
        // Mostrar sensores calibrados
        print_sensors_calibrated();

      } else if (command.startsWith("v")) {
        // Cambiar velocidad base
        int speed = command.substring(1).toInt();
        set_base_speed(speed);

      } else if (command.startsWith("a")) {
        // Cambiar aceleración
        int accel = command.substring(1).toInt();
        set_base_accel_speed(accel);

      } else if (command.startsWith("f")) {
        // Cambiar velocidad turbina
        int fan = command.substring(1).toInt();
        set_base_fan_speed(fan);

      } else if (command == "cal") {
        // Re-calibrar sensores
        Serial.println("Re-calibrando sensores...");
        calibrate_sensors();

      } else if (command == "x") {
        // Este comando solo funciona durante la carrera
        Serial.println("El robot no esta en carrera");

      } else {
        Serial.println("Comando no reconocido");
      }
    }

  } else {
    // EN CARRERA

    // Ejecutar bucle de control
    control_loop();

    // Verificar comando de detención por serial
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command == "x") {
        Serial.println();
        Serial.println("Detencion manual solicitada");
        set_race_started(false);
      }
    }

    // Verificar botón para detener
    BTN_STATES btn_state = get_btn_state();
    if (btn_state == BTN_PRESSED || btn_state == BTN_LONG_PRESSED) {
      Serial.println();
      Serial.println("Boton presionado - Deteniendo");
      set_race_started(false);
    }

    // Detener después del tiempo máximo de prueba
    if (millis() - get_race_started_ms() >= TEST_DURATION_MS) {
      Serial.println();
      Serial.println("Tiempo de prueba completado");
      set_race_started(false);
    }
  }
}
