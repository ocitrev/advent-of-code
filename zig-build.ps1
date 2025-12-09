#requires -version 7
[cmdletbinding()]
param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$args,
    [alias('cpp')]
    [switch]$IncludeCpp
)

set-strictmode -v latest
$erroractionpreference = 'stop'

function format-args
{
    param([string[]]$arguments)

    foreach ($arg in $arguments)
    {
        if ($arg -match '[\s;"'']') {
            "'$($arg -replace "'","''")'"
        } else {
            $arg
        }
    }
}

if (-not (command zig -ea ignore)) {
    throw "Zig not installed!"
}

$zigArgs = @(
    'build'
    '--prefix', (Join-Path $PSScriptRoot build zig)
    '--cache-dir', (Join-Path $PSScriptRoot build zig .zig-cache)
)

if ($IncludeCpp) {
    $zigArgs += @('-Dlang=cpp')
}

write-verbose "zig $(format-args $zigArgs) $(format-args $args)"
zig @zigArgs @args
exit $lastexitcode
