#include <bateria.h>

#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// Esse código foi altamente inspirado no código de sample do zephyr

LOG_MODULE_REGISTER(Bateria);

const double calibration_factor = 1;

const struct gpio_dt_spec *transistor =
    &(const struct gpio_dt_spec)GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), bateria_gpios);
const struct adc_dt_spec bateria_adc_channel =
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);
static uint16_t buf;
static struct adc_sequence sequence = {
    .buffer = &buf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(buf),
};

int bateria_init() {
  LOG_INF("Inicializando...");

  int err;
  err = adc_is_ready_dt(&bateria_adc_channel);
  if (err < 0) {
    LOG_ERR("ADC não está pronto");
    return err;
  }

  err = device_is_ready(transistor->port);
  if (err < 0) {
    LOG_ERR("A porta do pino do transistor não está pronta");
    return err;
  }

  err = gpio_pin_configure_dt(transistor, GPIO_OUTPUT);
  if (err < 0) {
    LOG_ERR("Não foi possível configurar o pino do transistor como output (%d)",
            err);
    return err;
  }

  return 0;
}

uint32_t bateria_read() {
  int err;

  // Vamos habilitar o circuito ativando o transistor
  LOG_INF("Ativando o circuito de leitura da bateria");

  err = gpio_pin_set_dt(transistor, 1);
  if (err < 0) {
    LOG_ERR("Não foi possível setar o pino do transistor para 1 (%d)", err);
    return err;
  }

  // Vamos tentar ler direto com um delay de 1ms
  k_msleep(10);

  LOG_INF("Lendo...");

  int32_t val_mv;

  (void)adc_sequence_init_dt(&bateria_adc_channel, &sequence);

  err = adc_read_dt(&bateria_adc_channel, &sequence);
  if (err < 0) {
    LOG_ERR("Não foi possível ler (%d)", err);
    return err;
  }

  if (bateria_adc_channel.channel_cfg.differential) {
    val_mv = (int32_t)((int16_t)buf);
  } else {
    val_mv = (int32_t)buf;
  }

  /* Converte o valor em milivolts*/
  err = adc_raw_to_millivolts_dt(&bateria_adc_channel, &val_mv);
  if (err < 0) {
    LOG_ERR("Não foi possível converter para mV (%d)", err);
    return err;
  }

  // E agora desativamos o transistor
  err = gpio_pin_set_dt(transistor, 0);
  if (err < 0) {
    LOG_ERR("Não foi possível setar o pino do transistor para 0 (%d)", err);
    return err;
  }

  return val_mv*calibration_factor;
}
