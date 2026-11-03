#ifndef CV_Model_H
#define CV_Model_H

#include <Arduino.h>

/* Function definitions ------------------------------------------------------- */
bool ei_camera_init(void);
void ei_camera_deinit(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
bool runModelInference();
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr);


#endif