# tools

Build and flash tools are redirected to Python scripts.
They are tested on Ubuntu 16.04.

```
sudo apt install gcc-arm-none-eabi
virtualenv -p python3 ~/w600-venv
source ~/w600-venv/bin/activate
make
make flash DL_PORT=ttyUSB0
```
