# System Monitor Dashboard 📊

> Real-time system monitoring dashboard with a C collector, Node.js API, and Vue frontend.
> 
>Gerçek zamanlı sistem izleme paneli: C veri toplayıcı, Node.js API ve Vue arayüz.

---

## 🇹🇷 Türkçe

### Proje Ne Yapar? 🚀

Bu proje CPU, RAM, Disk I/O ve Ağ metriklerini gerçek zamanlı toplar ve dashboard üzerinde gösterir.

### Mimari (3 Katman) 🧱

1. *C Collector (backend/)*
   - Linux sisteminden metrik toplar.
   - backend/tmp/system_monitor.json dosyasına her 1 saniyede yazar.

2. *Node API (server/)*
   - JSON dosyasını okur.
   - GET /api/stats ile frontend'e veri sunar.

3. *Vue Frontend (src/)*
   - API'den veriyi çekip grafik/kartlarda gösterir.

### Veri Akışı 🔄

C Writer -> system_monitor.json -> Node API (3001) -> Vite Frontend (5173)

### Gereksinimler ✅

- Node.js (18+ önerilir)
- npm
- WSL2 + Linux distro (Ubuntu önerilir)
- GCC / build-essential (WSL içinde)

### Neden WSL Gerekli? 🐧

backend/ içindeki C kodu Linux sistem kaynaklarını (/proc, vb.) okuyarak metrik toplar. Bu nedenle C katmanı WSL/Linux ortamında çalışmalıdır.

### Kurulum ⚙️

Repo kökünde:

bash
npm install
cd server && npm install


### Çalıştırma - Seçenek 1 (3 Terminal, en garanti) 🖥️

*Terminal 1 (WSL) - C writer*

bash
wsl
cd /path/to/system-monitor-dashboard/backend
gcc -Wall -Wextra -g -c memory.c cpu.c disk.c network.c json_writer.c
gcc -Wall -Wextra -g -o json_writer memory.o cpu.o disk.o network.o json_writer.o
mkdir -p tmp
./json_writer


*Terminal 2 (Windows veya WSL) - API*

bash
cd /path/to/system-monitor-dashboard
npm run server


*Terminal 3 (Windows veya WSL) - Frontend*

bash
cd /path/to/system-monitor-dashboard
npm run dev


Tarayıcı: http://localhost:5173

### Çalıştırma - Seçenek 2 (2 Terminal, pratik) ⚡

server + frontend + electron tek komutla açılır.

*Terminal 1 (WSL) - C writer*

bash
wsl
cd /path/to/system-monitor-dashboard/backend
./json_writer


*Terminal 2 (Windows) - API + Frontend + Electron*

bash
cd /path/to/system-monitor-dashboard
npm run electron:dev


### API Endpointleri 🌐

- GET /api/stats -> Anlık metrikler
- POST /api/limits -> Limit bilgisi gönderir
- GET /api/health -> API sağlık durumu

### Sık Karşılaşılan Sorunlar 🛠️

*1) Veriler sabit veya 0 görünüyor*
- json_writer çalışıyor mu kontrol et.
- backend/tmp/system_monitor.json dosyası güncelleniyor mu kontrol et.

*2) C Backend Bulunamadı uyarısı*
- API JSON dosyasını bulamıyor olabilir.
- C writer sürecinin WSL içinde aktif olduğundan emin ol.

*3) Port çakışması*
- 3001 veya 5173 portunu kullanan süreci kapatıp tekrar dene.

---

## 🇬🇧 English

### What This Project Does 🚀

This project collects real-time CPU, Memory, Disk I/O, and Network metrics, then visualizes them on a dashboard.

### Architecture (3 Layers) 🧱

1. *C Collector (backend/)*
   - Reads low-level Linux system metrics.
   - Writes data every second to backend/tmp/system_monitor.json.

2. *Node API (server/)*
   - Reads the JSON file.
   - Serves data to the frontend via GET /api/stats.

3. *Vue Frontend (src/)*
   - Polls the API and renders charts/cards in real time.

### Data Flow 🔄

C Writer -> system_monitor.json -> Node API (3001) -> Vite Frontend (5173)

### Requirements ✅

- Node.js (18+ recommended)
- npm
- WSL2 + Linux distro (Ubuntu recommended)
- GCC / build-essential (inside WSL)

### Why WSL Is Required 🐧

The C collector reads Linux system interfaces (/proc, etc.). Because of this, the C layer must run in WSL/Linux.

### Setup ⚙️

From repository root:

bash
npm install
cd server && npm install


### Run - Option 1 (3 terminals, safest) 🖥️

*Terminal 1 (WSL) - C writer*

bash
wsl
cd /path/to/system-monitor-dashboard/backend
gcc -Wall -Wextra -g -c memory.c cpu.c disk.c network.c json_writer.c
gcc -Wall -Wextra -g -o json_writer memory.o cpu.o disk.o network.o json_writer.o
mkdir -p tmp
./json_writer


*Terminal 2 (Windows or WSL) - API*

bash
cd /path/to/system-monitor-dashboard
npm run server


*Terminal 3 (Windows or WSL) - Frontend*

bash
cd /path/to/system-monitor-dashboard
npm run dev


Open: http://localhost:5173

### Run - Option 2 (2 terminals, practical) ⚡

This starts server + frontend + electron with one command.

*Terminal 1 (WSL) - C writer*

bash
wsl
cd /path/to/system-monitor-dashboard/backend
./json_writer


*Terminal 2 (Windows) - API + Frontend + Electron*

bash
cd /path/to/system-monitor-dashboard
npm run electron:dev


### API Endpoints 🌐

- GET /api/stats -> Live metrics
- POST /api/limits -> Sends resource limit settings
- GET /api/health -> API health check

### Common Issues 🛠️

*1) Values are static or always zero*
- Check whether json_writer is running.
- Check whether backend/tmp/system_monitor.json is being updated.

*2) C Backend Not Found warning*
- The API cannot find/read the JSON file.
- Ensure C writer is running in WSL.

*3) Port conflicts*
- Free ports 3001 (API) and 5173 (frontend), then restart.

---

## Project Structure 📁

text
system-monitor-dashboard/
├─ backend/          # C collector and metric generation
│  └─ tmp/           # Generated JSON output
├─ server/           # Express API
├─ src/              # Vue frontend
├─ electron/         # Electron launcher
├─ package.json      # Root scripts/dependencies
└─ README.md