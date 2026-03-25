<script setup lang="ts">
import type { NetworkStats } from '../types/system'

defineProps<{
  network: NetworkStats
}>()

const formatRate = (rate: number) => {
  if (rate >= 1024) {
    return `${(rate / 1024).toFixed(2)} MB/s`
  }
  return `${rate.toFixed(2)} KB/s`
}

const formatBytes = (bytes: number) => {
  if (bytes >= 1073741824) {
    return `${(bytes / 1073741824).toFixed(2)} GB`
  }
  if (bytes >= 1048576) {
    return `${(bytes / 1048576).toFixed(2)} MB`
  }
  return `${(bytes / 1024).toFixed(2)} KB`
}
</script>

<template>
  <div class="metric-card">
    <div class="card-header">
      <div class="card-icon">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <path d="M5 12.55a11 11 0 0 1 14.08 0" />
          <path d="M1.42 9a16 16 0 0 1 21.16 0" />
          <path d="M8.53 16.11a6 6 0 0 1 6.95 0" />
          <line x1="12" y1="20" x2="12.01" y2="20" />
        </svg>
      </div>
      <div class="card-title">
        <h3>AĞ TRAFİĞİ</h3>
        <span class="interface-name">eth0 / enp0s3</span>
      </div>
    </div>

    <div class="network-stats">
      <div class="net-item download">
        <div class="net-header">
          <span class="net-icon">↓</span>
          <span class="net-label">İNDİRME</span>
        </div>
        <div class="net-value">{{ formatRate(network.rx_rate) }}</div>
        <div class="net-total">
          <span class="total-label">Toplam:</span>
          <span class="total-value">{{ formatBytes(network.rx_bytes) }}</span>
        </div>
        <div class="net-bar">
          <div 
            class="net-fill" 
            :style="{ width: `${Math.min(network.rx_rate / 100, 100)}%` }"
          ></div>
        </div>
      </div>

      <div class="net-item upload">
        <div class="net-header">
          <span class="net-icon">↑</span>
          <span class="net-label">YÜKLEME</span>
        </div>
        <div class="net-value">{{ formatRate(network.tx_rate) }}</div>
        <div class="net-total">
          <span class="total-label">Toplam:</span>
          <span class="total-value">{{ formatBytes(network.tx_bytes) }}</span>
        </div>
        <div class="net-bar">
          <div 
            class="net-fill" 
            :style="{ width: `${Math.min(network.tx_rate / 100, 100)}%` }"
          ></div>
        </div>
      </div>
    </div>

    <div class="connection-status">
      <div 
        class="status-dot" 
        :class="{ 'active': network.rx_rate > 0 || network.tx_rate > 0 }"
      ></div>
      <span>{{ network.rx_rate > 0 || network.tx_rate > 0 ? 'Veri Akışı Var' : 'Boşta' }}</span>
    </div>
  </div>
</template>

<style scoped>
.metric-card {
  background: var(--bg-card);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  padding: 24px;
  position: relative;
  overflow: hidden;
  transition: all 0.3s ease;
  animation: slide-in 0.3s ease-out;
  animation-delay: 0.3s;
  animation-fill-mode: both;
}

.metric-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: linear-gradient(135deg, var(--accent-purple) 0%, #9932cc 100%);
}

.metric-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 0 20px rgba(191, 64, 191, 0.15);
}

.card-header {
  display: flex;
  align-items: center;
  gap: 14px;
  margin-bottom: 20px;
}

.card-icon {
  width: 44px;
  height: 44px;
  background: rgba(191, 64, 191, 0.1);
  border: 1px solid var(--accent-purple);
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--accent-purple);
}

.card-icon svg {
  width: 24px;
  height: 24px;
}

.card-title h3 {
  font-family: var(--font-mono);
  font-size: 14px;
  font-weight: 600;
  letter-spacing: 2px;
  color: var(--text-primary);
}

.interface-name {
  font-size: 12px;
  color: var(--text-secondary);
}

.network-stats {
  display: flex;
  gap: 12px;
  margin-bottom: 20px;
}

.net-item {
  flex: 1;
  padding: 14px;
  background: var(--bg-secondary);
  border-radius: var(--radius-md);
  border-top: 3px solid;
}

.net-item.download {
  border-color: var(--accent-green);
}

.net-item.upload {
  border-color: var(--accent-cyan);
}

.net-header {
  display: flex;
  align-items: center;
  gap: 6px;
  margin-bottom: 8px;
}

.net-icon {
  font-size: 16px;
  font-weight: 700;
}

.net-item.download .net-icon {
  color: var(--accent-green);
}

.net-item.upload .net-icon {
  color: var(--accent-cyan);
}

.net-label {
  font-family: var(--font-mono);
  font-size: 10px;
  letter-spacing: 1px;
  color: var(--text-secondary);
}

.net-value {
  font-family: var(--font-mono);
  font-size: 18px;
  font-weight: 700;
  margin-bottom: 4px;
}

.net-item.download .net-value {
  color: var(--accent-green);
}

.net-item.upload .net-value {
  color: var(--accent-cyan);
}

.net-total {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
  font-size: 11px;
}

.total-label {
  color: var(--text-muted);
}

.total-value {
  font-family: var(--font-mono);
  color: var(--text-secondary);
}

.net-bar {
  height: 4px;
  background: var(--bg-primary);
  border-radius: 2px;
  overflow: hidden;
}

.net-fill {
  height: 100%;
  border-radius: 2px;
  transition: width 0.3s ease;
}

.net-item.download .net-fill {
  background: var(--gradient-green);
}

.net-item.upload .net-fill {
  background: var(--gradient-cyan);
}

.connection-status {
  display: flex;
  align-items: center;
  gap: 10px;
  font-size: 13px;
  color: var(--text-secondary);
}

.status-dot {
  width: 10px;
  height: 10px;
  background: var(--text-muted);
  border-radius: 50%;
  transition: all 0.3s ease;
}

.status-dot.active {
  background: var(--accent-purple);
  box-shadow: 0 0 10px var(--accent-purple);
  animation: pulse 1s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% { transform: scale(1); opacity: 1; }
  50% { transform: scale(1.2); opacity: 0.7; }
}
</style>

