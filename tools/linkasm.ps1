# Usage:
# .\linkasm.ps1 input output

$symbols = @()
$externs = @()

# Find all public & extern'd symbols
Get-Content $args[0] | Where-Object {$_ -match '^\s*public\s([^\s]+)\s*(;.*)?$'} | ForEach-Object {
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
    if ($line -match '^\s*extern\s([^\s]+)\s*(;.*)?$') {
        if ($Matches[1] -in $symbols -or $Matches[1] -in $externs) {
            continue
        }

        $externs += $Matches[1]
    }
    
    Add-Content -Path $args[1] -Value $line
}
