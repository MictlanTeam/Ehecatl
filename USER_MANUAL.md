# 📖 Manual de Usuario - EHECATL

## Guía de Operación del Robot Seguidor de Línea

---

## 🚦 Inicio Rápido

### 1. Primera Vez - Configuración Inicial

#### Paso 1: Conexión
1. Conecta la batería al robot
2. Espera 2 segundos (oirás un beep del ESC de la turbina)
3. El LED parpadeará indicando que está listo para calibrar

#### Paso 2: Calibración de Sensores
1. **Presiona el botón** del robot
2. **Mueve el robot lentamente** sobre la línea durante 3 segundos
   - Pasa por línea negra
   - Pasa por fondo blanco
   - Repite varias veces
3. Cuando termine:
   - ✅ LED se encenderá brevemente si calibración OK
   - ❌ LED parpadeará rápido si falló (repetir calibración)

> **💡 Consejo**: Mueve el robot lentamente y asegúrate de que todos los sensores pasen por la línea

---

## 🏁 Usar el Robot

### Método 1: Botón Físico (Recomendado)

1. **Posiciona** el robot en la pista (centrado en la línea)
2. **Presiona y mantén** el botón por más de 1 segundo
3. **Suelta** el botón cuando el LED parpadee rápido
4. **Cuenta regresiva automática**:
   ```
   3... (LED parpadea)
   2... (LED parpadea)
   1... (LED encendido fijo - turbina al 85%)
   ¡GO! (Robot arranca)
   ```

5. El robot seguirá la línea automáticamente
6. Se detendrá solo después de 30 segundos

### Método 2: Señal Externa (Para Competencias)

1. Conecta sistema de inicio al **Pin 6**
2. Cuando el pin reciba señal HIGH, el robot arranca inmediatamente
3. No hay cuenta regresiva en este modo

### Método 3: Comando Serial (Para Pruebas)

1. Abre el monitor serial (115200 baud)
2. Escribe: `s` y presiona Enter
3. Cuenta regresiva de 3 segundos
4. Robot arranca automáticamente

---

## 🛑 Detener el Robot

### Durante la Carrera:

- **Botón**: Presiona brevemente el botón
- **Serial**: Envía comando `x`
- **Automático**: Robot se detiene si pierde la línea por más de 250ms

---

## ⚙️ Ajustar Configuración

### Abrir Monitor Serial

**En PlatformIO**:
- Click en icono "Monitor Serial"
- O presiona `Ctrl+Alt+M`

**Velocidad**: 115200 baud

### Comandos Disponibles

#### Velocidad del Robot
```
v30   → Velocidad base 30%
v50   → Velocidad base 50%
v80   → Velocidad base 80%
```

#### Aceleración
```
a40   → Aceleración lenta
a60   → Aceleración media (recomendada)
a80   → Aceleración rápida
```

#### Turbina
```
f0    → Turbina apagada
f50   → Turbina 50%
f80   → Turbina 80% (recomendada)
f100  → Turbina máxima
```

#### Otros Comandos
```
s     → Iniciar carrera
x     → Detener carrera
r     → Ver valores RAW de sensores
c     → Ver sensores calibrados (0 o 1)
cal   → Re-calibrar sensores
```

---

## 🎯 Configuraciones Recomendadas

### Para Empezar (Pista Desconocida)
```
v30    (velocidad baja)
a60    (aceleración media)
f60    (turbina media)
```

### Pista con Curvas Suaves
```
v50    (velocidad media)
a70    (aceleración alta)
f80    (turbina alta)
```

### Pista Rápida con Pocas Curvas
```
v70    (velocidad alta)
a80    (aceleración muy alta)
f80    (turbina alta)
```

### Modo Máximo (Solo Pistas Conocidas)
```
v100   (velocidad máxima)
a90    (aceleración máxima)
f100   (turbina máxima)
```

---

## 🔧 Mantenimiento

### Antes de Cada Carrera

1. ✅ **Verificar batería cargada**
2. ✅ **Limpiar sensores** (paño seco suave)
3. ✅ **Verificar ruedas** (no hay objetos pegados)
4. ✅ **Probar turbina** (debe girar suave al encender)
5. ✅ **Calibrar sensores** si cambió la pista

### Después de Cada Carrera

1. ✅ **Desconectar batería**
2. ✅ **Limpiar robot** (quitar polvo)
3. ✅ **Revisar tornillos** (que estén ajustados)

### Limpieza de Sensores

**Frecuencia**: Cada 5 carreras o si detectas problemas

**Método**:
1. Apaga el robot
2. Usa paño de microfibra seco
3. Limpia suavemente cada sensor
4. NO uses líquidos
5. NO presiones fuerte

---

## 🐛 Solución de Problemas Comunes

### Problema: Robot no enciende

**Posibles causas**:
- ❌ Batería descargada
- ❌ Conexión suelta

**Solución**:
1. Verifica voltaje de batería (mínimo 7.4V)
2. Revisa conexiones de alimentación

---

### Problema: Robot pierde la línea constantemente

**Posibles causas**:
- ❌ Sensores sucios
- ❌ Mala calibración
- ❌ Velocidad muy alta

**Solución**:
1. Limpia sensores con paño seco
2. Re-calibra: Comando `cal`
3. Reduce velocidad: `v30`
4. Verifica en serial: Comando `r` para ver valores

---

### Problema: Turbina no funciona

**Posibles causas**:
- ❌ ESC no inicializado
- ❌ Turbina configurada en 0%

**Solución**:
1. Espera 2-3 segundos al encender (beep del ESC)
2. Configura turbina: `f80`
3. Verifica conexión del ESC

---

### Problema: Motores no responden

**Posibles causas**:
- ❌ Robot no está en modo carrera
- ❌ Conexión suelta
- ❌ Driver sobrecalentado

**Solución**:
1. Inicia carrera: Botón largo o comando `s`
2. Verifica conexiones de motores
3. Si drivers están calientes, espera 5 minutos

---

### Problema: Robot gira en círculos

**Posibles causas**:
- ❌ Mal montaje de ruedas
- ❌ Motores invertidos
- ❌ Calibración incorrecta

**Solución**:
1. Verifica que ambas ruedas giren bien
2. Re-calibra sensores
3. Prueba en pista simple primero

---

### Problema: Sensores no detectan línea

**Verificación**:
1. Comando `r` en serial
2. Valores deben cambiar al pasar sobre línea:
   - Fondo blanco: ~3000-4000
   - Línea negra: ~100-500
3. Si no cambian:
   - Sensores muy altos/bajos de la pista
   - Sensores sucios
   - Luz ambiental muy intensa

**Solución**:
1. Ajusta altura del robot (3-5mm ideal)
2. Limpia sensores
3. Evita luz solar directa
4. Re-calibra

---

## 📊 Interpretación de Sensores

### Ver Estado de Sensores

Comando `c` muestra sensores calibrados:
```
CAL: 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0
     ◄────────────────────────────►
     Izq                         Der
```

**Interpretación**:
- `1` = Sensor detecta línea (negro)
- `0` = Sensor detecta fondo (blanco)

**Ejemplos**:

```
Línea Centrada:
0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0
              ▲▲▲
           Centro perfecto
```

```
Línea a la Derecha:
0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0
                        ▲▲▲
                  Debe girar derecha
```

```
Línea a la Izquierda:
0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0
      ▲▲▲
  Debe girar izquierda
```

```
Línea Perdida:
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
          ⚠️ ALERTA ⚠️
```

---

## 💾 Guardar Configuración

El robot **NO guarda** configuraciones automáticamente.

Para guardar tus configuraciones preferidas:

### Opción 1: Modificar Código
En `main.cpp` líneas 70-72:
```cpp
set_base_speed(50);        // Cambia 50 por tu velocidad
set_base_accel_speed(70);  // Cambia 70 por tu aceleración
set_base_fan_speed(80);    // Cambia 80 por tu turbina
```

### Opción 2: Crear Script
Crea archivo `config.txt` con tus comandos:
```
v50
a70
f80
```
Y envíalos cada vez que enciendas

---

## 🔋 Batería

### Especificaciones
- **Voltaje**: 7.4V - 11.1V (2S - 3S LiPo)
- **Capacidad recomendada**: 1000-2000 mAh
- **Descarga**: 20C o superior

### Duración Estimada
- **Sin turbina**: ~15-20 minutos
- **Con turbina 50%**: ~8-12 minutos
- **Con turbina 100%**: ~5-8 minutos

### Indicadores de Batería Baja
- Robot pierde potencia en curvas
- Turbina hace ruidos extraños
- Velocidad máxima reducida

**⚠️ IMPORTANTE**:
- Nunca descargues LiPo por debajo de 3.0V por celda
- Usa alarma de voltaje o cargador con balance

---

## 🎓 Consejos de Competencia

### Antes de la Competencia

1. **Practica en pista similar**
2. **Calibra con la pista real** de competencia
3. **Prueba diferentes configuraciones**
4. **Anota tus mejores configuraciones**
5. **Carga batería completa**
6. **Lleva baterías de repuesto**

### Durante Clasificación

1. **Primera vuelta: Conservador** (v40, a50, f60)
2. **Observa el circuito** mientras el robot corre
3. **Identifica curvas difíciles**
4. **Ajusta configuración** basado en observación

### Para la Final

1. **Usa configuración probada**
2. **No cambies a último momento**
3. **Calibra 5 minutos antes de tu turno**
4. **Verifica batería cargada**

---

## 📈 Mejorando Tiempos

### Progresión Sugerida

**Semana 1**: Conocer el robot
```
v30, a50, f50
Objetivo: Completar pista sin salirse
```

**Semana 2**: Aumentar confianza
```
v50, a60, f70
Objetivo: Completar pista consistentemente
```

**Semana 3**: Velocidad
```
v70, a70, f80
Objetivo: Reducir tiempo en 30%
```

**Semana 4+**: Optimización
```
v80-100, a80-90, f80-100
Objetivo: Tiempo mínimo consistente
```

---

## ✅ Checklist Pre-Carrera

Antes de cada carrera, verifica:

- [ ] Batería cargada (>7.4V)
- [ ] Sensores limpios
- [ ] Turbina funciona (oyes el beep al encender)
- [ ] LED responde al presionar botón
- [ ] Calibración reciente (<10 minutos)
- [ ] Configuración cargada (v, a, f)
- [ ] Robot centrado en línea de inicio
- [ ] Ruedas limpias (sin objetos)
- [ ] Tornillos ajustados

---

## 🎉 ¡Listo para Competir!

Ahora que conoces tu robot EHECATL, es momento de dominarlo en la pista.

**Recuerda**:
- Empieza lento y aumenta velocidad gradualmente
- La calibración es CRÍTICA
- Cada pista es diferente
- Práctica hace al maestro

---

**¡Buena suerte en tus carreras!** 🏁💨

*EHECATL - El viento que domina la pista*
