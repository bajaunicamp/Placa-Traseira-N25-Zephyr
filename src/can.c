#include "can.h"

LOG_MODULE_REGISTER(can);

struct can_frame temperatura_frame = {
  .flags = 0, .id = 0x03, .dlc = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8}};
struct can_frame bateria_frame = {
    .flags = 0, .id = 0x04, .dlc = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8}};


const struct device *const can_dev = DEVICE_DT_GET(DT_ALIAS(mycan));

struct can_timing timing;

struct can_frame tpmsd_frame = {
  .flags = 0, .id = 0x7, .dlc = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8}};

struct can_frame tpmst_frame = {
  .flags = 0, .id = 0x8, .dlc = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8}};
int send_can_TPMS(int pressao_de, int pressao_dd, int pressao_te, int pressao_td){

  int ret;

  tpmsd_frame.data_32[0] = pressao_de;
  tpmsd_frame.data_32[1] = pressao_dd;

  LOG_INF("Enviando Pressão DE = %d, Pressão DD = %d", tpmsd_frame.data_32[0], tpmsd_frame.data_32[1]);
  ret = can_send(can_dev, &tpmsd_frame, K_MSEC(100), NULL, NULL);

  tpmst_frame.data_32[0] = pressao_te;
  tpmst_frame.data_32[1] = pressao_td;

  LOG_INF("Enviando Pressão TE = %d, Pressão TD = %d", tpmst_frame.data_32[0], tpmst_frame.data_32[1]);
  ret = can_send(can_dev, &tpmst_frame, K_MSEC(100), NULL, NULL);

  return 0;
};

//TODO: parei aqui
int send_can_Bateria();
int send_can_Temperatura();

int Init_CAN()
{
  int ret;
  LOG_INF("Inicializando");

  ret = can_set_mode(can_dev, CAN_MODE_NORMAL);
  if (ret != 0)
  {
    printf("Error setting CAN mode [%d]", ret);
    return 0;
  }

  ret = can_set_bitrate(can_dev, 125000);
  if (ret != 0)
  {
    LOG_ERR("Failed to set timing (%d)", ret);
  }

  ret = can_start(can_dev);
  if (ret != 0)
  {
    printf("Error starting CAN controller [%d]", ret);
    return 0;
  }

  return 0;
}
