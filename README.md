# EHECATL - Line Follower Racing Robot

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-green.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

**EHECATL** (pronunciado "Ehécatl") es un robot seguidor de línea de alta velocidad diseñado para competencias de racing. Su nombre proviene del dios azteca del viento, representando su característica principal: un sistema de succión mediante turbina para generar efecto ground y aumentar la tracción en curvas a alta velocidad.

## 🏆 Características Principales

### Hardware
- **Microcontrolador**: ESP32-S3-Zero
- **Sensores**: Array de 16 sensores IR con multiplexadores duales
- **Motores**: 2 motores DC con drivers RZ7886
- **Turbina**: Sistema de succión con ESC brushless (efecto ground)
- **Control**: Botón físico, señal externa de start, y comandos serial

### Software
- **Control PID**: Controlador proporcional-derivativo ajustable
- **Pre-inicio Inteligente**: Estabilización y activación de turbina antes del arranque
- **Aceleración Adaptativa**: Velocidad mínima de 20% para arranques rápidos
- **Protocolo ESC**: Control de turbina a 50Hz (protocolo servo estándar)
- **Lectura Simétrica**: Sensores leídos desde el centro hacia extremos
- **Calibración Automática**: Auto-calibración con umbral adaptativo

## 🛠️ Especificaciones Técnicas

### Mapa de Pines

| Componente | Pin | Tipo | Descripción |
|------------|-----|------|-------------|
| **Motores** |
| Motor Izq A | 1 | PWM | Control motor izquierdo |
| Motor Izq B | 4 | PWM | Control motor izquierdo |
| Motor Der A | 9 | PWM | Control motor derecho |
| Motor Der B | 2 | PWM | Control motor derecho |
| **Sensores** |
| MUX_A | 3 | Digital Out | Control multiplexador (LSB) |
| MUX_B | 11 | Digital Out | Control multiplexador |
| MUX_C | 10 | Digital Out | Control multiplexador (MSB) |
| Sensores 1-8 | 8 | Analog In | Lectura multiplexador 1 |
| Sensores 9-16 | 7 | Analog In | Lectura multiplexador 2 |
| **Utilidades** |
| LED | 5 | Digital Out | LED de estado |
| Botón | 13 | Digital In | Botón con pull-up |
| Start Signal | 6 | Digital In | Señal de inicio externa |
| Turbina | 12 | PWM (ESC) | Control turbina/succión |

### Configuraciones PWM

#### Motores
- **Frecuencia**: 5 kHz
- **Resolución**: 10 bits (0-1023)
- **Canales**: 0-3

#### Turbina (ESC)
- **Frecuencia**: 50 Hz (protocolo servo)
- **Resolución**: 11 bits
- **Rango**: 102-204 (1000μs-2000μs)
- **Canal**: 4

### Parámetros PID

```cpp
KP = 0.2    // Ganancia proporcional
KD = 0.80   // Ganancia derivativa
```

### Tiempos de Control

- **Loop de control**: 1000 μs (1 kHz)
- **Timeout pérdida de línea**: 250 ms
- **Calibración sensores**: 3000 ms
- **Tiempo máximo de prueba**: 30000 ms

## 📁 Estructura del Proyecto

```
Ehecatl/
├── code/
│   ├── include/
│   │   ├── pinout.h      # Definición de pines
│   │   ├── sensors.h     # Header de sensores
│   │   ├── motors.h      # Header de motores
│   │   ├── control.h     # Header de control PID
│   │   └── utils.h       # Header de utilidades
│   ├── src/
│   │   ├── main.cpp      # Programa principal
│   │   ├── sensors.cpp   # Lectura y calibración de sensores
│   │   ├── motors.cpp    # Control de motores y turbina
│   │   ├── control.cpp   # Lógica de control PID
│   │   └── utils.cpp     # Botón, LED, y utilidades
│   ├── platformio.ini    # Configuración de PlatformIO
│   └── README.md
├── pcb/                  # Archivos de diseño PCB
├── 3d/                   # Modelos 3D
└── docs/                 # Documentación adicional
```

## 🚀 Instalación

### Requisitos

- [PlatformIO](https://platformio.org/) (recomendado)
- O [Arduino IDE](https://www.arduino.cc/en/software) con soporte ESP32

### Dependencias

- **ESP32 Core**: v2.0.0 o superior
- **Framework**: Arduino

### Pasos de Instalación

1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/tu-usuario/Ehecatl.git
   cd Ehecatl/code
   ```

2. **Abrir con PlatformIO**
   ```bash
   pio init
   ```

3. **Compilar**
   ```bash
   pio run
   ```

4. **Cargar al ESP32-S3**
   ```bash
   pio run -t upload
   ```

5. **Abrir monitor serial**
   ```bash
   pio device monitor -b 115200
   ```

## ⚙️ Configuración

### Configuración de Flash (platformio.ini)

```ini
[env:esp32-s3-zero]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
board_build.mcu = esp32s3
board_build.f_cpu = 240000000L
board_build.flash_mode = qio
board_upload.flash_size = 4MB
board_build.arduino.memory_type = qio_opi
board_build.psram_type = opi
board_build.partitions = default.csv
monitor_speed = 115200
upload_speed = 921600
```

### Ajuste de Parámetros

Los parámetros principales se pueden ajustar en:

- **`control.h`**: Constantes PID, tiempos de control
- **`motors.h`**: Configuración PWM de motores y turbina
- **`sensors.h`**: Configuración de sensores

## 📱 Uso Básico

### Inicio del Robot

1. **Encendido**: Conectar alimentación
2. **Inicialización**: Esperar 2 segundos (ESC de turbina)
3. **Calibración**: Presionar botón → Mover robot sobre línea 3 segundos
4. **Listo**: LED indica que está listo para iniciar

### Modos de Inicio

El robot puede iniciarse de 3 formas:

1. **Botón Largo (>1s)**:
   - Presionar y mantener botón
   - Cuenta regresiva de 3 segundos
   - Último segundo: pre-inicio con turbina al 85%
   - ¡GO!

2. **Señal Externa (Pin 6)**:
   - Compatible con sistemas de competencia
   - Inicio inmediato al detectar HIGH en pin 6

3. **Comando Serial (`s`)**:
   - Enviar comando `s` por monitor serial
   - Cuenta regresiva de 3 segundos
   - Inicio automático

### Comandos Serial

| Comando | Función | Ejemplo |
|---------|---------|---------|
| `s` | Iniciar carrera | - |
| `x` | Detener carrera | - |
| `r` | Mostrar sensores RAW | - |
| `c` | Mostrar sensores calibrados | - |
| `v[num]` | Cambiar velocidad base (0-100%) | `v40` |
| `a[num]` | Cambiar aceleración (0-100%) | `a70` |
| `f[num]` | Cambiar velocidad turbina (0-100%) | `f90` |
| `cal` | Re-calibrar sensores | - |

## 🎛️ Funcionalidades Avanzadas

### Pre-inicio Inteligente

Durante el último segundo de cuenta regresiva:
- Turbina se activa al 85%
- Control PID sin avanzar (mantiene centrado)
- Robot estabilizado antes de arrancar

### Aceleración Mejorada

- Velocidad inicial: 20%
- Aceleración gradual según configuración
- Previene arranques lentos

### Protección de Motores

Los motores solo funcionan cuando:
- Carrera iniciada
- Pre-inicio activo
- 1 segundo después de detenerse (freno gradual)

### Detección de Pérdida de Línea

- Timeout: 250ms
- Detención automática de motores y turbina
- Mensaje en serial

## 🔧 Desarrollo

### Agregar Nuevas Funciones

1. Crear funciones en archivos `.cpp` correspondientes
2. Declarar en headers `.h`
3. Llamar desde `main.cpp`

### Ajustar PID

```cpp
// En control.h
#define PID_KP 0.2   // Aumentar para respuesta más agresiva
#define PID_KD 0.80  // Aumentar para reducir oscilaciones
```

### Cambiar Configuración de Sensores

```cpp
// En sensors.h
#define SENSORS_COUNT 16
#define SENSORS_CALIBRATION_MS 3000
#define SENSORS_POSITION_MAX 255
```

## 🐛 Solución de Problemas

| Problema | Causa | Solución |
|----------|-------|----------|
| Flash size error | Configuración incorrecta | Verificar `platformio.ini` |
| ESC no responde | Inicialización incompleta | Esperar 2-3 segundos al encender |
| Sensores sin contraste | Calibración insuficiente | Re-calibrar con `cal` |
| Robot no arranca | Botón no detectado | Verificar pin 13 con pull-up |
| Motores no responden | Estado incorrecto | Verificar que esté en carrera |

## 📊 Rendimiento

### Especificaciones de Velocidad

- **Velocidad base**: Configurable 0-100%
- **Aceleración**: Configurable 0-100%
- **Velocidad turbina**: Configurable 0-100%
- **Frecuencia de control**: 1 kHz
- **Tiempo de respuesta**: ~1 ms

### Consumo

- **Standby**: ~50 mA
- **Motores (50%)**: ~500 mA
- **Turbina (80%)**: ~2-4 A
- **Total (carrera)**: ~3-5 A

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crear branch (`git checkout -b feature/mejora`)
3. Commit cambios (`git commit -m 'Agregar mejora'`)
4. Push al branch (`git push origin feature/mejora`)
5. Abrir Pull Request

## 📝 Licencia

Este proyecto está bajo la Licencia MIT. Ver archivo `LICENSE` para más detalles.

## 🙏 Créditos

- **Inspirado en**: PX-01 Basic & Pro by PololuRacing
- **Hardware**: ESP32-S3 by Espressif Systems
- **Framework**: Arduino & PlatformIO

## 📧 Contacto

- **Autor**: Samuel
- **GitHub**: [Tu perfil de GitHub]
- **Email**: [Tu email]

## 🔗 Enlaces Útiles

- [Documentación ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [PlatformIO Docs](https://docs.platformio.org/)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)

---

**EHECATL** - El viento que domina la pista 💨🏎️
