/*
 * lawicel.c
 *
 *  Created on: 01.06.2018
 *      Author: tixiv
 */

#include "lawicel.h"


static uint8_t read_hex_number_8(const char * str, uint8_t len){
  uint8_t d = 0;
  while(len--){
    d <<= 4;
    uint8_t v = 0;
    if((*str >= '0') && (*str <= '9')) v = *str - '0';
    else if((*str >= 'A') && (*str <= 'F')) v = *str - 'A' + 10;
    else if((*str >= 'a') && (*str <= 'f')) v = *str - 'a' + 10;

    d |= v;
    str++;
  }
  return d;
}

static uint32_t read_hex_number(const char * str, uint8_t len){
  uint32_t d = 0;
  while(len--){
    d <<= 4;
    uint8_t v = 0;
    if((*str >= '0') && (*str <= '9')) v = *str - '0';
    else if((*str >= 'A') && (*str <= 'F')) v = *str - 'A' + 10;
    else if((*str >= 'a') && (*str <= 'f')) v = *str - 'a' + 10;

    d |= v;
    str++;
  }
  return d;
}

static bool send_can_message_from_string(lawicel_t * lp, const char * str){
  char cmd = *str++; // command char
  bool IDE = cmd == 'T' || cmd == 'R'; // upercase means EID
  bool RTR = cmd == 'r' || cmd == 'R'; // the upper or lowercase r means RTR

  uint32_t id;
  uint8_t dlc;

  if(!IDE){ //standard identifier
    id = read_hex_number(str, 3);
    str += 3;
    if(id > 0x7ff) return false;
  }else{
    id = read_hex_number(str, 8);
    str += 8;
    if(id > 0x1fffffff) return false;
  }

  dlc = read_hex_number_8(str, 1);
  str += 1;
  if(dlc > 8)
    return false;

  can_message_t * cmsg = lp->can_buffer_get_callback();
  cmsg->RTR = RTR;
  cmsg->IDE = IDE;
  cmsg->dlc = dlc;
  cmsg->id = id;

  if(!RTR){
    for(uint8_t i = 0; i < dlc; i++){
      cmsg->data[i] = read_hex_number_8(str, 2);
      str += 2;
    }
  }
  lp->can_transmit_callback(cmsg);
  return true;
}

bool lawicel_can_message_from_string(can_message_t * cmsg, const char * str){
  char cmd = *str++; // command char
  if(cmd != 't' && cmd != 'T' && cmd != 'r' && cmd != 'R')
      return false;

  bool IDE = cmd == 'T' || cmd == 'R'; // upercase means EID
  bool RTR = cmd == 'r' || cmd == 'R'; // the upper or lowercase r means RTR

  uint32_t id;
  uint8_t dlc;

  if(!IDE){ //standard identifier
    id = read_hex_number(str, 3);
    str += 3;
    if(id > 0x7ff) return false;
  }else{
    id = read_hex_number(str, 8);
    str += 8;
    if(id > 0x1fffffff) return false;
  }

  dlc = read_hex_number_8(str, 1);
  str += 1;
  if(dlc > 8)
    return false;

  cmsg->RTR = RTR;
  cmsg->IDE = IDE;
  cmsg->dlc = dlc;
  cmsg->id = id;

  if(!RTR){
    for(uint8_t i = 0; i < dlc; i++){
      cmsg->data[i] = read_hex_number_8(str, 2);
      str += 2;
    }
  }
  return true;
}


static char * put_hex_byte(char * str, uint8_t val);

static void execute_status_command(lawicel_t * lp){
  uint8_t s = lp->can_get_status_callback();

  char buf[5];
  char * str = buf;

  *str++ = 'F';
  str = put_hex_byte(str, s);
  *str++ = '\r';
  *str++ = 0;

  lp->uart_putstr_callback(buf);
}

void lawicel_execute_command(lawicel_t * lp, const char * str){
  switch(str[0]){
  case 't': // transmit standard ID messages
  case 'r':
    if(lp->transmit_enabled && send_can_message_from_string(lp, str)){
      lp->uart_putstr_callback("z\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'T': // transmit extended ID messages
  case 'R':
    if(lp->transmit_enabled && send_can_message_from_string(lp, str)){
      lp->uart_putstr_callback("Z\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'O': //open terminal
    if(lp->baudrate_configured && !lp->terminal_open
        && lp->can_init_callback(can_mode_normal, lp->baud)
    ){
      lp->terminal_open = 1;
      lp->transmit_enabled = 1;
      lp->uart_putstr_callback("\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'L': // open terminal listen only
    if(lp->baudrate_configured && !lp->terminal_open
        && lp->can_init_callback(can_mode_listen, lp->baud)
    ){
      lp->terminal_open = 1;
      lp->uart_putstr_callback("\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'l': // (lower case L) open terminal in loopback mode (non standard feature)
    if(lp->baudrate_configured && !lp->terminal_open
        && lp->can_init_callback(can_mode_loopback, lp->baud)
    ){
      lp->terminal_open = 1;
      lp->transmit_enabled = 1;
      lp->uart_putstr_callback("\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'C': //close terminal
    if(lp->terminal_open){
      lp->terminal_open = 0;
      lp->transmit_enabled = 0;
      lp->can_init_callback(can_mode_close, 0);
      lp->uart_putstr_callback("\r");
    }else{
      lp->uart_putstr_callback("\a"); // bell
    }
    break;
  case 'F': // request status
    execute_status_command(lp);
    break;
  case 'S': //select bitrate e.g. "S4" for 125kbit
    if(lp->terminal_open || str[1] < '0' || str[1] == '7' || str[1] > '8') {
      // 800 kBaud is not supported, and any other character is also error
      // or terminal is open, we cannot set a baud rate
      lp->uart_putstr_callback("\a"); // bell
    } else {
      lp->baudrate_configured = 1;
      lp->baud = str[1] - '0';
      lp->uart_putstr_callback("\r");
    }
    break;
  case'V'://version
    lp->uart_putstr_callback("V1010\r");
    break;
  case'N'://serial number
    lp->uart_putstr_callback("N1234\r");
    break;
  }
}

// puts one ascii hex digit (0-9, A-F) to a string for val in range
// 0-15, returns a pointer to the next position in the string
static char * put_hex_digit(char * str, uint8_t val){
  char c;
  val &= 0x0f;
  if(val < 10){
    c = val + '0';
  }else{
    c = val + 'A' - 10;
  }
  *str++ = c;
  return str;
}

// puts a byte as ascii hex to a string and returns a pointer
// to the nex position in the string.
static char * put_hex_byte(char * str, uint8_t val){
  str = put_hex_digit(str, val >> 4);
  str = put_hex_digit(str, val);
  return str;
}

void lawicel_handle_can_message(lawicel_t * lp, can_message_t *cmsg){
  //       cmd  id  dlc data   \r  0
  char buf[ 1  + 8 + 1 + 8*2 + 1 + 1];
  char * str = buf;

  if(lp->terminal_open){
    if(!cmsg->IDE){ // standard identifier
      *str++ = cmsg->RTR ? 'r':'t';
      str = put_hex_digit(str, cmsg->id >> 8);
      str = put_hex_byte(str, cmsg->id & 0xff);
    }else{ // extended identifier
      *str++ = cmsg->RTR ? 'R':'T';
      str = put_hex_byte(str, (cmsg->id >> 24) & 0xff);
      str = put_hex_byte(str, (cmsg->id >> 16) & 0xff);
      str = put_hex_byte(str, (cmsg->id >>  8) & 0xff);
      str = put_hex_byte(str, (cmsg->id >>  0) & 0xff);
    }

    str = put_hex_digit(str, cmsg->dlc);

    if(!cmsg->RTR){ // no data on RTR
      for(uint8_t i = 0; i < cmsg->dlc; i++){
        str = put_hex_byte(str, cmsg->data[i]);
      }
    }
    *str++ = '\r';
    *str++ = 0;

    lp->uart_putstr_callback(buf);
  }
}

void lawicel_string_from_can_message(char * str, can_message_t *cmsg){
    //       cmd  id  dlc data   \r  0
    //char buf[ 1  + 8 + 1 + 8*2 + 1 + 1];
    //char * str = buf;

    if(!cmsg->IDE){ // standard identifier
      *str++ = cmsg->RTR ? 'r':'t';
      str = put_hex_digit(str, cmsg->id >> 8);
      str = put_hex_byte(str, cmsg->id & 0xff);
    }else{ // extended identifier
      *str++ = cmsg->RTR ? 'R':'T';
      str = put_hex_byte(str, (cmsg->id >> 24) & 0xff);
      str = put_hex_byte(str, (cmsg->id >> 16) & 0xff);
      str = put_hex_byte(str, (cmsg->id >>  8) & 0xff);
      str = put_hex_byte(str, (cmsg->id >>  0) & 0xff);
    }

    str = put_hex_digit(str, cmsg->dlc);

    if(!cmsg->RTR){ // no data on RTR
      for(uint8_t i = 0; i < cmsg->dlc; i++){
        str = put_hex_byte(str, cmsg->data[i]);
      }
    }
    *str++ = '\r';
    *str++ = 0;
}
