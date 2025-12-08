#requires -version 7

[cmdletbinding(defaultparametersetname="exe")]
param(
    [parameter(mandatory=$true, parametersetname="exe", position=0)]
    [string]$Exe,

    [parameter(parametersetname="exe", position=1)]
    [parameter(mandatory=$true, parametersetname="build", position=0)]
    [string]$SourceFile,

    [parameter(parametersetname="exe", position=2)]
    [parameter(parametersetname="build", position=1)]
    [int]$LineNumber=0,

    [parameter(parametersetname="build")]
    [switch]$Build
)

set-strictmode -v latest
$ErrorActionPreference = 'Stop'

if ($PSCmdLet.ParameterSetName -eq "build")
{
    if ($SourceFile -match '\\(\d+)\\day(\d+).zig$') {
        $year = $matches[1]
        $day = $matches[2]
        .\zig-build.cmd "debug-test-$year-$day"
        return
    }

    throw "Cannot build source file $SourceFile"
}

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
        start-process remedybg.exe $args

        write-verbose 'add-breakpoint-at-file'
        $args[0] = 'add-breakpoint-at-file'
        start-process remedybg.exe $args
    }
}
