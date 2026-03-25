<script setup lang="ts">
import { computed } from 'vue'
import type { MemoryInfo } from '../types/system'

const props = defineProps<{
  memory: MemoryInfo
  limit: number
}>()

const statusColor = computed(() => {
  const percent = props.memory.percent
  if (percent > 80) return 'danger'
  if (percent > 50) return 'warning'
  return 'safe'
})

const usedMB = computed(() => Math.round(props.memory.used_ram / 1024))
const totalMB = computed(() => Math.round(props.memory.total_ram / 1024))
const freeMB = computed(() => Math.round(props.memory.free_ram / 1024))

const isOverLimit = computed(() => {
  return usedMB.value > props.limit
})

const limitPercent = computed(() => {
  return (props.limit / totalMB.value) * 100
})

const formatSize = (mb: number) => {
  if (mb >= 1024) {
    return `${(mb / 1024).toFixed(1)} GB`
  }
  return `${mb} MB`
}
</script>

<template>
  <div class="metric-card" :class="[statusColor, { 'over-limit': isOverLimit }]">
    <div class="card-header">
      <div class="card-icon">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <rect x="3" y="6" width="18" height="12" rx="2" />
          <line x1="7" y1="10" x2="7" y2="14" />
          <line x1="11" y1="10" x2="11" y2="14" />
          <line x1="15" y1="10" x2="15" y2="14" />
        </svg>
      </div>
      <div class="card-title">
        <h3>BELLEK (RAM)</h3>
        <span class="total-mem">Toplam: {{ formatSize(totalMB) }}</span>
      </div>
    </div>

    <div class="usage-display">
      <div class="percentage">
        <span class="value">{{ memory.percent.toFixed(1) }}</span>
        <span class="unit">%</span>
      </div>
      <div class="limit-badge" v-if="limit < totalMB">
        Limit: {{ formatSize(limit) }}
      </div>
    </div>

    <div class="progress-bar">
      <div 
        class="progress-fill" 
        :style="{ width: `${Math.min(memory.percent, 100)}%` }"
      >
        <div class="progress-glow"></div>
      </div>
      <div 
        v-if="limit < totalMB" 
        class="limit-marker" 
        :style="{ left: `${limitPercent}%` }"
      ></div>
    </div>

    <div class="memory-details">
      <div class="detail-item used">
        <div class="detail-icon"></div>
        <div class="detail-info">
          <span class="detail-label">Kullanılan</span>
          <span class="detail-value">{{ formatSize(usedMB) }}</span>
        </div>
      </div>
      <div class="detail-item free">
        <div class="detail-icon"></div>
        <div class="detail-info">
          <span class="detail-label">Kullanılabilir</span>
          <span class="detail-value">{{ formatSize(freeMB) }}</span>
        </div>
      </div>
    </div>

    <div class="memory-visual">
      <div class="memory-blocks">
        <div 
          v-for="i in 20" 
          :key="i"
          class="memory-block"
          :class="{ 
            'active': (i / 20 * 100) <= memory.percent,
            'warning': (i / 20 * 100) > 50 && (i / 20 * 100) <= 80,
            'danger': (i / 20 * 100) > 80
          }"
        ></div>
      </div>
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
  animation-delay: 0.1s;
  animation-fill-mode: both;
}

.metric-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: var(--gradient-green);
  transition: all 0.3s ease;
}

.metric-card.warning::before {
  background: var(--gradient-yellow);
}

.metric-card.danger::before {
  background: var(--gradient-red);
}

.metric-card.over-limit {
  animation: pulse-glow 1s ease-in-out infinite;
  border-color: var(--accent-red);
}

.metric-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 0 20px rgba(57, 255, 20, 0.15);
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
  background: rgba(57, 255, 20, 0.1);
  border: 1px solid var(--accent-green);
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--accent-green);
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

.total-mem {
  font-size: 12px;
  color: var(--text-secondary);
}

.usage-display {
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  margin-bottom: 16px;
}

.percentage {
  display: flex;
  align-items: baseline;
  gap: 4px;
}

.value {
  font-family: var(--font-mono);
  font-size: 42px;
  font-weight: 700;
  color: var(--accent-green);
  line-height: 1;
}

.metric-card.warning .value {
  color: var(--accent-yellow);
}

.metric-card.danger .value {
  color: var(--accent-red);
}

.unit {
  font-family: var(--font-mono);
  font-size: 20px;
  color: var(--text-secondary);
}

.limit-badge {
  background: rgba(255, 51, 102, 0.2);
  border: 1px solid var(--accent-red);
  padding: 4px 10px;
  border-radius: 12px;
  font-family: var(--font-mono);
  font-size: 11px;
  color: var(--accent-red);
}

.progress-bar {
  height: 8px;
  background: var(--bg-secondary);
  border-radius: 4px;
  position: relative;
  overflow: visible;
  margin-bottom: 20px;
}

.progress-fill {
  height: 100%;
  background: var(--gradient-green);
  border-radius: 4px;
  transition: width 0.3s ease;
  position: relative;
  overflow: hidden;
}

.metric-card.warning .progress-fill {
  background: var(--gradient-yellow);
}

.metric-card.danger .progress-fill {
  background: var(--gradient-red);
}

.progress-glow {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.3), transparent);
  animation: shimmer 2s ease-in-out infinite;
}

@keyframes shimmer {
  0% { transform: translateX(-100%); }
  100% { transform: translateX(100%); }
}

.limit-marker {
  position: absolute;
  top: -4px;
  bottom: -4px;
  width: 2px;
  background: var(--accent-red);
  transform: translateX(-50%);
}

.memory-details {
  display: flex;
  gap: 20px;
  margin-bottom: 16px;
}

.detail-item {
  display: flex;
  align-items: center;
  gap: 10px;
  flex: 1;
}

.detail-icon {
  width: 12px;
  height: 12px;
  border-radius: 3px;
}

.detail-item.used .detail-icon {
  background: var(--accent-green);
}

.detail-item.free .detail-icon {
  background: var(--text-muted);
}

.detail-label {
  font-size: 12px;
  color: var(--text-secondary);
  display: block;
}

.detail-value {
  font-family: var(--font-mono);
  font-size: 14px;
  font-weight: 600;
  color: var(--text-primary);
}

.memory-visual {
  margin-top: 8px;
}

.memory-blocks {
  display: flex;
  gap: 3px;
}

.memory-block {
  flex: 1;
  height: 24px;
  background: var(--bg-secondary);
  border-radius: 3px;
  transition: all 0.3s ease;
}

.memory-block.active {
  background: var(--accent-green);
}

.memory-block.active.warning {
  background: var(--accent-yellow);
}

.memory-block.active.danger {
  background: var(--accent-red);
}
</style>

