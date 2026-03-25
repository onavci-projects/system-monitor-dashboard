#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

void get_network_stats(NetworkStats *net) {
    FILE *fp;
    char line[256];
    char iface_name[32];
    
    unsigned long long curr_rx = 0;
    unsigned long long curr_tx = 0;

    // eth0 pc ye göre değişilik gösterebilir(ip a)
    const char *target_iface = "enp0s3"; 

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL) {
        perror("/proc/net/dev dosyasi acilamadi");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        // /proc/net/dev formatında arayüz isminden sonra ':' gelir.
        // sscanf'in kafası karışmasın diye ':' işaretini boşlukla değiştiriyoruz.
        char *search_pos = strchr(line, ':');
        if (search_pos != NULL) {
            *search_pos = ' '; 
        }

        // Format: Interface Receive_Bytes ... ... ... Transmit_Bytes ...
        // 1. sütun: isim, 2. sütun: RX, 10. sütun: TX
        int matches = sscanf(line, "%s %llu %*u %*u %*u %*u %*u %*u %*u %llu",
                             iface_name,
                             &curr_rx,
                             &curr_tx);

        if (matches >= 1 && strcmp(iface_name, target_iface) == 0) {
            
            // First-run koruması (İlk çalışmada hız hesaplama)
            if (net->rx_bytes > 0 || net->tx_bytes > 0) {
                net->rx_rate = (curr_rx - net->rx_bytes) / 1024.0;
                net->tx_rate = (curr_tx - net->tx_bytes) / 1024.0;
            } else {
                net->rx_rate = 0.0;
                net->tx_rate = 0.0;
            }

            // State Update (Durum güncelleme)
            net->rx_bytes = curr_rx;
            net->tx_bytes = curr_tx;
            
            break; // Bulduk, çık.
        }
    }
    fclose(fp);
}
