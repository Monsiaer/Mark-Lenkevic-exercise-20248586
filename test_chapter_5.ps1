param([switch]$SkipBuild)

$ErrorActionPreference = 'Stop'
if (-not $SkipBuild) { & "$PSScriptRoot\build.ps1" }
$script:checks = 0

function Run-Program([string]$Name, [string]$Text = '', [string[]]$Arguments = @()) {
    $info = New-Object System.Diagnostics.ProcessStartInfo
    $info.FileName = Join-Path $PSScriptRoot "bin\$Name.exe"
    $info.UseShellExecute = $false
    $info.CreateNoWindow = $true
    $info.RedirectStandardInput = $true
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    $info.Arguments = ($Arguments | ForEach-Object { '"' + $_ + '"' }) -join ' '
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $info
    [void]$process.Start()
    $stdout = $process.StandardOutput.ReadToEndAsync()
    $stderr = $process.StandardError.ReadToEndAsync()
    $process.StandardInput.Write($Text)
    $process.StandardInput.Close()
    if (-not $process.WaitForExit(10000)) {
        $process.Kill()
        throw "Timeout: $Name"
    }
    $result = [pscustomobject]@{
        Output = $stdout.Result.Replace("`r`n", "`n")
        Error = $stderr.Result.Replace("`r`n", "`n")
        Code = $process.ExitCode
    }
    $process.Dispose()
    return $result
}

function Check($Actual, $Expected, [string]$Label) {
    if ($Actual -cne $Expected) { throw "$Label expected <$Expected>, got <$Actual>" }
    $script:checks++
}

function Test-Output([string]$Name, [string]$Text, [string]$Expected) {
    $result = Run-Program $Name $Text
    Check $result.Code 0 "$Name exit"
    Check $result.Output $Expected "$Name output"
}

Test-Output 'exercise_5_03' "hello `nworld`n" "hello world`n"
Test-Output 'exercise_5_03' "`n`n" "`n"
Test-Output 'exercise_5_03' "abc`ndef" "abcdef`n"
Test-Output 'exercise_5_03' (('a' * 999) + "`n" + ('b' * 999) + "`n") (('a' * 999) + ('b' * 999) + "`n")
Check (Run-Program 'exercise_5_03' (('a' * 1000) + "`nb`n")).Code 1 'Reject long first line'
Check (Run-Program 'exercise_5_03' "a`n").Code 1 'Reject missing second line'
Test-Output 'exercise_5_04' "hello world`nworld`n" "1`n"
Test-Output 'exercise_5_04' "hello world`nhello`n" "0`n"
Test-Output 'exercise_5_04' "abc`nabcd`n" "0`n"
Test-Output 'exercise_5_04' "`n`n" "1`n"
Test-Output 'exercise_5_04' "abc`n`n" "1`n"
Test-Output 'exercise_5_04' "abc`nABC`n" "0`n"
Check (Run-Program 'exercise_5_04' '').Code 1 'Reject missing strings'
Test-Output 'exercise_5_07' "zebra`n`napple`nzebra" "`napple`nzebra`nzebra`n"
Test-Output 'exercise_5_07' '' ''
Test-Output 'exercise_5_07' 'last' "last`n"
Test-Output 'exercise_5_07' (('x' * 999) + "`n") (('x' * 999) + "`n")
Check (Run-Program 'exercise_5_07' ('x' * 1000)).Code 1 'Reject oversized line'
Check (Run-Program 'exercise_5_07' ((('x' * 999) + "`n") * 11)).Code 1 'Reject storage exhaustion'
Test-Output 'exercise_5_07' ((('x' * 999) + "`n") * 10) ((('x' * 999) + "`n") * 10)
Test-Output 'exercise_5_07' ("`n" * 5000) ("`n" * 5000)
Check (Run-Program 'exercise_5_07' ("`n" * 5001)).Code 1 'Reject too many lines'
Test-Output 'exercise_5_trace' '' "15 15`n10 10`n"
Test-Output 'exercise_5_08' '' "day_of_year(1988, 2, 29) = 60`nday_of_year(2020, 2, 31) = -3 (invalid day)`nmonth_day(1988, 60): status=0, month=2, day=29`nmonth_day(1999, 366): status=-4, month=0, day=0`n"

$valid = @(
    @{ Tokens = @('2', '3', '4', '+', '*'); Expected = "14`n" },
    @{ Tokens = @('10', '3', '-'); Expected = "7`n" },
    @{ Tokens = @('7', '2', '/'); Expected = "3.5`n" },
    @{ Tokens = @('7', '2', '%'); Expected = "1`n" },
    @{ Tokens = @('-3', '2.5', '*'); Expected = "-7.5`n" },
    @{ Tokens = @('1e2', '25', '/'); Expected = "4`n" },
    @{ Tokens = @('42'); Expected = "42`n" }
)
foreach ($case in $valid) {
    $result = Run-Program 'exercise_5_10' '' $case.Tokens
    Check $result.Code 0 'Calculator success'
    Check $result.Output $case.Expected 'Calculator value'
}
$invalid = @(
    @{ Tokens = @() }, @{ Tokens = @('2', '+') }, @{ Tokens = @('2', '3') },
    @{ Tokens = @('2', '0', '/') }, @{ Tokens = @('2', '0', '%') },
    @{ Tokens = @('abc') }, @{ Tokens = @('2x') }, @{ Tokens = @('nan') },
    @{ Tokens = @('inf') }, @{ Tokens = @('1e999') }, @{ Tokens = @('') },
    @{ Tokens = @('1e308', '1e308', '*') }, @{ Tokens = @('2', '3', '^') }
)
foreach ($case in $invalid) {
    $result = Run-Program 'exercise_5_10' '' $case.Tokens
    Check $result.Code 1 'Calculator rejects invalid expression'
    Check ($result.Error.Length -gt 0) $true 'Calculator reports reason'
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$vcvars = Join-Path $installation 'VC\Auxiliary\Build\vcvars64.bat'
$commands = @('@echo off', ('call "{0}" >nul' -f $vcvars), 'if errorlevel 1 exit /b 1')
foreach ($name in @('test_chapter_5', 'benchmark_5_07', 'test_chapter_5_extra')) {
    $commands += 'cl /nologo /TC /std:c17 /W4 /WX /O2 "{0}\tests\{1}.c" /Fo"{0}\obj\{1}.obj" /Fe"{0}\bin\{1}.exe" /link /INCREMENTAL:NO' -f $PSScriptRoot, $name
    $commands += 'if errorlevel 1 exit /b 1'
}
$commands += 'exit /b 0'
$batch = Join-Path $PSScriptRoot 'obj\test_chapter_5.cmd'
Set-Content -LiteralPath $batch -Value $commands -Encoding ascii
& $env:ComSpec /d /c "`"$batch`""
if ($LASTEXITCODE -ne 0) { throw 'Chapter 5 test compilation failed.' }
Test-Output 'test_chapter_5' '' "Chapter 5 function checks passed; 146097 calendar dates checked.`n"
Test-Output 'test_chapter_5_extra' '' "Exercises 5-5 and 5-6 checks passed.`n"
Write-Host "PASS: $script:checks chapter 5 process assertions and function checks (including a 400-year calendar cycle)."
