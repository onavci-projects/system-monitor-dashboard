#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

MemoryInfo get_memory_info() {
    MemoryInfo mem;
    FILE *fp;
    char line[256];
    
    // Değişkenleri güvenli başlangıç değerlerine (initialization) sıfırlayalım
    mem.total_ram = 0;
    mem.free_ram = 0;
    mem.used_ram = 0;
    mem.percent = 0.0;

    int found_total = 0;
    int found_available = 0;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("/proc/meminfo dosyasi acilamadi");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        
        // Doğrudan struct içine okuyoruz
        if (sscanf(line, "MemTotal: %ld kB", &mem.total_ram) == 1) {
            found_total = 1;
        }
        
        // MemAvailable değerini doğrudan free_ram alanına yazıyoruz
        if (sscanf(line, "MemAvailable: %ld kB", &mem.free_ram) == 1) {
            found_available = 1;
        }

        if (found_total && found_available) {
            break;
        }
    }

    fclose(fp);

    // Hesaplamalar
    mem.used_ram = mem.total_ram - mem.free_ram;

    if (mem.total_ram > 0) {
        mem.percent = ((double)mem.used_ram / mem.total_ram) * 100.0;
    }

    return mem;
}
