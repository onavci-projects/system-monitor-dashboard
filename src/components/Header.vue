<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'

defineProps<{
  isConnected: boolean
}>()

const currentTime = ref(new Date())
let timeInterval: number | null = null

onMounted(() => {
  timeInterval = window.setInterval(() => {
    currentTime.value = new Date()
  }, 1000)
})

onUnmounted(() => {
  if (timeInterval) clearInterval(timeInterval)
})
</script>

<template>
  <header class="header">
    <div class="header-left">
      <div class="logo">
        <div class="logo-icon">
          <span class="pulse-dot"></span>
          ◈
        </div>
        <div class="logo-text">
          <h1>SİSTEM MONİTÖRÜ</h1>
          <span class="subtitle">Gerçek Zamanlı İzleme</span>
        </div>
      </div>
    </div>
    
    <div class="header-center">
      <div class="status-indicator" :class="{ connected: isConnected }">
        <span class="status-dot"></span>
        <span class="status-text">{{ isConnected ? 'BAĞLI' : 'BAĞLANTI YOK' }}</span>
      </div>
    </div>

    <div class="header-right">
      <div class="time-display">
        <span class="time">{{ currentTime.toLocaleTimeString('tr-TR') }}</span>
        <span class="date">{{ currentTime.toLocaleDateString('tr-TR', { weekday: 'short', day: 'numeric', month: 'short' }) }}</span>
      </div>
    </div>
  </header>
</template>

<style scoped>
.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20px 24px;
  background: var(--bg-card);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  margin-bottom: 24px;
  position: relative;
  overflow: hidden;
}

.header::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 2px;
  background: linear-gradient(90deg, 
    transparent, 
    var(--accent-cyan), 
    var(--accent-green), 
    var(--accent-cyan), 
    transparent
  );
  animation: header-glow 3s ease-in-out infinite;
}

@keyframes header-glow {
  0%, 100% { opacity: 0.5; }
  50% { opacity: 1; }
}

.header-left {
  display: flex;
  align-items: center;
}

.logo {
  display: flex;
  align-items: center;
  gap: 16px;
}

.logo-icon {
  width: 50px;
  height: 50px;
  background: linear-gradient(135deg, rgba(0, 217, 255, 0.2) 0%, rgba(0, 153, 204, 0.2) 100%);
  border: 2px solid var(--accent-cyan);
  border-radius: var(--radius-md);
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 24px;
  color: var(--accent-cyan);
  position: relative;
  animation: pulse-glow 2s ease-in-out infinite;
}

.pulse-dot {
  position: absolute;
  top: 8px;
  right: 8px;
  width: 8px;
  height: 8px;
  background: var(--accent-green);
  border-radius: 50%;
  animation: pulse 1.5s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% { transform: scale(1); opacity: 1; }
  50% { transform: scale(1.2); opacity: 0.7; }
}

.logo-text h1 {
  font-family: var(--font-mono);
  font-size: 22px;
  font-weight: 700;
  letter-spacing: 2px;
  background: linear-gradient(135deg, var(--accent-cyan) 0%, var(--text-primary) 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.subtitle {
  font-size: 12px;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 3px;
}

.header-center {
  position: absolute;
  left: 50%;
  transform: translateX(-50%);
}

.status-indicator {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 10px 20px;
  background: rgba(255, 51, 102, 0.1);
  border: 1px solid var(--accent-red);
  border-radius: 25px;
  transition: all 0.3s ease;
}

.status-indicator.connected {
  background: rgba(57, 255, 20, 0.1);
  border-color: var(--accent-green);
}

.status-dot {
  width: 10px;
  height: 10px;
  background: var(--accent-red);
  border-radius: 50%;
  animation: blink 1s ease-in-out infinite;
}

.status-indicator.connected .status-dot {
  background: var(--accent-green);
  animation: pulse 2s ease-in-out infinite;
}

@keyframes blink {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.3; }
}

.status-text {
  font-family: var(--font-mono);
  font-size: 12px;
  font-weight: 600;
  letter-spacing: 1px;
  color: var(--accent-red);
}

.status-indicator.connected .status-text {
  color: var(--accent-green);
}

.header-right {
  display: flex;
  align-items: center;
}

.time-display {
  display: flex;
  flex-direction: column;
  align-items: flex-end;
  gap: 4px;
}

.time {
  font-family: var(--font-mono);
  font-size: 24px;
  font-weight: 600;
  color: var(--text-primary);
}

.date {
  font-size: 13px;
  color: var(--text-secondary);
  text-transform: uppercase;
}
</style>

