param(
    [ValidatePattern('^(all|exercise_[1-9][0-9]*_([0-9]{2}|trace)|example_[a-z_]+)$')]
    [string]$Exercise = 'all'
)

$ErrorActionPreference = 'Stop'
$sources = @(Get-ChildItem -Path "$PSScriptRoot\chapter_*\*.c" | Sort-Object Name)

$sources += @(Get-ChildItem -Path "$PSScriptRoot\chapter_*\exercise_*\main.c" | ForEach-Object {
    [pscustomobject]@{
        BaseName = $_.Directory.Name
        FullName = $_.FullName
        DirectoryName = $_.DirectoryName
    }
})

$sources += @(Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'examples') -Filter '*.c' | ForEach-Object {
    [pscustomobject]@{
        BaseName = 'example_' + $_.BaseName
        FullName = $_.FullName
        DirectoryName = $_.DirectoryName
    }
})

$sources += @(Get-ChildItem -Path "$PSScriptRoot\examples\*\main.c" | ForEach-Object {
    [pscustomobject]@{
        BaseName = 'example_' + $_.Directory.Name
        FullName = $_.FullName
        DirectoryName = $_.DirectoryName
    }
})
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
    if ((Split-Path -Leaf $source.FullName) -eq 'main.c') {
        $exerciseObj = Join-Path $obj $source.BaseName
        New-Item -ItemType Directory -Force -Path $exerciseObj | Out-Null
        $objects = @()
        foreach ($unit in Get-ChildItem -LiteralPath $source.DirectoryName -Filter '*.c') {
            $objectPath = Join-Path $exerciseObj ($unit.BaseName + '.obj')
            $objects += '"{0}"' -f $objectPath
            $commands += 'cl /nologo /TC /std:c17 /W4 /WX /c "{0}" /Fo"{1}"' -f $unit.FullName, $objectPath
            $commands += 'if errorlevel 1 exit /b 1'
        }
        $commands += 'cl /nologo {0} /Fe"{1}\{2}.exe" /link /INCREMENTAL:NO' -f ($objects -join ' '), $bin, $source.BaseName
        $commands += 'if errorlevel 1 exit /b 1'
        continue
    }

    $commands += 'cl /nologo /TC /std:c17 /W4 /WX "{0}" /Fo"{1}\{2}.obj" /Fe"{3}\{2}.exe" /link /INCREMENTAL:NO' -f $source.FullName, $obj, $source.BaseName, $bin
    $commands += 'if errorlevel 1 exit /b 1'
}
$commands += 'exit /b 0'
$batch = Join-Path $obj 'build.cmd'
Set-Content -LiteralPath $batch -Value $commands -Encoding ascii
& $env:ComSpec /d /c "`"$batch`""
if ($LASTEXITCODE -ne 0) { throw 'Compilation failed.' }
Write-Host "Built $($sources.Count) exercise(s). Executables: $bin"
