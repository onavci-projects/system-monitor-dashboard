#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include "monitor.h"

double cpu_history[HISTORY_SIZE] = {0};
double mem_history[HISTORY_SIZE] = {0};
int history_index = 0;

void draw_progress_bar(int y, int x, double percent, int width, const char *label)
{
    int filled = (int)((percent / 100.0) * width);

    mvprintw(y, x, "%-15s", label);
    mvprintw(y, x + 15, "[");

    int color_pair;
    if (percent > 80)
    {
        color_pair = 3;
    }
    else if (percent > 50)
    {
        color_pair = 2;
    }
    else
    {
        color_pair = 1;
    }

    attron(COLOR_PAIR(color_pair));
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
        {
            mvprintw(y, x + 16 + i, "#");
        }
        else
        {
            mvprintw(y, x + 16 + i, "-");
        }
    }
    attroff(COLOR_PAIR(color_pair));

    mvprintw(y, x + 16 + width, "]%5.1f%%", percent);
}

void draw_mini_bar(int y, int x, double percent, int width)
{
    int filled = (int)((percent / 100.0) * width);

    int color_pair;
    if (percent > 80)
    {
        color_pair = 3;
    }
    else if (percent > 50)
    {
        color_pair = 2;
    }
    else
    {
        color_pair = 1;
    }

    mvprintw(y, x, "[");
    attron(COLOR_PAIR(color_pair));
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
        {
            mvprintw(y, x + 1 + i, "#");
        }
        else
        {
            mvprintw(y, x + 1 + i, "-");
        }
    }
    attroff(COLOR_PAIR(color_pair));
    mvprintw(y, x + width + 1, "]%4.1f%%", percent);
}

void draw_mini_chart(int y, int x, double *data, int size, int width, const char *title)
{
    attron(COLOR_PAIR(4));
    mvprintw(y, x, "%s:", title);
    attroff(COLOR_PAIR(4));

    mvprintw(y + 1, x, "100|");
    mvprintw(y + 2, x, " 50|");
    mvprintw(y + 3, x, "  0+");

    for (int i = 0; i < width && i < size; i++)
    {
        int data_index = (history_index - width + i + size) % size;
        double value = data[data_index];

        int bar_height = (int)((value / 100.0) * 3);

        for (int h = 0; h < 3; h++)
        {
            int color;
            if (value > 80)
                color = 3;
            else if (value > 50)
                color = 2;
            else
                color = 1;

            if (2 - h < bar_height)
            {
                attron(COLOR_PAIR(color));
                mvprintw(y + 1 + h, x + 4 + i, "|");
                attroff(COLOR_PAIR(color));
            }
            else
            {
                mvprintw(y + 1 + h, x + 4 + i, " ");
            }
        }
    }

    for (int i = 0; i < width; i++)
    {
        mvprintw(y + 4, x + 4 + i, "-");
    }
}

void draw_box(int y, int x, int height, int width, const char *title)
{
    attron(COLOR_PAIR(4));

    mvprintw(y, x, "+");
    for (int i = 1; i < width - 1; i++)
    {
        mvprintw(y, x + i, "=");
    }
    mvprintw(y, x + width - 1, "+");

    if (strlen(title) > 0)
    {
        mvprintw(y, x + 2, "[ %s ]", title);
    }

    for (int i = 1; i < height - 1; i++)
    {
        mvprintw(y + i, x, "|");
        mvprintw(y + i, x + width - 1, "|");
    }

    mvprintw(y + height - 1, x, "+");
    for (int i = 1; i < width - 1; i++)
    {
        mvprintw(y + height - 1, x + i, "=");
    }
    mvprintw(y + height - 1, x + width - 1, "+");

    attroff(COLOR_PAIR(4));
}

int main()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);

    MemoryInfo mem;
    CpuUsage cpu = {0};
    DiskStats disk = {0};
    NetworkStats net = {0};

    int counter = 0;

    while (1)
    {
        int ch = getch();
        if (ch == 27)
        {
            break;
        }

        mem = get_memory_info();
        get_cpu_usage(&cpu);
        get_disk_stats(&disk);
        get_network_stats(&net);

        cpu_history[history_index] = cpu.usage_percentage;
        mem_history[history_index] = mem.percent;
        history_index = (history_index + 1) % HISTORY_SIZE;

        clear();

        attron(COLOR_PAIR(4) | A_BOLD);
        const char *dots[] = {"|", "/", "-", "\\", "|", "/", "-", "\\"};
        mvprintw(0, 0, "+======================================================================+");
        mvprintw(1, 0, "| %s SISTEM IZLEME - Gercek Zamanli [%d Cekirdek]                    |",
                 dots[counter % 8], cpu.core_count);
        mvprintw(2, 0, "+======================================================================+");
        attroff(COLOR_PAIR(4) | A_BOLD);

        draw_box(4, 2, 13, 35, "ISLEMCI & BELLEK");

        draw_progress_bar(5, 4, cpu.usage_percentage, 9, "CPU Ortalama"); // 12 › 9
        draw_progress_bar(6, 4, mem.percent, 9, "Bellek");                // 12 › 9

 fprintf(fp, "  \"memory_used\": %ld,\n", mem.used_ram / 1024);
fprintf(fp, "  \"memory_total\": %ld,\n", mem.total_ram / 1024);



        attron(COLOR_PAIR(4));
        mvprintw(10, 4, "CPU Cekirdekleri:");
        attroff(COLOR_PAIR(4));
        for (int i = 0; i < cpu.core_count && i < 8; i++)
        {
            int row = i / 4;
            int col = i % 4;
            mvprintw(11 + row, 4 + (col * 7), "C%d:%2.0f%%", i, cpu.cores[i].usage_percentage);
        }

        draw_box(4, 38, 13, 35, "GECMIS (60sn)");
        draw_mini_chart(5, 40, cpu_history, HISTORY_SIZE, 28, "CPU");
        draw_mini_chart(10, 40, mem_history, HISTORY_SIZE, 28, "RAM");

        draw_box(18, 2, 6, 71, "DISK & AG"); // 15 › 18 (Y pozisyonu, kutu aþaðý kaydý)

        attron(COLOR_PAIR(5));
        mvprintw(20, 4, "DISK (sda)");                            // 17 › 20
        mvprintw(21, 6, ">> Okuma: %8.2f KB/s", disk.read_rate);  // 18 › 21
        mvprintw(22, 6, "<< Yazma: %8.2f KB/s", disk.write_rate); // 19 › 22

        mvprintw(20, 39, "AG (enp0s3)");                         // 17 › 20
        mvprintw(21, 41, "vv Indirme: %8.2f KB/s", net.rx_rate); // 18 › 21
        mvprintw(22, 41, "^^ Yukleme: %8.2f KB/s", net.tx_rate); // 19 › 22
        attroff(COLOR_PAIR(5));

        attron(COLOR_PAIR(4));
        mvprintw(25, 2, "-----------------------------------------------------------------------"); // 22 › 25
        mvprintw(26, 2, "ESC tusuna basarak veya Scroll yaparak cikis yapabilirsiniz. | Guncelleme: Her 1 saniye");                     // 23 › 26
        attroff(COLOR_PAIR(4));

        refresh();
        counter++;
        sleep(1);
    }

    endwin();
    return 0;
}
