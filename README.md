System Monitor Dashboard 📊
(Türkçe açıklamalar için aşağı kaydırın / Scroll down for Turkish)

A real-time system resource monitoring tool developed for Linux environments. This project tracks and visualizes low-level system metrics including CPU load, Memory usage, Disk I/O, and Network traffic.

Built with a 3-tier architecture, the backend uses C and Linux system calls to extract raw data directly from the OS kernel. A Node.js server acts as the middleware (API), and the frontend provides a clean, real-time visualization using TypeScript and Vite.

🚀 Features
CPU Tracking: Real-time processor load and utilization.

Memory (RAM) Statistics: Total, used, and free memory analysis.

Disk I/O: Read/write speeds and storage capacities.

Network Monitoring: Active network traffic data.

📂 Project Structure
backend/ : C codebase for low-level OS metrics extraction (cpu.c, memory.c, disk.c, Makefile)
server/ : Node.js backend to serve data to the frontend (index.js)
src/ : Frontend TypeScript and UI logic
index.html : Main entry point for the dashboard
package.json : Frontend Node.js dependencies

Sistem İzleme Aracı (Turkish) 📊
Linux ortamında (Ubuntu, WSL vb.) sistem kaynaklarını gerçek zamanlı olarak izlemek için geliştirilmiş, 3 katmanlı mimariye sahip bir sistem izleme aracıdır. Bu proje; CPU yükü, Bellek (RAM) kullanımı, Disk G/Ç işlemleri ve Ağ trafiği gibi düşük seviyeli (low-level) metrikleri takip eder ve görselleştirir.

Arka plan (backend) C dili ile yazılmış olup, doğrudan işletim sistemi çekirdeğinden veri çekmek için düşük seviyeli Linux sistem çağrılarını (open, read, write vb.) kullanır. Veriler bir Node.js sunucusu üzerinden arayüze aktarılır ve ön yüz (frontend) TypeScript ile tasarlanmıştır.

🚀 Özellikler
CPU Takibi: Gerçek zamanlı işlemci yükü ve anlık kullanım verileri.

Bellek (RAM) İstatistikleri: Toplam, kullanılan ve boş bellek analizi.

Disk G/Ç: Okuma/Yazma hızları ve disk doluluk oranları.

Ağ Trafiği (Network) İzleme: Anlık ağ kullanım verileri.

📂 Proje Yapısı
backend/ : Düşük seviyeli OS metriklerini çeken C kodları ve Makefile
server/ : Verileri ön yüze sunan Node.js sunucusu (API)
src/ : Ön yüz (Frontend) TypeScript ve arayüz mantığı
index.html : Panel için ana giriş sayfası
package.json : Ön yüz için Node.js bağımlılıkları
