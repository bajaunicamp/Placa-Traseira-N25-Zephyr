#ifndef CAN_H
#define CAN_H

#include "zephyr/drivers/gpio.h"
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/*
 * É necessário rodar Init_CAN() e em um loop while rodar CAN_Tick()
 */

int Init_CAN();

int send_can_TPMS(int pressao_de, int pressao_dd, int pressao_te, int pressao_td);
int send_can_Bateria();
int send_can_Temperatura();

#endif // !CAN_H
