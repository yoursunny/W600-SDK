# tools

Build and flash tools are redirected to Python scripts.
They are tested on Debian Stretch ARMv7.

One time install:

```
sudo apt install gcc-arm-none-eabi python3-pip
sudo pip3 install -U pip
sudo pip3 install -U virtualenv
virtualenv -p python3 ../w600-venv
source ../w600-venv/bin/activate
pip install -r tools/py_scripts/requirements.txt
```

Build and upload:

```
virtualenv -p python3 ../w600-venv
make
make flash DL_PORT=ttyUSB0
```
