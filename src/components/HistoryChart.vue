<script setup lang="ts">
import { ref, watch, onMounted } from 'vue'
import { Line } from 'vue-chartjs'
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
} from 'chart.js'

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
)

const props = defineProps<{
  title: string
  data1: number[]
  data2: number[]
  label1: string
  label2: string
  color1: string
  color2: string
  isPercentage: boolean
}>()

const chartKey = ref(0)

const chartData = ref({
  labels: Array.from({ length: 60 }, (_, i) => `${60 - i}s`),
  datasets: [
    {
      label: props.label1,
      data: props.data1,
      borderColor: props.color1,
      backgroundColor: `${props.color1}20`,
      fill: true,
      tension: 0.4,
      pointRadius: 0,
      borderWidth: 2
    },
    {
      label: props.label2,
      data: props.data2,
      borderColor: props.color2,
      backgroundColor: `${props.color2}20`,
      fill: true,
      tension: 0.4,
      pointRadius: 0,
      borderWidth: 2
    }
  ]
})

const chartOptions = ref({
  responsive: true,
  maintainAspectRatio: false,
  animation: {
    duration: 300
  },
  interaction: {
    intersect: false,
    mode: 'index' as const
  },
  plugins: {
    legend: {
      display: true,
      position: 'top' as const,
      labels: {
        color: '#8b949e',
        usePointStyle: true,
        pointStyle: 'circle',
        padding: 20,
        font: {
          family: "'JetBrains Mono', monospace",
          size: 11
        }
      }
    },
    tooltip: {
      backgroundColor: '#161b22',
      titleColor: '#e6edf3',
      bodyColor: '#8b949e',
      borderColor: '#30363d',
      borderWidth: 1,
      padding: 12,
      titleFont: {
        family: "'JetBrains Mono', monospace",
        size: 12
      },
      bodyFont: {
        family: "'JetBrains Mono', monospace",
        size: 11
      },
      callbacks: {
        label: function(context: any) {
          let label = context.dataset.label || ''
          if (label) {
            label += ': '
          }
          if (context.parsed.y !== null) {
            if (props.isPercentage) {
              label += context.parsed.y.toFixed(1) + '%'
            } else {
              label += context.parsed.y.toFixed(2)
            }
          }
          return label
        }
      }
    }
  },
  scales: {
    x: {
      display: true,
      grid: {
        color: '#30363d40',
        drawBorder: false
      },
      ticks: {
        color: '#484f58',
        font: {
          family: "'JetBrains Mono', monospace",
          size: 10
        },
        maxTicksLimit: 6
      }
    },
    y: {
      display: true,
      min: 0,
      max: props.isPercentage ? 100 : undefined,
      grid: {
        color: '#30363d40',
        drawBorder: false
      },
      ticks: {
        color: '#484f58',
        font: {
          family: "'JetBrains Mono', monospace",
          size: 10
        },
        callback: function(value: number) {
          if (props.isPercentage) {
            return value + '%'
          }
          return value
        }
      }
    }
  }
})

watch(
  () => [props.data1, props.data2],
  () => {
    const paddedData1 = [...Array(60 - props.data1.length).fill(0), ...props.data1]
    const paddedData2 = [...Array(60 - props.data2.length).fill(0), ...props.data2]
    
    chartData.value = {
      ...chartData.value,
      datasets: [
        { ...chartData.value.datasets[0], data: paddedData1 },
        { ...chartData.value.datasets[1], data: paddedData2 }
      ]
    }
  },
  { deep: true }
)

onMounted(() => {
  chartKey.value++
})
</script>

<template>
  <div class="chart-wrapper">
    <h3 class="chart-title">{{ title }}</h3>
    <div class="chart-content">
      <Line
        :key="chartKey"
        :data="chartData"
        :options="chartOptions"
      />
    </div>
  </div>
</template>

<style scoped>
.chart-wrapper {
  height: 100%;
  display: flex;
  flex-direction: column;
}

.chart-title {
  font-family: var(--font-mono);
  font-size: 14px;
  font-weight: 600;
  letter-spacing: 1px;
  color: var(--text-secondary);
  margin-bottom: 16px;
  display: flex;
  align-items: center;
  gap: 10px;
}

.chart-title::before {
  content: '◈';
  color: var(--accent-cyan);
}

.chart-content {
  flex: 1;
  min-height: 200px;
}
</style>

