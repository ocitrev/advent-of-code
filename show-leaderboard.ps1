#requires -version 7
[cmdletbinding()]
param(
    [int]$Year=(Get-Date).Year
)

python (Join-Path $PSScriptRoot inputs leaderboard.py) --year $Year
