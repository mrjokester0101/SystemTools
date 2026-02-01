# SystemTools

# Windows Documentation
**Standalone CLI System Monitoring and Information Utility for Windows**
* Binary located on `Download/Windows/SystemTools.exe`
* Source code located on `Source Codes/Windows/SystemTools.ps1`
* A seperate documentation for Windows located on `Documents/Windows.md`

## 1. Application Type
* **Type:** Command-Line Interface (CLI) tool  
* **Platform:** Windows (x86/x64)  
* **Binary:** Single executable `.exe` (compiled from PowerShell script or C# wrapper)  
* **Dependencies:** None at runtime — uses built-in Windows APIs and PowerShell cmdlets internally  
* **GUI:** None, terminal-based  

## 2. Purpose / Functionality

`SystemTools` is a lightweight, all-in-one system monitoring utility for Windows, combining features similar to Task Manager, `systeminfo`, `Get-Process`, `Get-NetAdapter`, and disk monitoring commands.  

### Features
| Menu Option                | Functionality                                                                                   |
|-----------------------------|------------------------------------------------------------------------------------------------|
| 1. Show Full System Information       | Displays OS details, computer name, system manufacturer, memory, and other systeminfo data. |
| 2. Display Hardware Details          | Shows CPU model, cores, clock speed, installed RAM modules, GPU information, and disk devices. |
| 3. Show Installed Software           | Lists all installed programs with name and version using Win32_Product. |
| 4. Display Network Information       | Shows IP addresses, MAC addresses, and open TCP ports on the system. |
| 5. Show Running Processes            | Displays top 20 CPU-consuming processes with process ID, CPU usage, and memory usage. |
| 6. Monitor System Health             | Shows current CPU load, RAM usage, disk free space per drive, and total free/used disk space. |
| 7. Exit                              | Exit the program. |

## 3. How It Works Internally
* **Compiled Script:** The .ps1 script is converted into a Windows executable using tools like:
  * PS2EXE (PowerShell script to .exe)  
  * PowerShell + .NET C# wrapper using System.Management.Automation  
* **System Queries:** Uses built-in Windows cmdlets (Get-CimInstance, Get-WmiObject, Get-Process, Get-NetAdapter, Get-NetIPAddress, Get-NetTCPConnection) to fetch system data.  
* **Formatting:** Uses console formatting to present aligned tables and readable output.  
* **Menu Loop:** Runs an infinite loop prompting the user for input; executes the selected function and allows return to menu.  
* **Refresh Option:** Pressing any key refreshes the display, pressing Enter returns to the main menu.  

## 4. Compilation
To create an .exe from the PowerShell script:  

### Option 1: Using PS2EXE (Recommended) (TESTED)
Install PS2EXE module and compile:
```
Install-Module -Name PS2EXE -Scope CurrentUser
Invoke-PS2EXE .\SystemTools-Source.ps1 .\SystemTools.exe -noConsole
```
### Option 2: Using PowerShell + C# wrapper (NOT TESTED)
* Wrap the script in a PowerShell object inside a C# console project  
* Compile with Visual Studio or dotnet publish  
* Output: single .exe executable  

**Result:** Single, portable executable that runs on any modern Windows system without needing PowerShell to be manually invoked.

## 5. Runtime Environment
* **OS:** Windows 7/8/10/11 (x86/x64)  
* **Executable:** Runs directly from terminal, no PowerShell required at runtime  
* **Privileges:** Some queries (like Win32_Product) may require Administrator privileges to show full results  
* **Terminal-based:** No GUI required  

## 6. Benefits
* Lightweight, all-in-one system utility  
* Fully standalone .exe → portable across Windows machines  
* Provides CPU, RAM, disk, network, process, and installed software info in a single tool  
* Menu-driven interface with refreshable display  

## 7. Limitations  
* CPU and RAM monitoring is not real-time; display refreshes only on user action.  
* Some commands (Win32_Product) can be slow depending on system.  
* Disk space calculations are per logical drive; does not show advanced RAID info.  
* Open port listing may miss ports blocked by firewall or with insufficient privileges.  

## 8. Notes
* Fully functional on any modern Windows system.  
* Best suited for quick diagnostics, troubleshooting, and system monitoring in CLI environments.  
* Can be extended to include additional system checks such as event logs, services, or performance counters.  

---

# Linux Documentation
**Standalone CLI System Monitoring and Information Utility for Linux**
* Binary located on `Download/Linux/SystemTools`
* Source code located on `Source Codes/Linux/SystemTools.c`
* A seperate documentation for Windows located on `Documents/Linux.md`

## 1. Application Type
* **Type:** Command-Line Interface (CLI) tool  
* **Platform:** Linux  
* **Binary:** Statically linked, single executable  
* **Size Optimization:** Compiled with `-Os` (optimize for size) and `-s` (strip symbols)  
* **Dependencies:** None at runtime (reads directly from `/proc`)  

## 2. Purpose / Functionality

`SystemTools` is a lightweight, all-in-one system monitoring utility combining features of top, htop, free, ls /dev, cat /proc/net/dev, and lsblk.  

### Features
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

## 3. How It Works Internally
* **No external binaries:** Reads directly from /proc (cpuinfo, meminfo, net/dev, /proc/[pid]/comm, partitions) → works on minimal Linux installations.  
* **Tables and formatting:** Uses printf for aligned columns and converts byte counts into human-readable units. Percentages are calculated from totals.  
* **Portable:** Fully static → runs on any Linux distro, even without standard tools installed.  

## 4. Compilation
```
gcc SystemTools.c -static -Os -s -o SystemTools
sudo chmod 755 SystemTools
```
**gcc options explained:**
  * `-static` → produce a statically linked binary (no shared library dependencies)
  * `-Os` → optimize for size rather than speed
  * `-s` → strip all symbols (removes debugging info, smaller binary)
  * `-o` → specify the output binary name

**chmod explained:**
* chmod 755 SystemTools → sets the permissions of the SystemTools binary:
  - `7` → owner can read, write, execute (4+2+1)
  - `5` → group can read and execute (4+0+1)
  - `5` → others can read and execute (4+0+1)
* `sudo` → runs the command as root, needed if your user doesn’t have permission to change the file mode
* This makes the binary executable by everyone, but only writable by the owner.

**Result:** Single, portable binary, that runs on every distro.  

## 5. Install System-Wide (Recommended) (Optional)
```
sudo cp /path/to/SystemTools /usr/local/bin/SystemTools
sudo cp /path/to/SystemTools /usr/local/bin/systemtools
sudo cp /path/to/SystemTools /usr/local/bin/st
```
**Result:** The binary can be run by executing SystemTools, systemtools or st.

## 6. Runtime Environment
* **Linux Kernel:** >= 2.6
* **Terminal-based:** No GUI required
* **Dependencies:** Only the Linux kernel and /proc filesystem
* **External Programs:** None needed

## 7. Benefits
* Lightweight, all-in-one system tool
* Fully static → portable across distros
* Readable output
* Works in minimal Linux environments (LFS, WSL, rescue systems)

## 8. Limitations  
* Network stats only show /proc/net/dev interfaces  
* Not real-time: must re-run menu options to refresh data  
* CPU cores: hyperthreaded cores are counted as separate cores. 

## 9. Notes
* Fully functional on minimal Linux systems (no GUI, no extra packages).  
* Best suited for quick diagnostics, monitoring, and portable troubleshooting.  

## 10. Known Issues
### Permission Issue (Binary & Compiling)
* Sometimes the permissions are wrong and may show something different, so you'll have to run the command below to fix it:
```
sudo chmod 755 /path/to/SystemTools
```

### GCC Not Installed (Compiling)
* Sometimes the distro your currently using doesn't have the gcc tool.
* Make sure to install it with the package manager (assuming you have network) or install it manually.
* If any other issue arises, please make sure to check the documents of the distro your using and follow the steps to fix it and then install gcc.

### No Other Issues Reported
* If there is another issue you may be experiencing, open a new issue. and make sure to state your distro, what you did with a screenshot & kernel version.
* If you have edited anything in the binary or source code and followed the steps, report it in the discord server, the invite is in the README.md5.

---

> [!NOTE]
> I'm enjoying learning more about scripting/programming and learning as I go.<br>
> I'm a person who enjoys working alone so don't take it personally if i don't accept pull requests.

## Community
[![Discord](https://img.shields.io/badge/Join-Discord%20Community-5865F2?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/VWEcYvKztc)
