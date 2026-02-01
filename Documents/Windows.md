# SystemTools
**Standalone CLI System Monitoring and Information Utility for Windows**
* Binary located on `Download/Windows/SystemTools.exe`
* Source code located on `Source Codes/Windows/SystemTools.ps1`

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
Install-Module -Name PS2EXE -Scope CurrentUser -Force
Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned
Import-Module ps2exe
Invoke-PS2EXE -InputFile Path\To\SystemTools.ps1 -OutputFile SystemTools.exe
```
**PowerShell commands explained:**
* install-Module -Name PS2EXE -Scope CurrentUser -Force → installs the PS2EXE module for your current user only.
  - `-Scope CurrentUser` → installs the module just for your account (no admin rights needed)
  - `-Force` → automatically overwrite existing module if present
* Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned → allows running scripts on your system.
  - `-Scope CurrentUser` → applies only to your account
  - `-ExecutionPolicy RemoteSigned` → lets you run local scripts without signing, but downloaded scripts must be signed
  - `Import-Module ps2exe` → loads the PS2EXE module so its commands can be used in this session
* ConvertTo-Exe -Script \Path\To\SystemTools.ps1 -OutputFile .\SystemTools.exe → converts your PowerShell script into a standalone executable
  - `-Script` → path to the .ps1 script you want to convert
  - `-OutputFile` → name and location of the resulting .exe

### Option 2: Using PowerShell + C# wrapper (NOT TESTED)
* Wrap the script in a PowerShell object inside a C# console project  
* Compile with Visual Studio or dotnet publish  
* Output: single .exe executable  

**Result:** Single, portable executable that runs on any modern Windows system without needing PowerShell to be manually invoked.

## 5. Install System-Wide (Recommended) (Optional)
### Option 1 (Recommended)
```
mv .\SystemTools Path\To\Command-line\Utilities\Folder
$env:Path += ";Path\To\Command-line\Utilities\Folder\SystemTools"
```
* Makes SystemTools accessible system-wide without moving it to `C:\Windows\System32`.
### Option 2
```
Copy-Item .\SystemTools.exe C:\Windows\System32\
```
* **Note:** Windows Security may flag it as a virus and delete it from System32.

**Result:** You can use SystemTools system-wide.

## 6. Runtime Environment
* **OS:** Windows 7/8/10/11 (x86/x64)  
* **Executable:** Runs directly from terminal, no PowerShell required at runtime  
* **Privileges:** Some queries (like Win32_Product) may require Administrator privileges to show full results  
* **Terminal-based:** No GUI required  

## 7. Benefits
* Lightweight, all-in-one system utility  
* Fully standalone .exe → portable across Windows machines  
* Provides CPU, RAM, disk, network, process, and installed software info in a single tool  
* Menu-driven interface with refreshable display  

## 8. Limitations  
* CPU and RAM monitoring is not real-time; display refreshes only on user action.  
* Some commands (Win32_Product) can be slow depending on system.  
* Disk space calculations are per logical drive; does not show advanced RAID info.  
* Open port listing may miss ports blocked by firewall or with insufficient privileges.  

## 9. Notes
* Fully functional on any modern Windows system.  
* Best suited for quick diagnostics, troubleshooting, and system monitoring in CLI environments.  
* Can be extended to include additional system checks such as event logs, services, or performance counters.
