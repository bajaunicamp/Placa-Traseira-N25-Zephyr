#include <tpms.h>
#include <bateria.h>
#include "ds18b20.h"
#include <zephyr/kernel.h>
#include "can.h"
#include "zephyr/logging/log.h"
#include <float.h>

LOG_MODULE_REGISTER();

int main(){
  float temperatura;
  init_tpms();
  bateria_init();
  int bateria;

  while (true) {

    // Lendo os dados
    tpms_tick();
    bateria = bateria_read();
    temperatura = ds18b20_get_temperature();
    if(temperatura == FLT_MIN){
      LOG_ERR("Erro ao ler a temperatura do sensor");
      temperatura = 0;
    }

    //Enviando os dados CAN
    // Envaindo TPMS
  }
  
}
