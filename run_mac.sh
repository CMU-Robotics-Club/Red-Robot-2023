#!/bin/bash

# Make sure this script fails if any part fails
set -e

# The current directory is not set correctly by default
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${DIR}"

python3 -m pip install pygame pyserial

if python3 ./Controller-Interface/controller_pygame.py ; then
	echo "Finished"
else
	echo "An error occurred. Press any key to exit."
	read -n 1 -s
fi
