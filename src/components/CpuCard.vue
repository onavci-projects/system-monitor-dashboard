<script setup lang="ts">
import { computed } from 'vue'
import type { CpuUsage } from '../types/system'

const props = defineProps<{
  cpu: CpuUsage
  limit: number
}>()

const statusColor = computed(() => {
  const percent = props.cpu.usage_percentage
  if (percent > 80) return 'danger'
  if (percent > 50) return 'warning'
  return 'safe'
})

const isOverLimit = computed(() => {
  return props.cpu.usage_percentage > props.limit
})
</script>

<template>
  <div class="metric-card" :class="[statusColor, { 'over-limit': isOverLimit }]">
    <div class="card-header">
      <div class="card-icon">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <rect x="4" y="4" width="16" height="16" rx="2" />
          <rect x="9" y="9" width="6" height="6" />
          <line x1="9" y1="1" x2="9" y2="4" />
          <line x1="15" y1="1" x2="15" y2="4" />
          <line x1="9" y1="20" x2="9" y2="23" />
          <line x1="15" y1="20" x2="15" y2="23" />
          <line x1="20" y1="9" x2="23" y2="9" />
          <line x1="20" y1="15" x2="23" y2="15" />
          <line x1="1" y1="9" x2="4" y2="9" />
          <line x1="1" y1="15" x2="4" y2="15" />
        </svg>
      </div>
      <div class="card-title">
        <h3>İŞLEMCİ</h3>
        <span class="core-count">{{ cpu.core_count }} Çekirdek</span>
      </div>
    </div>

    <div class="usage-display">
      <div class="percentage">
        <span class="value">{{ cpu.usage_percentage.toFixed(1) }}</span>
        <span class="unit">%</span>
      </div>
      <div class="limit-badge" v-if="limit < 100">
        Limit: {{ limit }}%
      </div>
    </div>

    <div class="progress-bar">
      <div 
        class="progress-fill" 
        :style="{ width: `${Math.min(cpu.usage_percentage, 100)}%` }"
      >
        <div class="progress-glow"></div>
      </div>
      <div 
        v-if="limit < 100" 
        class="limit-marker" 
        :style="{ left: `${limit}%` }"
      ></div>
    </div>

    <div class="cores-grid">
      <div 
        v-for="(core, index) in cpu.cores.slice(0, 8)" 
        :key="index"
        class="core-item"
        :class="{
          'high': core.usage_percentage > 80,
          'medium': core.usage_percentage > 50 && core.usage_percentage <= 80
        }"
      >
        <span class="core-label">C{{ index }}</span>
        <div class="core-bar">
          <div 
            class="core-fill" 
            :style="{ height: `${core.usage_percentage}%` }"
          ></div>
        </div>
        <span class="core-value">{{ core.usage_percentage.toFixed(0) }}%</span>
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
}

.metric-card::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: var(--gradient-cyan);
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
  box-shadow: var(--shadow-glow);
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
  background: rgba(0, 217, 255, 0.1);
  border: 1px solid var(--accent-cyan);
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--accent-cyan);
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

.core-count {
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
  color: var(--accent-cyan);
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
  background: var(--gradient-cyan);
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

.limit-marker::before {
  content: '';
  position: absolute;
  top: -4px;
  left: 50%;
  transform: translateX(-50%);
  width: 0;
  height: 0;
  border-left: 5px solid transparent;
  border-right: 5px solid transparent;
  border-top: 5px solid var(--accent-red);
}

.cores-grid {
  display: grid;
  grid-template-columns: repeat(8, 1fr);
  gap: 8px;
}

.core-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
}

.core-label {
  font-family: var(--font-mono);
  font-size: 10px;
  color: var(--text-muted);
}

.core-bar {
  width: 100%;
  height: 40px;
  background: var(--bg-secondary);
  border-radius: 3px;
  position: relative;
  overflow: hidden;
}

.core-fill {
  position: absolute;
  bottom: 0;
  left: 0;
  right: 0;
  background: var(--accent-cyan);
  transition: height 0.3s ease;
  border-radius: 3px;
}

.core-item.medium .core-fill {
  background: var(--accent-yellow);
}

.core-item.high .core-fill {
  background: var(--accent-red);
}

.core-value {
  font-family: var(--font-mono);
  font-size: 10px;
  color: var(--text-secondary);
}
</style>

