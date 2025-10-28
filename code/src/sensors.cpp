#include <sensors.h>

static int sensors_raw[SENSORS_COUNT];
static long sensors_refresh_us = 0;

static int sensors_max[SENSORS_COUNT];
static int sensors_min[SENSORS_COUNT];
static int sensors_threshold[SENSORS_COUNT];

static long last_line_detected_ms = 0;

/**
 * @brief Configura los pines CBA del multiplexor según el canal (0-7)
 *
 * @param channel Canal del multiplexor (0-7)
 */
static void set_mux_channel(int channel) {
  digitalWrite(MUX_A, bitRead(channel, 0)); // Bit A (LSB)
  digitalWrite(MUX_B, bitRead(channel, 1)); // Bit B
  digitalWrite(MUX_C, bitRead(channel, 2)); // Bit C (MSB)
}

/**
 * @brief Inicializa los pines de los sensores
 *
 */
void init_sensors() {
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(SENSOR_1_8, INPUT);
  pinMode(SENSOR_9_16, INPUT);

  // Inicializar valores de calibración
  for (int i = 0; i < SENSORS_COUNT; i++) {
    sensors_min[i] = SENSORS_MAX;
    sensors_max[i] = SENSORS_MIN;
    sensors_threshold[i] = 0;
  }

  set_mux_channel(0);
}

/**
 * @brief Actualiza los valores de los sensores cada 1ms
 * Se van leyendo los valores de dos en dos por cada estado de los multiplexores
 * La lectura se realiza simétricamente desde el centro hacia los extremos
 *
 */
static void refresh_sensors() {
  if (micros() - sensors_refresh_us >= 1000 || micros() < sensors_refresh_us) {
    // Canal 0 (CBA=000): Sensores 8 y 9 (centro)
    set_mux_channel(0);
    delayMicroseconds(10);
    sensors_raw[7] = analogRead(SENSOR_1_8);
    sensors_raw[8] = analogRead(SENSOR_9_16);

    // Canal 1 (CBA=001): Sensores 7 y 10
    set_mux_channel(1);
    delayMicroseconds(10);
    sensors_raw[6] = analogRead(SENSOR_1_8);
    sensors_raw[9] = analogRead(SENSOR_9_16);

    // Canal 2 (CBA=010): Sensores 6 y 11
    set_mux_channel(2);
    delayMicroseconds(10);
    sensors_raw[5] = analogRead(SENSOR_1_8);
    sensors_raw[10] = analogRead(SENSOR_9_16);

    // Canal 3 (CBA=011): Sensores 5 y 12
    set_mux_channel(3);
    delayMicroseconds(10);
    sensors_raw[4] = analogRead(SENSOR_1_8);
    sensors_raw[11] = analogRead(SENSOR_9_16);

    // Canal 4 (CBA=100): Sensores 4 y 13
    set_mux_channel(4);
    delayMicroseconds(10);
    sensors_raw[3] = analogRead(SENSOR_1_8);
    sensors_raw[12] = analogRead(SENSOR_9_16);

    // Canal 5 (CBA=101): Sensores 3 y 14
    set_mux_channel(5);
    delayMicroseconds(10);
    sensors_raw[2] = analogRead(SENSOR_1_8);
    sensors_raw[13] = analogRead(SENSOR_9_16);

    // Canal 6 (CBA=110): Sensores 2 y 15
    set_mux_channel(6);
    delayMicroseconds(10);
    sensors_raw[1] = analogRead(SENSOR_1_8);
    sensors_raw[14] = analogRead(SENSOR_9_16);

    // Canal 7 (CBA=111): Sensores 1 y 16 (extremos)
    set_mux_channel(7);
    delayMicroseconds(10);
    sensors_raw[0] = analogRead(SENSOR_1_8);
    sensors_raw[15] = analogRead(SENSOR_9_16);

    // Volver al canal 0
    set_mux_channel(0);

    sensors_refresh_us = micros();
  }
}

/**
 * @brief Calibra los sensores obteniendo los valores máximos y mínimos
 * El umbral se calcula como el 2/3 del rango de valores entre el máximo y el mínimo
 *
 */
void calibrate_sensors() {
  Serial.println("==============================================");
  Serial.println("CALIBRACION DE SENSORES");
  Serial.println("==============================================");
  Serial.println("Mueve el robot sobre la linea durante 3 segundos...");
  Serial.println();

  long calibration_start_ms = millis();
  int count_ok = 0;

  do {
    count_ok = 0;
    for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
      int sensor_value = get_sensor_raw(sensor);
      if (sensor_value > sensors_max[sensor]) {
        sensors_max[sensor] = sensor_value;
      }
      if (sensor_value < sensors_min[sensor]) {
        sensors_min[sensor] = sensor_value;
      }
      // Umbral = min + 2/3 del rango
      sensors_threshold[sensor] = sensors_min[sensor] + ((sensors_max[sensor] - sensors_min[sensor]) * 2 / 3);

      // Verificar que hay suficiente contraste
      if (abs(sensors_max[sensor] - sensors_min[sensor]) >= 1000) {
        count_ok++;
      }
    }

    // Mostrar progreso cada 500ms
    if ((millis() - calibration_start_ms) % 500 < 50) {
      Serial.print(".");
    }

  } while (millis() - calibration_start_ms < SENSORS_CALIBRATION_MS);

  Serial.println();
  Serial.println();
  Serial.println("Calibracion completa:");
  Serial.print("Sensores con buen contraste: ");
  Serial.print(count_ok);
  Serial.print("/");
  Serial.println(SENSORS_COUNT);

  // Mostrar valores de calibración
  Serial.println();
  Serial.println("S# | Min  | Max  | Umbral");
  Serial.println("---+------+------+-------");
  for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
    Serial.print(sensor + 1);
    Serial.print("  | ");
    Serial.print(sensors_min[sensor]);
    Serial.print(" | ");
    Serial.print(sensors_max[sensor]);
    Serial.print(" | ");
    Serial.println(sensors_threshold[sensor]);
  }

  Serial.println("==============================================");
  Serial.println();

  if (count_ok < SENSORS_COUNT) {
    Serial.println("ADVERTENCIA: Algunos sensores no tienen suficiente contraste!");
    Serial.println();
  }

  delay(1000);
}

/**
 * @brief Obtiene el valor sin procesar de un sensor
 *
 * @param sensor Sensor a leer (0-15)
 * @return int Valor sin procesar del sensor
 */
int get_sensor_raw(int sensor) {
  if (sensor >= 0 && sensor < SENSORS_COUNT) {
    refresh_sensors();
    return sensors_raw[sensor];
  }
  return -1;
}

/**
 * @brief Obtiene el valor calibrado de un sensor (binario: sobre línea o no)
 *
 * @param sensor Sensor a leer (0-15)
 * @return int SENSORS_MAX si detecta línea, SENSORS_MIN si no
 */
int get_sensor_calibrated(int sensor) {
  if (sensor >= 0 && sensor < SENSORS_COUNT) {
    refresh_sensors();
    return sensors_raw[sensor] >= sensors_threshold[sensor] ? SENSORS_MAX : SENSORS_MIN;
  }
  return -1;
}

/**
 * @brief Obtiene la posición del robot en la pista
 * Calcula la posición ponderada de la línea usando todos los sensores
 *
 * @param last_position Última posición conocida del robot
 * @return int Posición del robot (-SENSORS_POSITION_MAX a +SENSORS_POSITION_MAX)
 */
int get_sensor_position(int last_position) {
  long sum_sensors_weight = 0;
  long sum_sensors = 0;
  int count_sensors_detecting = 0;

  for (int sensor = 0; sensor < SENSORS_COUNT; sensor++) {
    int sensor_value = get_sensor_calibrated(sensor);
    if (sensor_value >= sensors_threshold[sensor]) {
      count_sensors_detecting++;
    }
    // Peso: sensor 0 (izq) = 1000, sensor 15 (der) = 16000
    sum_sensors_weight += (sensor + 1) * sensor_value * 1000;
    sum_sensors += sensor_value;
  }

  int position_max = ((1000 * (SENSORS_COUNT + 1)) / 2);
  int position = 0;

  // Si detecta la línea (no todos los sensores en negro ni todos en blanco)
  if (count_sensors_detecting > 0 && count_sensors_detecting < SENSORS_COUNT) {
    position = (sum_sensors_weight / sum_sensors) - position_max;
    last_line_detected_ms = millis();
  } else {
    // Línea perdida, mantener última dirección
    position = last_position >= 0 ? position_max : -position_max;
  }

  // Mapear a rango -SENSORS_POSITION_MAX a +SENSORS_POSITION_MAX
  return map(position, -position_max, position_max, -SENSORS_POSITION_MAX, SENSORS_POSITION_MAX);
}

/**
 * @brief Obtiene el tiempo en ms desde la última vez que se detectó la línea
 *
 * @return long Tiempo en ms de la última detección de línea
 */
long get_last_line_detected_ms() {
  return last_line_detected_ms;
}

/**
 * @brief Imprime los valores raw de todos los sensores
 *
 */
void print_sensors_raw() {
  Serial.print("RAW: ");
  for (int i = 0; i < SENSORS_COUNT; i++) {
    Serial.print(sensors_raw[i]);
    if (i < SENSORS_COUNT - 1) Serial.print("\t");
  }
  Serial.println();
}

/**
 * @brief Imprime los valores calibrados de todos los sensores
 *
 */
void print_sensors_calibrated() {
  Serial.print("CAL: ");
  for (int i = 0; i < SENSORS_COUNT; i++) {
    Serial.print(get_sensor_calibrated(i) == SENSORS_MAX ? "1" : "0");
    if (i < SENSORS_COUNT - 1) Serial.print(" ");
  }
  Serial.println();
}
