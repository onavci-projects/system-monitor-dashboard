<script setup lang="ts">
import { ref, onMounted, onUnmounted, computed } from 'vue'
import axios from 'axios'
import type { SystemStats, ResourceLimits } from './types/system'
import CpuCard from './components/CpuCard.vue'
import MemoryCard from './components/MemoryCard.vue'
import DiskCard from './components/DiskCard.vue'
import NetworkCard from './components/NetworkCard.vue'
import HistoryChart from './components/HistoryChart.vue'
import LimitsPanel from './components/LimitsPanel.vue'
import Header from './components/Header.vue'

const stats = ref<SystemStats | null>(null)
const cpuHistory = ref<number[]>([])
const memoryHistory = ref<number[]>([])
const diskReadHistory = ref<number[]>([])
const diskWriteHistory = ref<number[]>([])
const networkRxHistory = ref<number[]>([])
const networkTxHistory = ref<number[]>([])
const isConnected = ref(false)
const error = ref<string | null>(null)

const limits = ref<ResourceLimits>({
  cpu_limit: 100,
  memory_limit: 4096,
  io_limit: 10240
})

const HISTORY_SIZE = 60
let pollInterval: number | null = null

const fetchStats = async () => {
  try {
    const response = await axios.get<SystemStats>('/api/stats')
    stats.value = response.data
    isConnected.value = true
    error.value = null

    // Geçmiş verilerini güncelle
    cpuHistory.value = [...cpuHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.cpu.usage_percentage]
    memoryHistory.value = [...memoryHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.memory.percent]
    diskReadHistory.value = [...diskReadHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.disk.read_rate]
    diskWriteHistory.value = [...diskWriteHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.disk.write_rate]
    networkRxHistory.value = [...networkRxHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.network.rx_rate]
    networkTxHistory.value = [...networkTxHistory.value.slice(-(HISTORY_SIZE - 1)), response.data.network.tx_rate]
  } catch (err) {
    isConnected.value = false
    error.value = 'Sunucuya bağlanılamadı. Backend çalışıyor mu?'
    console.error('Stats alınamadı:', err)
  }
}

const applyLimits = async () => {
  try {
    await axios.post('/api/limits', limits.value)
  } catch (err) {
    console.error('Limitler uygulanamadı:', err)
  }
}

onMounted(() => {
  fetchStats()
  pollInterval = window.setInterval(fetchStats, 1000)
})

onUnmounted(() => {
  if (pollInterval) {
    clearInterval(pollInterval)
  }
})

const currentTime = computed(() => {
  return new Date().toLocaleTimeString('tr-TR')
})
</script>

<template>
  <div class="app">
    <Header :is-connected="isConnected" />
    
    <div v-if="error" class="error-banner">
      <span class="error-icon">⚠</span>
      {{ error }}
    </div>

    <main class="dashboard" v-if="stats">
      <!-- Üst Kısım: Ana Metrikler -->
      <section class="metrics-grid">
        <CpuCard 
          :cpu="stats.cpu" 
          :limit="limits.cpu_limit"
        />
        <MemoryCard 
          :memory="stats.memory" 
          :limit="limits.memory_limit"
        />
        <DiskCard :disk="stats.disk" />
        <NetworkCard :network="stats.network" />
      </section>

      <!-- Orta Kısım: Grafikler -->
      <section class="charts-section">
        <div class="chart-container">
          <HistoryChart 
            title="CPU & Bellek Geçmişi (60sn)"
            :data1="cpuHistory"
            :data2="memoryHistory"
            label1="CPU %"
            label2="RAM %"
            color1="#00d9ff"
            color2="#39ff14"
            :is-percentage="true"
          />
        </div>
        <div class="chart-container">
          <HistoryChart 
            title="Disk I/O Geçmişi (60sn)"
            :data1="diskReadHistory"
            :data2="diskWriteHistory"
            label1="Okuma KB/s"
            label2="Yazma KB/s"
            color1="#ffd700"
            color2="#ff6b35"
            :is-percentage="false"
          />
        </div>
      </section>

      <!-- Alt Kısım: Kaynak Limitleri -->
      <section class="limits-section">
        <LimitsPanel 
          v-model:limits="limits"
          @apply="applyLimits"
        />
      </section>
    </main>

    <div v-else class="loading">
      <div class="loading-spinner"></div>
      <p>Sistem verileri yükleniyor...</p>
    </div>

    <footer class="app-footer">
      <span class="footer-text">Sistem İzleme Monitörü • WSL Linux</span>
      <span class="footer-time">{{ currentTime }}</span>
    </footer>
  </div>
</template>

<style scoped>
.app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
}

.error-banner {
  background: linear-gradient(135deg, rgba(255, 51, 102, 0.2) 0%, rgba(204, 0, 51, 0.2) 100%);
  border: 1px solid var(--accent-red);
  border-radius: var(--radius-md);
  padding: 16px 24px;
  margin-bottom: 24px;
  display: flex;
  align-items: center;
  gap: 12px;
  animation: slide-in 0.3s ease-out;
}

.error-icon {
  font-size: 24px;
}

.dashboard {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 24px;
}

.metrics-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 20px;
}

@media (max-width: 1200px) {
  .metrics-grid {
    grid-template-columns: repeat(2, 1fr);
  }
}

@media (max-width: 768px) {
  .metrics-grid {
    grid-template-columns: 1fr;
  }
}

.charts-section {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 20px;
}

@media (max-width: 1024px) {
  .charts-section {
    grid-template-columns: 1fr;
  }
}

.chart-container {
  background: var(--bg-card);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  padding: 24px;
  animation: slide-in 0.4s ease-out;
  animation-fill-mode: both;
}

.chart-container:nth-child(1) { animation-delay: 0.2s; }
.chart-container:nth-child(2) { animation-delay: 0.3s; }

.limits-section {
  animation: slide-in 0.5s ease-out;
  animation-delay: 0.4s;
  animation-fill-mode: both;
}

.loading {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 24px;
}

.loading-spinner {
  width: 60px;
  height: 60px;
  border: 3px solid var(--border-color);
  border-top-color: var(--accent-cyan);
  border-radius: 50%;
  animation: spin 1s linear infinite;
}

@keyframes spin {
  to { transform: rotate(360deg); }
}

.loading p {
  color: var(--text-secondary);
  font-size: 18px;
}

.app-footer {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px 0;
  margin-top: 24px;
  border-top: 1px solid var(--border-color);
}

.footer-text {
  color: var(--text-muted);
  font-size: 14px;
}

.footer-time {
  font-family: var(--font-mono);
  color: var(--accent-cyan);
  font-size: 14px;
}
</style>

