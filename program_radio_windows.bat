@ECHO OFF

python -m pip install hidapi pyserial
python Controller-Interface/program_nrf.py
PAUSE
