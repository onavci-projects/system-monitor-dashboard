#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

#define SECTOR_SIZE 512

void get_disk_stats(DiskStats *disk) {
    FILE *fp;
    char line[256];
    char dev_name[32];

    unsigned long long rd_sectors = 0;
    unsigned long long wr_sectors = 0;

    unsigned long long curr_read_bytes;
    unsigned long long curr_write_bytes;

    const char *target_disk = "sda";

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL) {
        perror("/proc/diskstats dosyasi acilamadi");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp)) {
        int matches = sscanf(
            line,
            "%*d %*d %s %*u %*u %llu %*u %*u %*u %llu",
            dev_name,
            &rd_sectors,
            &wr_sectors
        );

        if (matches >= 1 && strcmp(dev_name, target_disk) == 0) {

            curr_read_bytes  = rd_sectors * SECTOR_SIZE;
            curr_write_bytes = wr_sectors * SECTOR_SIZE;

            /* First-run koruması:
               Önceki değerler 0 ise hız hesaplama */
            if (disk->read_bytes > 0 || disk->write_bytes > 0) {
                disk->read_rate =
                    (curr_read_bytes - disk->read_bytes) / 1024.0;

                disk->write_rate =
                    (curr_write_bytes - disk->write_bytes) / 1024.0;
            } else {
                disk->read_rate = 0.0;
                disk->write_rate = 0.0;
            }

            /* State update:
               Bir sonraki çağrı için mevcut değerleri sakla */
            disk->read_bytes  = curr_read_bytes;
            disk->write_bytes = curr_write_bytes;

            break;
        }
    }

    fclose(fp);
}
