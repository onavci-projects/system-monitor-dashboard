const { app, BrowserWindow } = require('electron');
const path = require('path');
const { spawn } = require('child_process');
const fs = require('fs');

let mainWindow;
let cBackendProcess;
if (!process.env.NODE_ENV) {
    process.env.NODE_ENV = 'development';
}

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1400,
        height: 900,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
        },
        backgroundColor: '#1a1a2e',
        title: 'Sistem Monitörü',
    });

    if (process.env.NODE_ENV === 'development') {
        mainWindow.loadURL('http://localhost:5173');
        mainWindow.webContents.openDevTools();
    } else {
        mainWindow.loadFile(path.join(__dirname, '../dist/index.html'));
    }

    mainWindow.on('closed', () => {
        mainWindow = null;
    });
}

function startCBackend() {
    const isDev = !app.isPackaged;

    // Dinamik path
    const backendDir = isDev
        ? path.join(__dirname, '../backend')
        : path.join(process.resourcesPath, 'backend');

    const cProgramPath = path.join(backendDir, 'json_writer');

    // WSL üzerinden çalıştır (Windows için)
    const isWindows = process.platform === 'win32';

    console.log('Starting C Backend from:', cProgramPath);

    try {
        if (isWindows) {
            // Windows'ta WSL üzerinden çalıştır
            const wslPath = cProgramPath.replace(/\\/g, '/').replace(/^([A-Z]):/, (match, drive) => {
                return `/mnt/${drive.toLowerCase()}`;
            });

            cBackendProcess = spawn('wsl', [wslPath]);
        } else {
            // Linux/Mac'te direkt çalıştır
            cBackendProcess = spawn(cProgramPath);
        }

        cBackendProcess.stdout.on('data', (data) => {
            console.log(`[C Backend] ${data}`);
        });

        cBackendProcess.stderr.on('data', (data) => {
            console.error(`[C Backend Error] ${data}`);
        });

        cBackendProcess.on('close', (code) => {
            console.log(`C Backend exited with code ${code}`);
        });

        console.log('C Backend started successfully');
    } catch (error) {
        console.error('Failed to start C Backend:', error);
    }
}

app.whenReady().then(() => {
    startCBackend();
    createWindow();

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow();
        }
    });
});

app.on('window-all-closed', () => {
    if (cBackendProcess) {
        cBackendProcess.kill();
    }
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('before-quit', () => {
    if (cBackendProcess) {
        cBackendProcess.kill();
    }
});