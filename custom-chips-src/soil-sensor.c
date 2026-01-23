// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  //AOUT pin 
  pin_t aout_pin;
  uint32_t moisture_attr;
} chip_state_t;

static void chip_timer_event(void* user_data);

void chip_init() {
  //Initialize memory
  chip_state_t* chip = malloc(sizeof(chip_state_t));

  //Get output pin
  chip->aout_pin = pin_init("AOUT", ANALOG);

  //Get input slider 
  chip->moisture_attr = attr_init("moisture", 20);

  //Glue the input to chip logic 
  const timer_config_t moisture_timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };

  timer_t timer_id = timer_init(&moisture_timer_config);
  timer_start(timer_id, 1000, true);

  printf("Moisture sensor Intialized!\n");
}

static void chip_timer_event(void* user_data) {
  chip_state_t* chip = user_data;
  uint32_t moisture_value = attr_read(chip->moisture_attr);
  float voltage = (((float) moisture_value / 100.0) * 3.0);  
  pin_dac_write(chip->aout_pin, voltage);
  printf("Value from inside the sensor, %f\n", voltage);
}