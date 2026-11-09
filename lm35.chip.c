// Chip personalizado LM35 para Wokwi
// Salida analógica: Vout = 10 mV/°C x T (0 °C -> 0 V, 25 °C -> 0,25 V, 100 °C -> 1,00 V)

#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t    pin_out;
  uint32_t temperature_attr;
} chip_state_t;

static void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;
  float temperatura = attr_read_float(chip->temperature_attr);
  pin_dac_write(chip->pin_out, temperatura * 0.01);   // 10 mV/°C
}

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_out          = pin_init("OUT", ANALOG);
  chip->temperature_attr = attr_init_float("temperature", 25.0);

  const timer_config_t timer_config = {
    .callback  = chip_timer_event,
    .user_data = chip,
  };
  timer_t timer_id = timer_init(&timer_config);
  timer_start(timer_id, 1000, true);   // actualiza la salida cada 1 ms
}
