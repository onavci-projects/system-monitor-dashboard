# System Monitor Dashboard

Gercek zamanli sistem izleme paneli. Proje 3 katmandan olusur:

1. **C backend (`backend/`)**: CPU, RAM, Disk ve Network metriklerini Linux sisteminden okur.
2. **Node API (`server/`)**: C tarafinin urettigi JSON dosyasini HTTP endpoint ile sunar.
3. **Vue frontend (`src/`)**: API'den veriyi cekip dashboard olarak gosterir.

---

## Mimari ve Veri Akisi

- `backend/json_writer` her 1 saniyede `backend/tmp/system_monitor.json` dosyasini gunceller.
- `server/index.js` bu dosyayi okuyup `GET /api/stats` endpoint'ine cevirir.
- Frontend (`vite`) her 1 saniyede `/api/stats` cagirir.
- Vite proxy sayesinde `/api` istekleri `http://localhost:3001` adresine yonlenir.

---

## Gereksinimler

### Zorunlu
- Node.js (18+ onerilir)
- npm
- WSL2 + Linux distro (Ubuntu onerilir)
- GCC / build-essential (WSL icinde)

### Neden WSL gerekiyor?
`backend/` icindeki C kodu Linux sistem dosyalarini (`/proc`, vb.) okuyarak veri toplar. Bu yuzden C katmani Windows native yerine WSL/Linux ortaminda calismalidir.

---

## Kurulum

Proje kok dizininde:

```bash
npm install
cd server && npm install
```

---

## Calistirma Secenekleri

## Secenek 1: 3 Terminal (en acik ve en garanti)

### Terminal 1 - WSL (C veri uretici)
```bash
wsl
cd /mnt/c/All-around/abdurrahman-micro-project/system-monitor-dashboard/backend
gcc -Wall -Wextra -g -c memory.c cpu.c disk.c network.c json_writer.c
gcc -Wall -Wextra -g -o json_writer memory.o cpu.o disk.o network.o json_writer.o
mkdir -p tmp
./json_writer
```

### Terminal 2 - Windows/WSL (Node API)
```bash
cd c:/All-around/abdurrahman-micro-project/system-monitor-dashboard
npm run server
```

### Terminal 3 - Windows/WSL (Frontend)
```bash
cd c:/All-around/abdurrahman-micro-project/system-monitor-dashboard
npm run dev
```

Ardindan tarayicida:

`http://localhost:5173`

---

## Secenek 2: 2 Terminal (onerilen pratik kullanim)

Bu secenekte `server + frontend + electron` tek komutla acilir.

### Terminal 1 - WSL (C veri uretici)
```bash
wsl
cd /mnt/c/All-around/abdurrahman-micro-project/system-monitor-dashboard/backend
./json_writer
```

### Terminal 2 - Windows (web + API + electron)
```bash
cd c:/All-around/abdurrahman-micro-project/system-monitor-dashboard
npm run electron:dev
```

---

## Endpointler

- `GET /api/stats` : Anlik sistem metrikleri
- `POST /api/limits` : Limit bilgilerini alir (simdilik kayit/onerme amacli)
- `GET /api/health` : API saglik durumu

---

## Sik Karsilasilan Sorunlar

### 1) Dashboard aciliyor ama veriler sabit/0 gorunuyor
- `json_writer` calismiyor olabilir.
- `backend/tmp/system_monitor.json` dosyasinin degisme zamanini kontrol et.
- C surecini WSL icinde baslattigindan emin ol.

### 2) `C Backend Bulunamadi` uyarisini aliyorum
- `npm run server` acikken API, `../backend/tmp/system_monitor.json` dosyasini bulamiyor demektir.
- Dosya yolunu ve writer surecini kontrol et.

### 3) Port cakismasi
- `5173` (frontend) veya `3001` (API) doluysa ilgili sureci kapatip tekrar dene.

### 4) Electron acilmiyor ama web aciliyor
- Sadece web paneli kullanmak icin `npm run server` + `npm run dev` yeterlidir.

---

## Klasor Yapisi

- `backend/` : C kaynak kodu, metrik uretimi
- `backend/tmp/` : Uretilen JSON veri dosyasi
- `server/` : Express API
- `src/` : Vue arayuz
- `electron/` : Electron baslatma yapisi

---

## Not

Bu proje Linux tabanli metrik toplama mantigi ile yazildigi icin en stabil calisma ortami WSL/Linux'tur. Node ve frontend tarafi Windows'ta calissa da C veri toplayicinin WSL icinde calistirilmasi tavsiye edilir.
