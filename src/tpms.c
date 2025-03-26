#include <tpms.h>

// Código altamente copiado do GPS (sim, estou desesperado)

static void read_line()
{
    char *buf = malloc(tpms.buff_len * sizeof(char));

    // Obter somente uma linha do buffer
    sscanf(tpms.buffer, "%[^\n]s", buf);

    // Procuramos o início da mensagem (byte = 0x55)
    while (buf[0] != 0x55)
    {
        // Se não tivermos encontrado o início, incrementamos o ponteiro
        buf = buf + 1;
        if (strlen(buf) < 6)
        {
            LOG_ERR("Não foi encontrado o início da mensagem 0x55");
            return;
        }
    }
    // Formato das mensagens do TPMS:
    // 55 aa 08   01  33      50            00 95
    //   Start    ID  Pressão  Temperatura  Lixo
    //  Status: A=Valid Data
    //          V=Invalid Data
    char status;
    int id = 0;

    id = buf[3];

    if (strlen(buf) < 8) // Não tem informação suficiente
        return 0;

    

    free(gps.latitude);
    gps.latitude = lat_str;

    free(gps.longitude);
    gps.longitude = long_str;

    gps.information_status = status;

    LOG_INF("Latitude: %s | Longitude: %s | Status: %c", lat_str, long_str,
            status);
}

struct s_tpms tpms;

static void tpms_callback(const struct device *dev, struct uart_event *evt,
                          void *user_data)
{

    switch (evt->type)
    {
    case UART_RX_RDY:
        tpms.ready = true;
        uart_rx_disable(gps.dev); // Não precisamos receber novas informações até
                                  // terminarmos de lidar com as que já temos
        break;
    }
}