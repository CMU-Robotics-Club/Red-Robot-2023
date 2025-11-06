#!/bin/bash

# Make sure this script fails if any part fails
set -e

# The current directory is not set correctly by default
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${DIR}"

python3 -m pip install hidapi pyserial || :

python3 ./Controller-Interface/program_nrf.py || :
echo "Press any key to exit."
read -n 1 -s
