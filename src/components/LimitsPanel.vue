<script setup lang="ts">
import type { ResourceLimits } from '../types/system'

const props = defineProps<{
  limits: ResourceLimits
}>()

const emit = defineEmits<{
  (e: 'update:limits', value: ResourceLimits): void
  (e: 'apply'): void
}>()

const updateLimit = (key: keyof ResourceLimits, value: number) => {
  emit('update:limits', { ...props.limits, [key]: value })
}

const formatMemory = (mb: number) => {
  if (mb >= 1024) {
    return `${(mb / 1024).toFixed(1)} GB`
  }
  return `${mb} MB`
}
</script>

<template>
  <div class="limits-panel">
    <div class="panel-header">
      <div class="panel-icon">⚙</div>
      <div class="panel-title">
        <h3>KAYNAK LİMİTLERİ</h3>
        <span class="panel-subtitle">WSL Kaynak Kısıtlamaları</span>
      </div>
    </div>

    <div class="limits-grid">
      <div class="limit-item">
        <div class="limit-header">
          <span class="limit-icon cpu">●</span>
          <label class="limit-label">CPU Limiti</label>
        </div>
        <div class="limit-control">
          <input 
            type="range" 
            :value="limits.cpu_limit" 
            @input="(e) => updateLimit('cpu_limit', Number((e.target as HTMLInputElement).value))"
            min="10" 
            max="100" 
            step="5"
            class="limit-slider cpu"
          />
          <div class="limit-value">
            <span class="value">{{ limits.cpu_limit }}</span>
            <span class="unit">%</span>
          </div>
        </div>
        <div class="limit-marks">
          <span>10%</span>
          <span>50%</span>
          <span>100%</span>
        </div>
      </div>

      <div class="limit-item">
        <div class="limit-header">
          <span class="limit-icon memory">●</span>
          <label class="limit-label">Bellek Limiti</label>
        </div>
        <div class="limit-control">
          <input 
            type="range" 
            :value="limits.memory_limit" 
            @input="(e) => updateLimit('memory_limit', Number((e.target as HTMLInputElement).value))"
            min="512" 
            max="16384" 
            step="256"
            class="limit-slider memory"
          />
          <div class="limit-value">
            <span class="value">{{ formatMemory(limits.memory_limit) }}</span>
          </div>
        </div>
        <div class="limit-marks">
          <span>512MB</span>
          <span>8GB</span>
          <span>16GB</span>
        </div>
      </div>

      <div class="limit-item">
        <div class="limit-header">
          <span class="limit-icon io">●</span>
          <label class="limit-label">I/O Limiti</label>
        </div>
        <div class="limit-control">
          <input 
            type="range" 
            :value="limits.io_limit" 
            @input="(e) => updateLimit('io_limit', Number((e.target as HTMLInputElement).value))"
            min="1024" 
            max="102400" 
            step="1024"
            class="limit-slider io"
          />
          <div class="limit-value">
            <span class="value">{{ (limits.io_limit / 1024).toFixed(0) }}</span>
            <span class="unit">MB/s</span>
          </div>
        </div>
        <div class="limit-marks">
          <span>1 MB/s</span>
          <span>50 MB/s</span>
          <span>100 MB/s</span>
        </div>
      </div>
    </div>

    <div class="panel-footer">
      <button class="apply-btn" @click="emit('apply')">
        <span class="btn-icon">▶</span>
        Limitleri Uygula
      </button>
      <div class="info-text">
        <span class="info-icon">ℹ</span>
        <span>WSL kaynak limitleri .wslconfig dosyası üzerinden uygulanır</span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.limits-panel {
  background: var(--bg-card);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  padding: 24px;
  position: relative;
  overflow: hidden;
}

.limits-panel::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 3px;
  background: linear-gradient(90deg, var(--accent-cyan), var(--accent-green), var(--accent-yellow));
}

.panel-header {
  display: flex;
  align-items: center;
  gap: 16px;
  margin-bottom: 28px;
}

.panel-icon {
  width: 50px;
  height: 50px;
  background: linear-gradient(135deg, rgba(0, 217, 255, 0.1) 0%, rgba(57, 255, 20, 0.1) 100%);
  border: 1px solid var(--accent-cyan);
  border-radius: var(--radius-md);
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 24px;
}

.panel-title h3 {
  font-family: var(--font-mono);
  font-size: 16px;
  font-weight: 600;
  letter-spacing: 2px;
  color: var(--text-primary);
}

.panel-subtitle {
  font-size: 13px;
  color: var(--text-secondary);
}

.limits-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 24px;
  margin-bottom: 28px;
}

@media (max-width: 1024px) {
  .limits-grid {
    grid-template-columns: 1fr;
  }
}

.limit-item {
  background: var(--bg-secondary);
  border-radius: var(--radius-md);
  padding: 20px;
}

.limit-header {
  display: flex;
  align-items: center;
  gap: 10px;
  margin-bottom: 16px;
}

.limit-icon {
  font-size: 12px;
}

.limit-icon.cpu { color: var(--accent-cyan); }
.limit-icon.memory { color: var(--accent-green); }
.limit-icon.io { color: var(--accent-yellow); }

.limit-label {
  font-family: var(--font-mono);
  font-size: 13px;
  font-weight: 500;
  color: var(--text-secondary);
  letter-spacing: 0.5px;
}

.limit-control {
  display: flex;
  align-items: center;
  gap: 16px;
  margin-bottom: 12px;
}

.limit-slider {
  flex: 1;
  -webkit-appearance: none;
  appearance: none;
  height: 8px;
  background: var(--bg-primary);
  border-radius: 4px;
  outline: none;
  cursor: pointer;
}

.limit-slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 20px;
  height: 20px;
  background: var(--bg-card);
  border: 3px solid;
  border-radius: 50%;
  cursor: pointer;
  transition: all 0.2s ease;
}

.limit-slider.cpu::-webkit-slider-thumb { border-color: var(--accent-cyan); }
.limit-slider.memory::-webkit-slider-thumb { border-color: var(--accent-green); }
.limit-slider.io::-webkit-slider-thumb { border-color: var(--accent-yellow); }

.limit-slider::-webkit-slider-thumb:hover {
  transform: scale(1.1);
}

.limit-slider::-moz-range-thumb {
  width: 20px;
  height: 20px;
  background: var(--bg-card);
  border: 3px solid;
  border-radius: 50%;
  cursor: pointer;
}

.limit-slider.cpu::-moz-range-thumb { border-color: var(--accent-cyan); }
.limit-slider.memory::-moz-range-thumb { border-color: var(--accent-green); }
.limit-slider.io::-moz-range-thumb { border-color: var(--accent-yellow); }

.limit-value {
  min-width: 80px;
  text-align: right;
  display: flex;
  align-items: baseline;
  justify-content: flex-end;
  gap: 4px;
}

.limit-value .value {
  font-family: var(--font-mono);
  font-size: 22px;
  font-weight: 700;
  color: var(--text-primary);
}

.limit-value .unit {
  font-family: var(--font-mono);
  font-size: 13px;
  color: var(--text-secondary);
}

.limit-marks {
  display: flex;
  justify-content: space-between;
  font-family: var(--font-mono);
  font-size: 10px;
  color: var(--text-muted);
}

.panel-footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding-top: 20px;
  border-top: 1px solid var(--border-color);
}

.apply-btn {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 14px 28px;
  background: linear-gradient(135deg, rgba(0, 217, 255, 0.2) 0%, rgba(57, 255, 20, 0.2) 100%);
  border: 1px solid var(--accent-cyan);
  border-radius: var(--radius-md);
  color: var(--accent-cyan);
  font-family: var(--font-mono);
  font-size: 14px;
  font-weight: 600;
  letter-spacing: 1px;
  cursor: pointer;
  transition: all 0.3s ease;
}

.apply-btn:hover {
  background: linear-gradient(135deg, rgba(0, 217, 255, 0.3) 0%, rgba(57, 255, 20, 0.3) 100%);
  transform: translateY(-2px);
  box-shadow: 0 4px 20px rgba(0, 217, 255, 0.3);
}

.btn-icon {
  font-size: 12px;
}

.info-text {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 12px;
  color: var(--text-muted);
}

.info-icon {
  font-size: 14px;
  color: var(--accent-cyan);
}
</style>

