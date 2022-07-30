# Usage:
# .\linkasm.ps1 input output

$symbols = @()

# Find all public symbols
Get-Content $args[0] | Where-Object {$_ -match 'public\s([^\s]+)\s*$'} | ForEach-Object {
    $symbols += $Matches[1]
}

# Create out file
if (Test-Path $args[1]) {
    Clear-Content $args[1]
} else {
    New-Item -Path $args[1] -ItemType "file"
}

# Filter out externs of public symbols
foreach ($line in Get-Content $args[0]) { 
    if ($line -match 'extern\s([^\s]+)\s*$') {
        if ($Matches[1] -in $symbols) {
            continue
        }
    }

    Add-Content -Path $args[1] -Value $line
}
