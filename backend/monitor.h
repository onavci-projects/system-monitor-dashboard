#ifndef MONITOR_H
#define MONITOR_H

#define MAX_CPU_CORES 16
#define HISTORY_SIZE 60

// CPU Core bilgisi
typedef struct
{
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    double usage_percentage;
} CpuCore;

// CPU bilgisi
typedef struct
{
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    double usage_percentage;

    // Çekirdek bazında
    int core_count;
    CpuCore cores[MAX_CPU_CORES];
} CpuUsage;

// Bellek bilgisi
typedef struct
{
    long total_ram;
    long free_ram;
    long used_ram;
    double percent;
} MemoryInfo;

// Disk istatistikleri
typedef struct
{
    unsigned long long read_bytes;
    unsigned long long write_bytes;
    double read_rate;
    double write_rate;
} DiskStats;

// Ağ istatistikleri
typedef struct
{
    unsigned long long rx_bytes;
    unsigned long long tx_bytes;
    double rx_rate;
    double tx_rate;
} NetworkStats;

// Fonksiyon prototipleri
MemoryInfo get_memory_info();
void get_cpu_usage(CpuUsage *cpu);
void get_disk_stats(DiskStats *disk);
void get_network_stats(NetworkStats *net);

#endif