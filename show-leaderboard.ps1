#requires -version 7
[cmdletbinding()]
param(
    $Year=$null,
    $Top=$null
)

$arguments = @()

if ($year) {
    $arguments += '--year', $Year
}

if ($top -ne $null) {
    $arguments += '--top', $Top
}

python (Join-Path $PSScriptRoot inputs leaderboard.py) @arguments
