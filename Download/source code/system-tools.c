#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <dirent.h>

#define BUF_SIZE 1028

// Format bytes into readable form
void format_bytes(unsigned long long bytes, char *buf, size_t size) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int i = 0;
    double val = (double)bytes;
    while (val >= 1024.0 && i < 5) {
        val /= 1024.0;
        i++;
    }
    snprintf(buf, size, "%.2f %s", val, units[i]);
}

// Clears terminal screen
void clear_screen() {
    (void)write(1, "\033[2J\033[H", 7);
}

// Pause until ENTER
void pause_menu() {
    (void)write(1, "\nPress ENTER to return to menu...", 33);
    getchar();
}

// Menu display
void show_menu() {
    clear_screen();
    (void)printf(
        "================================\n"
        "          SYSTEM TOOLS\n"
        "================================\n"
        "1. System Information\n"
        "2. Hardware Details\n"
        "3. Network Statistics\n"
        "4. Running Processes\n"
        "5. System Health Monitor\n"
        "6. List /dev Devices\n"
        "7. Disks\n"
        "8. Exit\n"
        "================================\n"
        "Choose an option (1-8): "
    );
}

// System information
void system_info() {
    struct utsname u;
    clear_screen();
    if (uname(&u) == 0) {
        printf("System Information\n------------------\n");
        printf("Hostname  : %s\n", u.nodename);
        printf("Kernel    : %s %s %s\n", u.sysname, u.release, u.version);
    } else {
        perror("uname");
    }
    pause_menu();
}

// Hardware details
void hardware_info() {
    FILE *fp;
    char line[BUF_SIZE];
    char cpu_model[BUF_SIZE] = "";
    int cores = 0;

    clear_screen();
    fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("fopen");
        pause_menu();
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0 && cpu_model[0] == '\0') {
            char *colon = strchr(line, ':');
            if (colon) strncpy(cpu_model, colon + 2, sizeof(cpu_model)-1);
            cpu_model[strcspn(cpu_model, "\n")] = 0;
        }
        if (strncmp(line, "processor", 9) == 0) cores++;
    }
    fclose(fp);

    struct sysinfo si;
    sysinfo(&si);

    char mem_buf[32];
    format_bytes(si.totalram, mem_buf, sizeof(mem_buf));

    printf("Hardware Details\n----------------\n");
    printf("CPU          : %s\n", cpu_model);
    printf("Total Cores  : %d\n", cores);
    printf("Total Memory : %s\n", mem_buf);
    pause_menu();
}

// Network statistics (RX/TX bytes & packets)
void network_stats() {
    FILE *fp;
    char line[BUF_SIZE];
    unsigned long long rx, tx, rx_packets, tx_packets;
    unsigned long long total_rx = 0, total_tx = 0, total_rxp = 0, total_txp = 0;

    typedef struct {
        char iface[64];
        unsigned long long rx, tx, rxp, txp;
    } iface_stat;

    iface_stat stats[64];
    int count = 0;

    clear_screen();
    printf("Network Statistics\n------------------\n");

    fp = fopen("/proc/net/dev", "r");
    if (!fp) {
        perror("fopen");
        pause_menu();
        return;
    }

    // Skip headers
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        char *ptr = line;
        while (*ptr == ' ') ptr++; // skip leading spaces

        char iface[64];
        if (sscanf(ptr, "%63[^:]: %llu %llu %*u %*u %*u %*u %*u %*u %llu %llu",
                   iface, &rx, &rx_packets, &tx, &tx_packets) == 5) {
            strcpy(stats[count].iface, iface);
            stats[count].rx = rx;
            stats[count].tx = tx;
            stats[count].rxp = rx_packets;
            stats[count].txp = tx_packets;
            total_rx += rx;
            total_tx += tx;
            total_rxp += rx_packets;
            total_txp += tx_packets;
            count++;
        }
    }

    fclose(fp);

    // Print header
    printf("+------------+------------+--------+------------+------------+--------+------------+\n");
    printf("| %-10s | %10s | %-6s | %10s | %10s | %-6s | %10s |\n",
           "Interface","RX Bytes","% RX","RX Packets","TX Bytes","% TX","TX Packets");
    printf("+------------+------------+--------+------------+------------+--------+------------+\n");

    char buf[32];
    for (int i = 0; i < count; i++) {
        format_bytes(stats[i].rx, buf, sizeof(buf));
        char rx_buf[32]; snprintf(rx_buf, sizeof(rx_buf), "%s", buf);
        format_bytes(stats[i].tx, buf, sizeof(buf));
        char tx_buf[32]; snprintf(tx_buf, sizeof(tx_buf), "%s", buf);

        printf("| %-10s | %10s | %-6.2f | %10llu | %10s | %-6.2f | %10llu |\n",
               stats[i].iface,
               rx_buf,
               stats[i].rx * 100.0 / total_rx,
               stats[i].rxp,
               tx_buf,
               stats[i].tx * 100.0 / total_tx,
               stats[i].txp);
    }

    format_bytes(total_rx, buf, sizeof(buf));
    char total_rx_buf[32]; snprintf(total_rx_buf, sizeof(total_rx_buf), "%s", buf);
    format_bytes(total_tx, buf, sizeof(buf));
    char total_tx_buf[32]; snprintf(total_tx_buf, sizeof(total_tx_buf), "%s", buf);

    printf("+------------+------------+--------+------------+------------+--------+------------+\n");
    printf("| %-10s | %10s | %-6.2f | %10llu | %10s | %-6.2f | %10llu |\n",
           "TOTAL",
           total_rx_buf, 100.0, total_rxp,
           total_tx_buf, 100.0, total_txp);
    printf("+------------+------------+--------+------------+------------+--------+------------+\n");

    pause_menu();
}

// Running processes
void process_list() {
    DIR *proc = opendir("/proc");
    if (!proc) {
        perror("opendir /proc");
        return;
    }

    struct dirent *de;
    char pname[BUF_SIZE]; // process name buffer
    int count = 0;

    printf("-----------------\n");

    while ((de = readdir(proc)) != NULL) {
        int is_pid = 1;
        for (int i = 0; de->d_name[i]; i++) {
            if (de->d_name[i] < '0' || de->d_name[i] > '9') {
                is_pid = 0;
                break;
            }
        }
        if (!is_pid) continue;

        char path[BUF_SIZE];
        snprintf(path, sizeof(path), "/proc/%s/comm", de->d_name);

        FILE *fp = fopen(path, "r");
        if (!fp) continue;
        if (fgets(pname, sizeof(pname), fp) == NULL) {
            fclose(fp);
            continue;
        }
        fclose(fp);

        pname[strcspn(pname, "\n")] = 0;

        printf("%-20s", pname);
        count++;
        if (count % 4 == 0) printf("\n");
    }

    if (count % 4 != 0) printf("\n");

    closedir(proc);

    printf("\nPress ENTER to return to menu...");
    getchar();
}

// System health
void system_health() {
    FILE *fp;
    char buf[BUF_SIZE];

    clear_screen();
    printf("System Health\n-------------\n");

    fp = fopen("/proc/loadavg", "r");
    if (fp) {
        unsigned int running, total, lastpid;
        double l1, l5, l15;
        if (fscanf(fp, "%lf %lf %lf %u/%u %u",
                   &l1, &l5, &l15, &running, &total, &lastpid) == 6) {
            printf("Load Average   : %.2f (1 min), %.2f (5 min), %.2f (15 min)\n", l1, l5, l15);
            printf("Processes      : %u running of %u total\n", running, total);
            printf("Last PID       : %u\n", lastpid);
        } else {
            printf("Could not parse /proc/loadavg\n");
        }
        fclose(fp);
    }

    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        printf("\nMemory Info:\n");
        while (fgets(buf, sizeof(buf), fp)) {
            char key[32]; unsigned long long val;
            if (sscanf(buf, "%31[^:]: %llu", key, &val) == 2) {
                char mem_buf[32];
                format_bytes(val, mem_buf, sizeof(mem_buf));
                printf("%-12s : %s\n", key, mem_buf);
            }
        }
        fclose(fp);
    }

    pause_menu();
}

// List /dev devices
void dev_list() {
    DIR *d;
    struct dirent *de;
    clear_screen();
    printf("/dev Devices\n-----------\n");

    d = opendir("/dev");
    if (!d) {
        perror("opendir");
        pause_menu();
        return;
    }

    int count = 0;
    while ((de = readdir(d))) {
        printf("%-20s", de->d_name);
        if (++count % 4 == 0) printf("\n");
    }
    if (count % 4 != 0) printf("\n");

    closedir(d);
    pause_menu();
}

// List disks (/proc/partitions)
void list_disks() {
    FILE *fp;
    char line[BUF_SIZE];
    char major[16], minor[16], blocks[32], name[128];

    clear_screen();
    printf("Disks (/proc/partitions):\n");
    printf("%-7s %-7s %-10s %-10s\n", "major", "minor", "#blocks", "name");

    fp = fopen("/proc/partitions", "r");
    if (!fp) {
        perror("fopen");
        pause_menu();
        return;
    }

    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, " %15s %15s %31s %127s", major, minor, blocks, name) == 4) {
            char blocks_buf[32];
            format_bytes(strtoull(blocks, NULL, 10)*1024, blocks_buf, sizeof(blocks_buf)); // assuming blocks = 1KB
            printf("%-7s %-7s %-10s %s\n", major, minor, blocks_buf, name);
        }
    }

    fclose(fp);
    pause_menu();
}

int main() {
    int choice;
    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        getchar();
        switch (choice) {
            case 1: system_info(); break;
            case 2: hardware_info(); break;
            case 3: network_stats(); break;
            case 4: process_list(); break;
            case 5: system_health(); break;
            case 6: dev_list(); break;
            case 7: list_disks(); break;
            case 8: exit(0);
            default: break;
        }
    }
    return 0;
}
