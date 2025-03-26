#ifndef GPS_H
#define GPS_H

#include <zephyr/logging/log.h>


extern struct s_tpms {
  const struct device *dev;
  char *buffer;
  int buff_len;
  bool ready;

  int pressao[4] = {0};         
  int temperatura[4] = {0};          
  char information_status; // A: status OK
                           // V: status NOT ok

} tpms;


/**
 * @brief 
 * 
 * @param buf ponteiro para a mensagem que será enviada
 * @param len tamanho da mensagem que será enviada
 */
static void send_uart_gps(unsigned char* buf, int len);

/**
 * @brief desabilita todas as linhas do GPS, uma de cada vez
 * 
 * @param mod representa qual linha será desativada
 */
static void disable_type(int mod);

/**
 * @brief habilita a linha de um determinado tipo
 * 
 * @param tp tipo que será habilitado
 */
static void enable_type(enum TYPE tp);

/**
 * @brief Inicializa o GPS
 * 
 * @return int que representa algum erro no callback
 */
int init_gps();

/**
 * @brief executa o get_location, dessa forma o get_location não
 * segura o restante do código
 */
void gps_tick();

#endif // GPS_H