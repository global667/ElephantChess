# PowerShell-Skript für das Deployen von ElephantChess unter Windows 11

# Pfad zur Qt-Installation
$QtDir = "C:\Qt\6.7.0\msvc2019_64"
# Projet Verzeichnis
$PrjDir = "F:\source\XiangQi\ElephantChess"
# Resourcen Verzeichnis
$ResDir = $PrjDir + "\res"
# Pfad zum Build-Verzeichnis Ihres Projekts
$BuildDir = "F:\source\XiangQi\ElephantChess\out\build"
# Zielverzeichnis für das Deployment
$DeployDir = "F:\source\XiangQi\ElephantChess\out\deploy"
# Installer Directory
$InstallerDir = "F:\source\XiangQi\ElephantChess\installer"
# Name Ihrer ausführbaren Datei
$ExeVersion = "_v0.4.1"
$ExeName = "ElephantChess.exe"
$ExeNameVersioned = "ElephantChess" + $ExeVersion + ".exe"
# Name des/der Schachprogramms/Schachprogramme
$PikaExe = "pikafish.exe"
$PikaNet = "pikafish.nnue"

# Schritt 1: Kompilieren des Projekts (optional, falls nicht bereits geschehen)
# cd zum Projektverzeichnis, falls nötig
# Invoke-Expression "qmake && mingw32-make"

# Schritt 2: Erstellen des Deploy-Verzeichnisses, aber auf jeden Fall löschen
if (Test-Path $DeployDir) {
    Remove-Item -Path $DeployDir -Recurse
}
New-Item -ItemType Directory -Force -Path $DeployDir

# Schritt 3: Verwenden von windeployqt zum Sammeln der Abhängigkeiten
$QtBinDir = Join-Path -Path $QtDir -ChildPath "bin"
$WinDeployQt = Join-Path -Path $QtBinDir -ChildPath "windeployqt.exe"
$ExeDir = Join-Path -Path $BuildDir -ChildPath $ExeName
$PikaExePath = Join-Path -Path $ResDir -ChildPath $PikaExe
$PikaNetPath = Join-Path -Path $ResDir -ChildPath $PikaNet

# Schritt 4: Kopieren der ausführbaren Datei und anderer benötigter Dateien in das Deploy-Verzeichnis
# Exe umbenennen
Copy-Item -Path $ExeDir -Destination $DeployDir\$ExeNameVersioned
Copy-Item -Path $PikaExePath -Destination $DeployDir
Copy-Item -Path $PikaNetPath -Destination $DeployDir

# Führen Sie windeployqt aus, um die erforderlichen Abhängigkeiten zu kopieren
& $WinDeployQt --debug --no-translations --no-opengl-sw --no-system-d3d-compiler --no-compiler-runtime --qmldir $QtDir\qml $DeployDir\$ExeNameVersioned $DeployDir

# Deployment abgeschlossen
Write-Host "Geschrieben in Verzeichnis: $DeployDir"

$DataDir = $InstallerDir + "\packages\com.boringgames.elephantchess\data"
if (Test-Path $DataDir) {
    Remove-Item -Path $DataDir -Recurse
}
New-Item -ItemType Directory -Force -Path $DataDir

Copy-Item  -Path $DeployDir\* -Destination $DataDir -Recurse -Force

Write-Host "Binary Creator läuft..."
C:\Qt\Tools\QtInstallerFramework\4.7\bin\binarycreator.exe -c $InstallerDir"\config\config.xml" -p $InstallerDir"\packages" $InstallerDir\ElephantChess_v0.4.1.exe

Write-Host "Installationsfile unter: $InstallerDir"   
Write-Host ".: End :."