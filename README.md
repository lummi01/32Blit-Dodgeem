# Dodge'em
![](/assets/image.png)<BR>
![](/assets/Dodge'em0.bmp)<BR>
A classic arcade game for 32Blit.<BR>
!! early beta version !!<BR>
<BR>



For local build:
```
mkdir build
cd build
cmake -D32BLIT_DIR=/path/to/32blit-sdk/ ..
make
```

For 32Blit build:
```
mkdir build.stm32
cd build.stm32
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/32blit/repo/32blit.toolchain
make
```

For PicoSystem build:
```
mkdir build.pico
cd build.pico
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../32blit-sdk/pico.toolchain -DPICO_BOARD=pimoroni_picosystem
make
```
