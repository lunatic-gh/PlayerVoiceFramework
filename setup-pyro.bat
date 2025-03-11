@echo off
setlocal

if exist pyro (
    echo Pyro already exists, skipping
) else (
    mkdir pyro
    curl -L -o pyro.zip https://github.com/fireundubh/pyro/releases/download/1656807840/pyro-master-1656807840.zip
    tar -xf pyro.zip -C pyro
    del pyro.zip
    echo Pyro was set up.
)

endlocal