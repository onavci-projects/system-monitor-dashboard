#include <stdio.h>
#include <unistd.h>
#include "monitor.h"

int main()
{
    MemoryInfo mem;
    CpuUsage cpu = {0};
    DiskStats disk = {0};
    NetworkStats net = {0};

    FILE *fp;

    while (1)
    {
        mem = get_memory_info();
        get_cpu_usage(&cpu);
        get_disk_stats(&disk);
        get_network_stats(&net);

        // JSON dosyasýna yaz
        fp = fopen("./tmp/system_monitor.json", "w");
        if (fp)
        {
            fprintf(fp, "{\n");
            fprintf(fp, "  \"cpu\": %.2f,\n", cpu.usage_percentage);
            fprintf(fp, "  \"memory\": %.2f,\n", mem.percent);
            fprintf(fp, "  \"memory_used\": %ld,\n", mem.used_ram);
            fprintf(fp, "  \"memory_total\": %ld,\n", mem.total_ram);
            fprintf(fp, "  \"disk_read\": %.2f,\n", disk.read_rate);
            fprintf(fp, "  \"disk_write\": %.2f,\n", disk.write_rate);
            fprintf(fp, "  \"net_download\": %.2f,\n", net.rx_rate);
            fprintf(fp, "  \"net_upload\": %.2f,\n", net.tx_rate);
            fprintf(fp, "  \"cores\": [");
            for (int i = 0; i < cpu.core_count; i++)
            {
                fprintf(fp, "%.2f%s", cpu.cores[i].usage_percentage,
                        (i < cpu.core_count - 1) ? "," : "");
            }
            fprintf(fp, "]\n}\n");
            fclose(fp);
        }

        sleep(1);
    }
    return 0;
}
