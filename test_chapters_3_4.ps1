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

Test-Output 'exercise_3_02' '' "Escaped string: Hello\tworld\nThis is a test.`nUnescaped string: Hello`tworld`nThis is a test.`n"
Test-Output 'exercise_4_07' "first`nsecond" "first`nsecond"
Test-Output 'exercise_4_07' '' ''
Test-Output 'exercise_4_08' "a`t b`n" "a`t b`n"
Test-Output 'exercise_4_09' 'abc' "pushed EOF: preserved`nabc"

foreach ($name in @('exercise_4_03','exercise_4_04','exercise_4_05','exercise_4_06','exercise_4_10')) {
    Test-Output $name "1 2 - 4 5 + *`n-7 3 %`n7 -3 %`n-.5 2 *" "-9`n-1`n1`n-1`n"
    Test-Output $name "1 0 /`n2 3 +`n" "error: zero divisor`n5`n"
    Test-Output $name "+`n2 3`n.`n" "error: not enough operands`nerror: too many operands`nerror: invalid token`n"
    Test-Output $name '' ''
    Test-Output $name (('1 ' * 101) + "`n2 2 +`n") "error: stack full`n4`n"
}
foreach ($name in @('exercise_4_04','exercise_4_05','exercise_4_06','exercise_4_10')) {
    Test-Output $name "3 p d +`n2 7 s -`n1 2 c 9`n" "3`n6`n5`n9`n"
}
foreach ($name in @('exercise_4_05','exercise_4_06')) {
    Test-Output $name "0 sin`n0 exp`n2 3 pow`n9 .5 pow`n-2 3 pow`n" "0`n1`n8`n3`n-8`n"
    Test-Output $name "sin`n-1 .5 pow`n1000 exp`n3 2 +`n" "error: not enough operands`nerror: math domain or range`nerror: math domain or range`n5`n"
    Test-Output $name "unknown`n" "error: invalid token`n"
    Test-Output $name (('x' * 200) + "`n2 2 +`n") "error: invalid token`n4`n"
}
Test-Output 'exercise_4_06' "5 A =`nA 2 *`nv 1 +`n3 Z =`nZ A +`n" "5`n10`n11`n3`n8`n"
Test-Output 'exercise_4_06' "v`n1 =`n5 A + =`n2 2 +`n" "0`nerror: use value A = to assign`nerror: use value A = to assign`n4`n"
Test-Output 'exercise_4_06' "7 p c`nv`n" "7`n7`n"
Test-Output 'exercise_4_10' (('1 ' * 3000) + "`n2 3 +`n") "error: invalid token`n5`n"
Test-Output 'exercise_4_10' ((' ' * 4094) + "1`n2 3 +`n") "1`n5`n"
Test-Output 'exercise_4_10' "2 3 +   " "5`n"
Test-Output 'example_control_flow' '' "block: *********`nnested if: 8`nbinary search 9: 5`nswitch: digit 1=1, whitespace=3, other=1`nbreak: |control flow|`ncontinue: 7`ngoto: common=6`n"
Test-Output 'example_scope_rules' '' "extern shared count: 2`nblock: 20, enclosing: 10`nstatic lifetime: 3 4`n"
Test-Output 'example_preprocessor' '' "maximum: 7`naverage: 5.5`nHello world`ntemporary macro is undefined`n"
Test-Output 'example_postfix_calculator' "1 2 - 4 5 + *`n" "`t-9`n"
$result = Run-Program 'example_particular_pattern' "could`nhello`nwould`n"
Check $result.Output "could`nwould`n" 'Pattern example output'
Check $result.Code 2 'Pattern example match count'
$result = Run-Program 'exercise_4_01' "could`nhello`nwould`n"
Check $result.Output "could`nwould`n" 'Rightmost search program output'
Check $result.Code 2 'Rightmost search program match count'

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$vcvars = Join-Path $installation 'VC\Auxiliary\Build\vcvars64.bat'
$commands = @('@echo off', ('call "{0}" >nul' -f $vcvars), 'if errorlevel 1 exit /b 1')
$commands += 'cl /nologo /TC /std:c17 /W4 /WX "{0}\tests\test_chapter_3.c" /Fo"{0}\obj\test_chapter_3.obj" /Fe"{0}\bin\test_chapter_3.exe" /link /INCREMENTAL:NO' -f $PSScriptRoot
$commands += 'if errorlevel 1 exit /b 1'
foreach ($number in @(7,8,9)) {
    $commands += 'cl /nologo /TC /std:c17 /W4 /WX /DTEST_EXERCISE={1} "{0}\tests\test_pushback.c" /Fo"{0}\obj\test_pushback_{1}.obj" /Fe"{0}\bin\test_pushback_{1}.exe" /link /INCREMENTAL:NO' -f $PSScriptRoot,$number
    $commands += 'if errorlevel 1 exit /b 1'
}
$commands += 'cl /nologo /TC /std:c17 /W4 /WX /DEXAMPLE_DEBUG /DEXAMPLE_NATION=3 "{0}\examples\preprocessor.c" /Fo"{0}\obj\preprocessor_alternative.obj" /Fe"{0}\bin\preprocessor_alternative.exe" /link /INCREMENTAL:NO' -f $PSScriptRoot
$commands += 'if errorlevel 1 exit /b 1'
$commands += 'exit /b 0'
$batch = Join-Path $PSScriptRoot 'obj\test_chapters_3_4.cmd'
Set-Content -LiteralPath $batch -Value $commands -Encoding ascii
& $env:ComSpec /d /c "`"$batch`""
if ($LASTEXITCODE -ne 0) { throw 'Chapter 3/4 test compilation failed.' }
Test-Output 'test_chapter_3' '' "Chapter 3 and strrindex function checks passed.`n"
foreach ($number in @(7,8,9)) {
    Test-Output "test_pushback_$number" '' "Pushback checks passed.`n"
}
Test-Output 'preprocessor_alternative' '' "debug: x=3, y=8`nmaximum: 7`naverage: 5.5`nother language`ntemporary macro is undefined`n"
Write-Host "PASS: $script:checks chapter 3/4 process assertions, plus conversion, search, and pushback function checks."
