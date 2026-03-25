<script setup lang="ts">
import type { DiskStats } from '../types/system'

defineProps<{
  disk: DiskStats
}>()

const formatRate = (rate: number) => {
  if (rate >= 1024) {
    return `${(rate / 1024).toFixed(2)} MB/s`
  }
  return `${rate.toFixed(2)} KB/s`
}
</script>

<template>
  <div class="metric-card">
    <div class="card-header">
      <div class="card-icon">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <ellipse cx="12" cy="5" rx="9" ry="3" />
          <path d="M21 12c0 1.66-4 3-9 3s-9-1.34-9-3" />
          <path d="M3 5v14c0 1.66 4 3 9 3s9-1.34 9-3V5" />
        </svg>
      </div>
      <div class="card-title">
        <h3>DİSK I/O</h3>
        <span class="disk-name">sda</span>
      </div>
    </div>

    <div class="io-stats">
      <div class="io-item read">
        <div class="io-header">
          <span class="io-icon">▼</span>
          <span class="io-label">OKUMA</span>
        </div>
        <div class="io-value">{{ formatRate(disk.read_rate) }}</div>
        <div class="io-bar">
          <div 
            class="io-fill" 
            :style="{ width: `${Math.min(disk.read_rate / 100, 100)}%` }"
          ></div>
        </div>
      </div>

      <div class="io-item write">
        <div class="io-header">
          <span class="io-icon">▲</span>
          <span class="io-label">YAZMA</span>
        </div>
        <div class="io-value">{{ formatRate(disk.write_rate) }}</div>
        <div class="io-bar">
          <div 
            class="io-fill" 
            :style="{ width: `${Math.min(disk.write_rate / 100, 100)}%` }"
          ></div>
        </div>
      </div>
    </div>

    <div class="activity-indicator">
      <div 
        class="activity-dot" 
        :class="{ 'active': disk.read_rate > 0 || disk.write_rate > 0 }"
      ></div>
      <span>{{ disk.read_rate > 0 || disk.write_rate > 0 ? 'Aktif' : 'Boşta' }}</span>
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
  animation-delay: 0.2s;
  animation-fill-mode: both;
}

.metric-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: var(--gradient-yellow);
}

.metric-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 0 20px rgba(255, 215, 0, 0.15);
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
  background: rgba(255, 215, 0, 0.1);
  border: 1px solid var(--accent-yellow);
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--accent-yellow);
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

.disk-name {
  font-size: 12px;
  color: var(--text-secondary);
}

.io-stats {
  display: flex;
  flex-direction: column;
  gap: 20px;
  margin-bottom: 20px;
}

.io-item {
  padding: 16px;
  background: var(--bg-secondary);
  border-radius: var(--radius-md);
  border-left: 3px solid;
}

.io-item.read {
  border-color: var(--accent-yellow);
}

.io-item.write {
  border-color: var(--accent-orange);
}

.io-header {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-bottom: 8px;
}

.io-icon {
  font-size: 14px;
}

.io-item.read .io-icon {
  color: var(--accent-yellow);
}

.io-item.write .io-icon {
  color: var(--accent-orange);
}

.io-label {
  font-family: var(--font-mono);
  font-size: 11px;
  letter-spacing: 1px;
  color: var(--text-secondary);
}

.io-value {
  font-family: var(--font-mono);
  font-size: 22px;
  font-weight: 700;
  margin-bottom: 10px;
}

.io-item.read .io-value {
  color: var(--accent-yellow);
}

.io-item.write .io-value {
  color: var(--accent-orange);
}

.io-bar {
  height: 4px;
  background: var(--bg-primary);
  border-radius: 2px;
  overflow: hidden;
}

.io-fill {
  height: 100%;
  border-radius: 2px;
  transition: width 0.3s ease;
}

.io-item.read .io-fill {
  background: var(--gradient-yellow);
}

.io-item.write .io-fill {
  background: linear-gradient(135deg, var(--accent-orange) 0%, #cc5500 100%);
}

.activity-indicator {
  display: flex;
  align-items: center;
  gap: 10px;
  font-size: 13px;
  color: var(--text-secondary);
}

.activity-dot {
  width: 10px;
  height: 10px;
  background: var(--text-muted);
  border-radius: 50%;
  transition: all 0.3s ease;
}

.activity-dot.active {
  background: var(--accent-green);
  box-shadow: 0 0 10px var(--accent-green);
  animation: pulse 1s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% { transform: scale(1); opacity: 1; }
  50% { transform: scale(1.2); opacity: 0.7; }
}
</style>

