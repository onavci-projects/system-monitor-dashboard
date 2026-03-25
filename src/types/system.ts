export interface CpuCore {
  usage_percentage: number
}

export interface CpuUsage {
  usage_percentage: number
  core_count: number
  cores: CpuCore[]
}

export interface MemoryInfo {
  total_ram: number
  free_ram: number
  used_ram: number
  percent: number
}

export interface DiskStats {
  read_bytes: number
  write_bytes: number
  read_rate: number
  write_rate: number
}

export interface NetworkStats {
  rx_bytes: number
  tx_bytes: number
  rx_rate: number
  tx_rate: number
}

export interface SystemStats {
  cpu: CpuUsage
  memory: MemoryInfo
  disk: DiskStats
  network: NetworkStats
  timestamp: number
}

export interface ResourceLimits {
  cpu_limit: number // yüzde cinsinden
  memory_limit: number // MB cinsinden
  io_limit: number // KB/s cinsinden
}

