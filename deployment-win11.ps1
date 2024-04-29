# PowerShell-Skript für das Deployen von ElephantChess unter Windows 11

# Pfad zur Qt-Installation
$QtDir =  "C:\Qt\6.7.0\msvc2019_64" # "C:\Qt\6.7.0\mingw_64"
# Pfad zum Build-Verzeichnis Ihres Projekts
$BuildDir = "C:\Users\wolfk\CLionProjects\ElephantChess\cmake-build-debug-visual-studio"
#Installer data and config root dir
$InstRoot = "F:\deploy\ElephantChess\"
# Zielverzeichnis für das Deployment
$DataDir = $InstRoot + "\packages\com.boringgames.elephantchess\data"
# Name Ihrer ausführbaren Datei
$ExeName = "ElephantChess.exe"
# Name des/der Schachprogramms/Schachprogramme
#$PikaExe = "pikafish.exe"
#$PikaNet = "pikafish.nnue"

# Schritt 1: Kompilieren des Projekts (optional, falls nicht bereits geschehen)
# cd zum Projektverzeichnis, falls nötig
# Invoke-Expression "qmake && mingw32-make"

# Schritt 2: Erstellen des Deploy-Verzeichnisses, falls es nicht existiert
if (-not (Test-Path $DataDir)) {
    New-Item -ItemType Directory -Force -Path $DataDir
}

# Schritt 3: Verwenden von windeployqt zum Sammeln der Abhängigkeiten
$QtBinDir = Join-Path -Path $QtDir -ChildPath "bin"
$WinDeployQt = Join-Path -Path $QtBinDir -ChildPath "windeployqt.exe"
$ExePath = Join-Path -Path $BuildDir -ChildPath $ExeName
#$PikaExePath = Join-Path -Path $BuildDir -ChildPath $PikaExe
#$PikaNetPath = Join-Path -Path $BuildDir -ChildPath $PikaNet

Copy-Item -Path $ExePath -Destination $DataDir

#& $WinDeployQt --dir $DeployDir $ExePath
# Führen Sie windeployqt aus, um die erforderlichen Abhängigkeiten zu kopieren
& $WinDeployQt --debug --no-translations --no-opengl-sw   $DataDir"\"$ExeName
#--no-system-d3d-compiler --qmldir $QtDir\qml --no-compiler-runtime

# Schritt 4: Kopieren der ausführbaren Datei und anderer benötigter Dateien in das Deploy-Verzeichnis

#Copy-Item -Recurse -Force -Path $BuildDir"\*" -Destination $DataDir
#Copy-Item -Path $PikaExePath -Destination $DeployDir
#Copy-Item -Path $PikaNetPath -Destination $DeployDir



# Weitere benötigte Ressourcen oder Dateien kopieren
# Copy-Item -Path "" -Destination $DeployDir
cd $InstRoot
& C:\Qt\Tools\QtInstallerFramework\4.7\bin\binarycreator.exe -f -c F:\deploy\ElephantChess\config\config.xml -p F:\deploy\ElephantChess\packages $ExeName

# Deployment abgeschlossen
Write-Host "Deployment abgeschlossen. Überprüfen Sie das Verzeichnis: $InstRoot" 
