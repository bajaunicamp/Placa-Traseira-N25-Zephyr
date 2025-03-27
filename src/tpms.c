#include "stdlib.h"
#include "zephyr/drivers/uart.h"
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include <stdio.h>
#include <string.h>
#include <tpms.h>

// Código altamente copiado do GPS (sim, estou desesperado)

LOG_MODULE_DECLARE(TPMS);

struct s_tpms tpms;

static int read_line() {
  char *buf = malloc(tpms.buff_len * sizeof(char));

  // Obter somente uma linha do buffer
  sscanf(tpms.buffer, "%[^\n]s", buf);

  // Procuramos o início da mensagem (byte = 0x55)
  while (buf[0] != 0x55) {
    // Se não tivermos encontrado o início, incrementamos o ponteiro
    buf = buf + 1;
    if (strlen(buf) < 6) {
      LOG_ERR("Não foi encontrado o início da mensagem 0x55");
      return -1;
    }
  }
  // Formato das mensagens do TPMS:
  // 55 aa 08   01  33      50            00 95
  //   Start    ID  Pressão  Temperatura  Lixo
  //  Status: A=Valid Data
  //          V=Invalid Data
  int id = 0;

  id = buf[3];

  switch (id) {
  case 0x00:
    id = 0;
    break;
  case 0x01:
    id = 1;
    break;
  case 0x10:
    id = 2;
    break;
  case 0x11:
    id = 3;
    break;
  default:
    break;
  }

  if (strlen(buf) < 8) { // Não tem informação suficiente
    tpms.information_status = 'A';
    return -1;
  }

  tpms.pressao[id] = buf[4] / 2;
  tpms.temperatura[id] = (buf[5] - 21) / 1.8;
  tpms.information_status = 'V';

  LOG_INF("ID: %d | PRESSAO: %d | TEMPERATURA: %d", id, tpms.pressao[id],
          tpms.temperatura[id]);
  return 0;
}

static void tpms_callback(const struct device *dev, struct uart_event *evt,
                          void *user_data) {

  switch (evt->type) {
  case UART_RX_RDY:
    tpms.ready = true;
    uart_rx_disable(tpms.dev); // Não precisamos receber novas informações até
                               // terminarmos de lidar com as que já temos
    break;
  }
}

int init_tpms() {
  k_msleep(1000);
  LOG_INF("Inicializando TPMS");
  tpms.dev = DEVICE_DT_GET(DT_ALIAS(tpms));
  tpms.buff_len = 20;
  tpms.buffer = malloc(tpms.buff_len * sizeof(char));
  tpms.ready = false;

  int ret = uart_rx_enable(tpms.dev, tpms.buffer, tpms.buff_len, 1000);
  if (ret) {
    LOG_ERR("ERRO UART RX Enable TPMS: %d", ret);
    return ret;
  } else {
    LOG_INF("UART Callback definido");
  }

  ret = uart_callback_set(tpms.dev, tpms_callback, NULL);
  if (ret) {
    LOG_ERR("ERRO UART RX Callback TPMS: %d", ret);
    return ret;
  }
}

// Essa função deve ser executada no while(true) da main
void tpms_tick() {
  if (tpms.ready) {
    read_line();
    tpms.ready = false;
    memset(tpms.buffer, 0, tpms.buff_len);
    uart_rx_enable(tpms.dev, tpms.buffer, tpms.buff_len, 1000);
  }
}
