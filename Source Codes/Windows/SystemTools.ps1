function Show-Menu {
    Clear-Host
    Write-Host "================================"
    Write-Host "          SYSTEM TOOLS"
    Write-Host "================================"
    Write-Host "1. Show Full System Information"
    Write-Host "2. Display Hardware Details"
    Write-Host "3. Show Installed Software"
    Write-Host "4. Display Network Information"
    Write-Host "5. Show Running Processes"
    Write-Host "6. Monitor System Health"
    Write-Host "7. Exit"
}

function Enter {
    Write-Host "`nPress Enter to exit to menu or any key to refresh."

    $key = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    if ($key.VirtualKeyCode -eq 13) {
        Clear-Host

        break
    }
}

function Show-SystemInfo {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "====== SYSTEM INFORMATION ======"
        systeminfo | Out-String
    
        Enter
    }
}

function Show-HardwareInfo {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "======= HARDWARE DETAILS ======="
        Write-Host "`nCPU INFO:"
        Get-CimInstance Win32_Processor | Select-Object Name, MaxClockSpeed, NumberOfCores | Format-Table -AutoSize
        Write-Host "`nMEMORY INFO:"
        Get-CimInstance Win32_PhysicalMemory | Select-Object Manufacturer, Capacity | Format-Table -AutoSize
        Write-Host "`nGPU INFO:"
        Get-CimInstance Win32_VideoController | Select-Object Name, AdapterRAM | Format-Table -AutoSize
        Write-Host "`nDISK INFO:"
        Get-PhysicalDisk | Select-Object DeviceID, MediaType, Size | Format-Table -AutoSize
    
        Enter
    }
}

function Show-InstalledSoftware {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "====== INSTALLED SOFTWARE ======"
        Get-WmiObject -Query "SELECT * FROM Win32_Product" | Select-Object Name, Version | Format-Table -AutoSize
    
        Enter
    }
}

function Show-NetworkInfo {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "===== INTERNET INFORMATION ====="
        Write-Host "`nIP ADDRESSES:"
        Get-NetIPAddress | Select-Object InterfaceAlias, IPAddress | Format-Table -AutoSize
        Write-Host "`nMAC ADDRESSES:"
        Get-NetAdapter | Select-Object Name, MacAddress | Format-Table -AutoSize
        Write-Host "`nOPEN PORTS:"
        Get-NetTCPConnection | Where-Object { $_.State -eq "Listen" } | Select-Object LocalAddress, LocalPort | Format-Table -AutoSize
    
        Enter
    }
}

function Show-RunningProcesses {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "===== RUNNING APPLICATIONS ====="
        Get-Process | Sort-Object -Property CPU -Descending | Select-Object -First 20 | Format-Table Name, Id, CPU, WS -AutoSize
        
        Enter
    }
}

function Monitor-SystemHealth {
    Clear-Host

    while ($true) {
        Show-Menu
        Write-Host "======== SYSTEM MONITOR ========"
        
        $cpu = Get-WmiObject Win32_Processor | Measure-Object -Property LoadPercentage -Average | Select-Object -ExpandProperty Average
        $ram = Get-CimInstance Win32_OperatingSystem | Select-Object @{N="Free RAM (GB)"; E={[math]::Round($_.FreePhysicalMemory / 1MB,2)}}, @{N="Total RAM (GB)"; E={[math]::Round($_.TotalVisibleMemorySize / 1MB,2)}}
        
        $disks = Get-CimInstance Win32_LogicalDisk | Where-Object { $_.DriveType -eq 3 } | 
        Select-Object DeviceID, @{N="Free"; E={[math]::Round($_.FreeSpace / 1GB,2)}}, @{N="Total"; E={[math]::Round($_.Size / 1GB,2)}}

        $totalFree = ($disks | Measure-Object -Property Free -Sum).Sum
        $totalSize = ($disks | Measure-Object -Property Total -Sum).Sum

        Write-Host "`nCPU Usage: $cpu %"
        Write-Host "RAM: $($ram.'Free RAM (GB)') GB free / $($ram.'Total RAM (GB)') GB total"

        Write-Host "`nDisk Space:"
        Write-Host "    Total Disk Space: $totalFree GB free / $totalSize GB total"
        foreach ($disk in $disks) {
            Write-Host "    Drive $($disk.DeviceID): $($disk.Free) GB free / $($disk.Total) GB total"
        }

        Enter
    }
}

do {
    Clear-Host
    Show-Menu
    Write-Host "================================"
    $choice = Read-Host "Enter your choice (1-7)"

    switch ($choice) {
        1 { Show-SystemInfo }
        2 { Show-HardwareInfo }
        3 { Show-InstalledSoftware }
        4 { Show-NetworkInfo }
        5 { Show-RunningProcesses }
        6 { Monitor-SystemHealth }
        7 { Write-Host "Exiting..."; Clear-History; Start-Sleep 1; exit }
        default { Write-Host "Invalid number!" }
    }
} while ($true)