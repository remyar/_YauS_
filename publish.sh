#! /bin/bash

cd core
pio pkg publish --no-interactive
cd ..

cd archs

cd stm32f1xx
pio pkg publish --no-interactive
cd ..

cd ..

cd modules

cd console
pio pkg publish --no-interactive
cd ..

cd drivers
pio pkg publish --no-interactive
cd ..

cd queue
pio pkg publish --no-interactive
cd ..

pause
