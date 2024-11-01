@ECHO OFF

python -m pip install hidapi pyserial
python Controller-Interface/controller_hid.py
PAUSE