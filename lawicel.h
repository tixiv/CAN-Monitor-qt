/*
 * lawicel.h
 *
 *  Created on: 01.06.2018
 *      Author: tixiv
 */

#ifndef LAWICEL_H_
#define LAWICEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <can_message.h>

typedef enum{
  can_mode_close,
  can_mode_normal,
  can_mode_listen,
  can_mode_loopback,
}lawicel_can_mode_e;

typedef bool can_init_func_t(lawicel_can_mode_e mode, uint8_t baud);
typedef bool can_transmit_func_t(const can_message_t *);
typedef void uart_putstr_func_t(const char *);
typedef can_message_t *can_buffer_get_func_t(void);
typedef uint8_t can_get_status_func_t(void);

typedef struct{
  can_init_func_t * can_init_callback;
  uart_putstr_func_t * uart_putstr_callback;
  can_buffer_get_func_t * can_buffer_get_callback;
  can_transmit_func_t * can_transmit_callback;
  can_get_status_func_t * can_get_status_callback;

  uint8_t terminal_open:1;
  uint8_t transmit_enabled:1;
  uint8_t baudrate_configured:1;
  uint8_t baud;
}lawicel_t;

// for microcontroller with lawicel_t callback struct
void lawicel_execute_command(lawicel_t * lp, const char * str);
void lawicel_handle_can_message(lawicel_t * lp, can_message_t *cmsg);

// for using on PC
bool lawicel_can_message_from_string(can_message_t * cmsg, const char * str);
void lawicel_string_from_can_message(char * str, can_message_t *cmsg);
const char * lawicel_get_baud_string(int bps);

#ifdef __cplusplus
}
#endif
#endif /* LAWICEL_H_ */
