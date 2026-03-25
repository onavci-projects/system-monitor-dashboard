import express from 'express'
import cors from 'cors'
import { readFileSync, existsSync, watchFile } from 'fs'

const app = express()
const PORT = 3001

app.use(cors())
app.use(express.json())

// C programının yazdığı JSON dosyası
const JSON_FILE = '../backend/tmp/system_monitor.json'

// Son okunan veri (cache)
let cachedData = null
let lastReadTime = 0


// C programından veri oku
function readSystemData() {
  if (!existsSync(JSON_FILE)) {
    console.warn(`${JSON_FILE} bulunamadı, simülasyon kullanılıyor`)
  }

  const data = readFileSync(JSON_FILE, 'utf-8')
  const parsed = JSON.parse(data)

  cachedData = parsed
  lastReadTime = Date.now()

  return parsed
}

// JSON dosyasını izle (değiştiğinde otomatik güncelle)
if (existsSync(JSON_FILE)) {
  watchFile(JSON_FILE, { interval: 500 }, (curr, prev) => {
    if (curr.mtime !== prev.mtime) {
      readSystemData()
    }
  })
  console.log(`${JSON_FILE} izleniyor...`)
} else {
  console.warn(`${JSON_FILE} bulunamadı. C backend çalışıyor mu?`)
}

// API endpoint'leri
app.get('/api/stats', (req, res) => {
  const rawData = readSystemData()

  // C programının formatını Vue'nun beklediği formata çevir
  const stats = {
    cpu: {
      usage_percentage: rawData.cpu || 0,
      core_count: rawData.cores ? rawData.cores.length : 0,
      cores: (rawData.cores || []).map(percent => ({
        usage_percentage: percent
      }))
    },
    memory: {
      total_ram: rawData.memory_total || 0,
      used_ram: rawData.memory_used || 0,
      free_ram: (rawData.memory_total || 0) - (rawData.memory_used || 0),
      percent: rawData.memory || 0
    },
    disk: {
      read_bytes: 0,
      write_bytes: 0,
      read_rate: rawData.disk_read || 0,
      write_rate: rawData.disk_write || 0
    },
    network: {
      rx_bytes: 0,
      tx_bytes: 0,
      rx_rate: rawData.net_download || 0,
      tx_rate: rawData.net_upload || 0
    },
    timestamp: Date.now(),
    simulated: !existsSync(JSON_FILE)
  }

  res.json(stats)
})

app.post('/api/limits', (req, res) => {
  const { cpu_limit, memory_limit, io_limit } = req.body

  console.log('Yeni limitler alındı:', { cpu_limit, memory_limit, io_limit })

  const wslConfig = `[wsl2]
memory=${Math.round(memory_limit)}MB
processors=${Math.ceil(cpu_limit / 100 * 4)}
`

  console.log('Önerilen .wslconfig içeriği:')
  console.log(wslConfig)

  res.json({
    success: true,
    message: 'Limitler kaydedildi',
    wslconfig: wslConfig
  })
})

app.get('/api/health', (req, res) => {
  const healthy = existsSync(JSON_FILE)
  res.json({
    status: healthy ? 'ok' : 'degraded',
    backend: healthy ? 'running' : 'not found',
    timestamp: Date.now(),
    file: JSON_FILE
  })
})

app.listen(PORT, () => {
  const backendStatus = existsSync(JSON_FILE) ? '✅ C Backend Aktif' : '⚠️  C Backend Bulunamadı'
  console.log(`
╔════════════════════════════════════════════════════════════╗
║                  SİSTEM MONİTÖRÜ API                       ║
║                                                            ║
║  Sunucu başlatıldı: http://localhost:${PORT}                 ║
║  Backend: ${backendStatus}                       
║  Veri Kaynağı: ${JSON_FILE}           
║                                                            ║
║  Endpoints:                                                ║
║    GET  /api/stats   - Sistem istatistikleri               ║
║    POST /api/limits  - Kaynak limitlerini ayarla           ║
║    GET  /api/health  - Sunucu durumu                       ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
`)
})