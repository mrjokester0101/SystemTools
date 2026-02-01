# SystemTools
**Standalone CLI System Monitoring and Information Utility for Windows**

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
