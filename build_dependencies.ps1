param (
    [string]$buildMode = "Debug"
)

if ($buildMode -eq "Debug" -or $buildMode -eq "Release") {
    Write-Output "Build mode is $buildMode"
} else {
    Write-Output "Build mode not set or invalid, the default is the debug mode"
    $buildMode = "Debug"
}

$installPrefix = Join-Path (Get-Location) "Windows\$buildMode"

Set-Location -Path "../hlslbuilder_modules"
Start-Process vcvars64.bat
.\build_deps $buildMode $installPrefix
Set-Location -Path "../hlslbuilder"