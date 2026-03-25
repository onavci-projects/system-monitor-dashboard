export interface SystemData {
    cpu: number;
    memory: number;
    memory_used: number;
    memory_total: number;
    disk_read: number;
    disk_write: number;
    net_download: number;
    net_upload: number;
    cores: number[];
}

const DATA_FILE = '/tmp/system_monitor.json';

export async function readSystemData(): Promise<SystemData | null> {
    try {
        const fs = require('fs');
        const data = fs.readFileSync(DATA_FILE, 'utf8');
        return JSON.parse(data);
    } catch (error) {
        console.error('Error reading system data:', error);
        return null;
    }
}

// Polling için
export function startDataPolling(callback: (data: SystemData) => void, interval = 1000) {
    const poll = async () => {
        const data = await readSystemData();
        if (data) {
            callback(data);
        }
    };

    poll(); // İlk okuma
    const timer = setInterval(poll, interval);

    return () => clearInterval(timer); // Cleanup function
}