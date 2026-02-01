# SystemTools
**Standalone CLI System Monitoring and Information Utility for Linux**

## 1. Application Type
* **Type:** Command-Line Interface (CLI) tool  
* **Platform:** Linux  
* **Binary:** Statically linked, single executable  
* **Size Optimization:** Compiled with `-Os` (optimize for size) and `-s` (strip symbols)  
* **Dependencies:** None at runtime (reads directly from `/proc`)  

## 2. Purpose / Functionality

`SystemTools` is a lightweight, all-in-one system monitoring utility combining features of top, htop, free, ls /dev, cat /proc/net/dev, and lsblk.  

### Features
|-----------------------------|------------------------------------------------------------------------------------------------|
| Menu Option                | Functionality                                                                                   |
|-----------------------------|------------------------------------------------------------------------------------------------|
| 1. System Information       | Shows hostname and kernel version (via uname).                                               |
| 2. Hardware Details         | Displays CPU model, total cores, and total memory (in KB, MB, GB, TB, PB).                     |
| 3. Network Statistics       | Shows RX/TX bytes and packets per interface in human-readable format, with totals and percentages. |
| 4. Running Processes        | Lists currently running processes in multi-column layout (/proc/[pid]/comm).                 |
| 5. System Health Monitor    | Displays load averages and memory stats (total, free, available) in human-readable format.     |
| 6. List /dev Devices        | Lists device nodes in /dev, 4 per line.                                                     |
| 7. Disks                    | Shows /proc/partitions with major, minor, blocks, and device name.                           |
| 8. Exit                     | Exit the program.                                                                             |
|-----------------------------|------------------------------------------------------------------------------------------------|
---

## 3. How It Works Internally
* **No external binaries:** Reads directly from /proc (cpuinfo, meminfo, net/dev, /proc/[pid]/comm, partitions) → works on minimal Linux installations.  

* **Tables and formatting:** Uses printf for aligned columns and converts byte counts into human-readable units. Percentages are calculated from totals.  

* **Portable:** Fully static → runs on any Linux distro, even without standard tools installed.  

## 4. Compilation
```
gcc SystemTools-Source.c -static -Os -s -o SystemTools
```
* `-static` → no shared library dependencies  
* `-Os` → optimize for size  
* `-s` → strip symbols (smaller binary)  

**Result:** Single, portable binary that runs anywhere.  

## 5. Runtime Environment
* **OS:** Linux kernel >= 2.6  
* **Terminal-based:** No GUI required  
* **Dependencies:** Only the Linux kernel and /proc filesystem  
* **External Programs:** None needed  

## 6. Benefits
* Lightweight, all-in-one system tool  
* Fully static → portable across distros  
* Readable output  
* Works in minimal Linux environments (LFS, WSL, rescue systems)  

## 7. Limitations  
* Network stats only show /proc/net/dev interfaces  
* Not real-time: must re-run menu options to refresh data  
* CPU cores: hyperthreaded cores are counted as separate cores. 

## 8. Notes
* Fully functional on minimal Linux systems (no GUI, no extra packages).  
* Best suited for quick diagnostics, monitoring, and portable troubleshooting.  
