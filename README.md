# Termostato con alarma en Arduino UNO (LM35 + potenciómetro + botón + LED) — Wokwi

Simulación en [Wokwi](https://wokwi.com/projects/480234282317076481) de un termostato con alarma hecho con Arduino UNO, para la asignatura **Sensórica para Ingenierías** del programa de Ingeniería Electrónica de la **Universidad Surcolombiana** (Neiva, Huila).

▶️ **Simulación en línea:** https://wokwi.com/projects/480234282317076481

## Qué hace

| Componente | Pin | Función |
|---|---|---|
| LM35 | A1 | Mide la temperatura (10 mV/°C) |
| Potenciómetro 10 kΩ | A0 | Fija el umbral de alarma entre 0 y 100 °C |
| Pulsador | D2 (`INPUT_PULLUP`) | Activa o desactiva la alarma |
| LED rojo + 220 Ω | D13 | Se enciende si la alarma está activa y T > umbral |

El monitor serial (9600 baudios) muestra cada 0,5 s la temperatura, el umbral y el estado de la alarma:

```
T = 24.9 C | Umbral = 30.0 C | Alarma: ON | LED: apagado
```

## Cálculos

- **Del ADC a voltaje:** `V = ADC × 5 / 1023` (ADC de 10 bits, referencia de 5 V).
- **De voltaje a temperatura:** el LM35 entrega 10 mV/°C, así que `T = V / 0,01 = V × 100`.
- **Resolución:** 5 V / 1023 ≈ 4,89 mV por paso, o sea **≈ 0,49 °C por paso**. Por eso el LM35 en 25 °C se lee como 24,9 °C (ADC = 51).
- **Mejora para hardware real:** con `analogReference(INTERNAL)` (1,1 V) la resolución baja a ≈ 0,11 °C por paso, con un rango máximo de ~110 °C.
- **Botón:** la resistencia pull-up interna mantiene el pin en HIGH; al presionar se va a GND (LOW). Antirrebote por software de 50 ms.

## El LM35 en Wokwi

Wokwi no incluye el LM35 entre sus piezas oficiales, así que se modeló como **chip personalizado** con la API de Custom Chips en C (`lm35.chip.c` + `lm35.chip.json`). El chip entrega `Vout = 0,01 × T` en su pin OUT y tiene un deslizador de 0 a 100 °C para cambiar la temperatura durante la simulación.

## Archivos

- `sketch.ino` — programa del Arduino.
- `diagram.json` — circuito (piezas y conexiones).
- `lm35.chip.json` — pines y control de temperatura del LM35.
- `lm35.chip.c` — modelo del LM35 (10 mV/°C).

## Cómo probarlo

1. Abre el [proyecto en Wokwi](https://wokwi.com/projects/480234282317076481) y dale ▶.
2. Haz clic en el LM35 y mueve la temperatura.
3. Gira el potenciómetro para cambiar el umbral.
4. Presiona el botón verde para activar o desactivar la alarma.

---

**Autor:** Sebastián Serrato Ramírez — Ingeniería Electrónica, Universidad Surcolombiana · [LinkedIn](https://www.linkedin.com/in/sebaserrato)
