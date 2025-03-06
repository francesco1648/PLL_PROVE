# build.ps1

# Imposta le variabili in base al tuo Makefile
$BOARD_FQBN = "rp2040:rp2040:rpipico"
$SKETCH_PATH = "C:\Users\Titania\Desktop\isaac\picolowlevel_x_vs\PicoLowLevel"
$BUILD_DIR = "$SKETCH_PATH\build"
$OUTPUT_DIR = "$BUILD_DIR\output"

# Comando di compilazione con flag verbose per maggior output (anche se non progressivo)
$arguments = @(
    "compile"
    "--fqbn", $BOARD_FQBN
    "--build-path", $BUILD_DIR
    $SKETCH_PATH
    "--output-dir", $OUTPUT_DIR
    "--verbose"
)

# Avvia il processo di compilazione
$process = Start-Process -FilePath "arduino-cli.exe" -ArgumentList $arguments -NoNewWindow -PassThru

# Tempo stimato (in secondi) per la compilazione: modificalo in base alle tue misurazioni
$totalTime = 60
$startTime = Get-Date

# Simula la barra di avanzamento basata su un timer
while (-not $process.HasExited) {
    $elapsed = (Get-Date) - $startTime
    $percent = [math]::Min(($elapsed.TotalSeconds / $totalTime) * 100, 100)
    Write-Progress -Activity "Compilazione Arduino" -Status ("Progresso stimato: {0:N0}%%" -f $percent) -PercentComplete $percent
    Start-Sleep -Seconds 1
}

# Assicurati che la barra sia al 100% al termine
Write-Progress -Activity "Compilazione Arduino" -Status "100% completato" -PercentComplete 100
Write-Host "Compilazione completata!"
