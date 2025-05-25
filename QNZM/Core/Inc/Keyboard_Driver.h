/*
 * Keyboard_Driver.h
 *
 *  Created on: Mar 10, 2025
 *      Author: 28584
 */

#ifndef INC_KEYBOARD_DRIVER_H_
#define INC_KEYBOARD_DRIVER_H_

#include "main.h"
#include "usb_device.h"
#include "usbd_customhid.h"



void ifkey(int command);
void HIDSend(int command);

#endif /* INC_KEYBOARD_DRIVER_H_ */
