param(
    [ValidatePattern('^(all|exercise_[12]_[0-9]{2})$')]
    [string]$Exercise = 'all'
)

$ErrorActionPreference = 'Stop'
$sources = @(Get-ChildItem -Path "$PSScriptRoot\chapter_*\*.c" | Sort-Object Name)
if ($Exercise -ne 'all') {
    $sources = @($sources | Where-Object BaseName -eq $Exercise)
}
if ($sources.Count -eq 0) { throw "Exercise not found: $Exercise" }

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) { throw 'Visual Studio Installer was not found.' }
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $installation) { throw 'Install the Visual Studio Desktop development with C++ workload.' }
$vcvars = Join-Path $installation 'VC\Auxiliary\Build\vcvars64.bat'
$bin = Join-Path $PSScriptRoot 'bin'
$obj = Join-Path $PSScriptRoot 'obj'
New-Item -ItemType Directory -Force -Path $bin, $obj | Out-Null

# Keep command generation local; paths are quoted, and the selector is validated.
$commands = @('@echo off', ('call "{0}" >nul' -f $vcvars), 'if errorlevel 1 exit /b 1')
foreach ($source in $sources) {
    $commands += 'echo Building {0}' -f $source.BaseName
    $commands += 'cl /nologo /TC /std:c17 /W4 /WX "{0}" /Fo"{1}\{2}.obj" /Fe"{3}\{2}.exe" /link /INCREMENTAL:NO' -f $source.FullName, $obj, $source.BaseName, $bin
    $commands += 'if errorlevel 1 exit /b 1'
}
$commands += 'exit /b 0'
$batch = Join-Path $obj 'build.cmd'
Set-Content -LiteralPath $batch -Value $commands -Encoding ascii
& $env:ComSpec /d /c "`"$batch`""
if ($LASTEXITCODE -ne 0) { throw 'Compilation failed.' }
Write-Host "Built $($sources.Count) exercise(s). Executables: $bin"
