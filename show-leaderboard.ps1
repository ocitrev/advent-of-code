#requires -version 7
[cmdletbinding()]
param(
    [int]$Year=(Get-Date).Year,
    [int]$Top=3
)

python (Join-Path $PSScriptRoot inputs leaderboard.py) --year $Year --top $Top
