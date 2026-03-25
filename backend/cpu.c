#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

void get_cpu_usage(CpuUsage *cpu)
{
    FILE *fp;
    char line[128];

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    unsigned long long prev_idle_sum, curr_idle_sum;
    unsigned long long prev_total_sum, curr_total_sum;
    unsigned long long total_diff, idle_diff;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("CPU bilgisi okunamadi");
        exit(1);
    }

    // İlk satır: toplam CPU
    if (fgets(line, sizeof(line), fp) == NULL)
    {
        perror("CPU verisi okunamadi");
        fclose(fp);
        exit(1);
    }

    sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    prev_idle_sum = cpu->idle + cpu->iowait;
    prev_total_sum = cpu->user + cpu->nice + cpu->system +
                     cpu->idle + cpu->iowait + cpu->irq +
                     cpu->softirq + cpu->steal;

    curr_idle_sum = idle + iowait;
    curr_total_sum = user + nice + system + idle + iowait + irq + softirq + steal;

    total_diff = curr_total_sum - prev_total_sum;
    idle_diff = curr_idle_sum - prev_idle_sum;

    if (total_diff == 0)
    {
        cpu->usage_percentage = 0.0;
    }
    else
    {
        cpu->usage_percentage = (double)(total_diff - idle_diff) / total_diff * 100.0;
    }

    cpu->user = user;
    cpu->nice = nice;
    cpu->system = system;
    cpu->idle = idle;
    cpu->iowait = iowait;
    cpu->irq = irq;
    cpu->softirq = softirq;
    cpu->steal = steal;

    // ŞİMDİ: Her çekirdeği oku
    cpu->core_count = 0;
    int core_index = 0;

    while (fgets(line, sizeof(line), fp) != NULL && core_index < MAX_CPU_CORES)
    {
        // "cpuX" ile başlayan satırları bul
        if (strncmp(line, "cpu", 3) == 0 && line[3] >= '0' && line[3] <= '9')
        {
            sscanf(line, "cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu",
                   &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

            // Önceki değerler
            prev_idle_sum = cpu->cores[core_index].idle + cpu->cores[core_index].iowait;
            prev_total_sum = cpu->cores[core_index].user + cpu->cores[core_index].nice +
                             cpu->cores[core_index].system + cpu->cores[core_index].idle +
                             cpu->cores[core_index].iowait + cpu->cores[core_index].irq +
                             cpu->cores[core_index].softirq + cpu->cores[core_index].steal;

            // Şimdiki değerler
            curr_idle_sum = idle + iowait;
            curr_total_sum = user + nice + system + idle + iowait + irq + softirq + steal;

            total_diff = curr_total_sum - prev_total_sum;
            idle_diff = curr_idle_sum - prev_idle_sum;

            if (total_diff == 0)
            {
                cpu->cores[core_index].usage_percentage = 0.0;
            }
            else
            {
                cpu->cores[core_index].usage_percentage =
                    (double)(total_diff - idle_diff) / total_diff * 100.0;
            }

            // State update
            cpu->cores[core_index].user = user;
            cpu->cores[core_index].nice = nice;
            cpu->cores[core_index].system = system;
            cpu->cores[core_index].idle = idle;
            cpu->cores[core_index].iowait = iowait;
            cpu->cores[core_index].irq = irq;
            cpu->cores[core_index].softirq = softirq;
            cpu->cores[core_index].steal = steal;

            core_index++;
        }
    }

    cpu->core_count = core_index;
    fclose(fp);
}