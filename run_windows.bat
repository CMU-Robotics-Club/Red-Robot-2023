@ECHO OFF

python -m pip install pygame pyserial
python Controller-Interface/controller_hid.py
PAUSE