@echo off
REM Script para rodar o simulador no Windows via WSL

echo ======================================
echo  LiPo Battery Tester - Simulator
echo ======================================
echo.
echo Modos disponiveis:
echo   1. Demo - Executa casos de teste automaticos
echo   2. Interactive - Digite voltagens manualmente
echo   3. Monitor - Simula descarga de bateria em tempo real
echo.

set /p mode="Escolha o modo (1-3): "

if "%mode%"=="1" (
    wsl bash -c "cd /mnt/c/dsn/arduino/lipobatterytester/simulator && ./lipo_simulator demo"
) else if "%mode%"=="2" (
    echo.
    echo Digite voltagens para testar (0 para sair)
    echo Exemplos: 3.7 (1S), 7.4 (2S), 11.1 (3S), 14.8 (4S)
    echo.
    wsl bash -c "cd /mnt/c/dsn/arduino/lipobatterytester/simulator && ./lipo_simulator interactive"
) else if "%mode%"=="3" (
    echo.
    echo Iniciando simulacao de descarga de bateria 3S...
    echo Pressione Ctrl+C para parar
    echo.
    wsl bash -c "cd /mnt/c/dsn/arduino/lipobatterytester/simulator && ./lipo_simulator monitor"
) else (
    echo Modo invalido!
)

pause
