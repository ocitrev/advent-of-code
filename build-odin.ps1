[cmdletbinding()]
param(
    [switch]$Run
)

$outdir = (join-path $psscriptroot build odin)
$null = mkdir $outdir -ea ignore

$odinProjects = @(
    [pscustomobject]@{Year=2019; Day=2}
)

foreach ($proj in $odinProjects)
{
    $out = join-path $outdir "$($proj.Year)-$($proj.Day).exe"
    odin build (join-path $psscriptroot src $proj.Year "day$($proj.Day).odin") -file -out:$out

    if ($Run)
    {
        & $out
    }
}
