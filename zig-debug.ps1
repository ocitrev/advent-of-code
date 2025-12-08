#requires -version 7

[cmdletbinding()]
param(
    [parameter(mandatory=$true)]
    [string]$Exe,

    [parameter(mandatory=$true)]
    [string]$sourceFile
)

set-strictmode -v latest
$ErrorActionPreference = 'Stop'

write-verbose 'launching debugger'
$p = start-process remedybg.exe @($Exe) -passThru

write-verbose 'WaitForInputIdle'
$null = $p.WaitForInputIdle()

$lineNumber = 0
foreach ($line in (get-content $sourceFile)) {
    $lineNumber++

    if ($line -cmatch '^test')
    {
        $args = @(
            'open-file'
            $sourceFile
            $lineNumber + 1
        )

        write-verbose 'open-file'
        start-process remedybg.exe $args -wait

        write-verbose 'add-breakpoint-at-file'
        $args[0] = 'add-breakpoint-at-file'
        start-process remedybg.exe $args -wait
    }
}
