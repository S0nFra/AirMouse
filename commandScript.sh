#!/bin/bash

# Course: Embedded Systems 2021/2022
#   
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#  
# Group:
# 
# Ranieri Speranza		0622701687	s.ranieri4@studenti.unisa.it
# Mancusi Ernesto		0622701668	e.mancusi2@studenti.unisa.it
# Parrella Giuseppe		0622701665	g.parrella9@studenti.unisa.it
# Sonnessa Francesco		0622701672	f.sonnessa@studenti.unisa.it
# 
# Copyright (C) 2022 - All Rights Reserved
# This file is part of AirMouse.
# 
# AirMouse is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# AirMouse is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with AirMouse.  If not, see <http://www.gnu.org/licenses/>.

# Open /dev/ttyUSB0 on FD9
exec 9< /dev/ttyACM0
# Set all parameters for correct communication with serial port
stty -F /dev/ttyACM0 115200 -brkint -icrnl -imaxbel iutf8 -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
# Infinite loop
while true; do 
    # Read the next command from the serial port
    read -e output <&9
    # Check if this new command is an empty string, or a valid command 
	if [ -z "$output" ]
		then
		# Discard the command if this is an empty string
			  echo "."
		else
		# Print the command on terminal and execute it if is not an empty string
			  echo "$output" 
			  eval "$output"
	fi
done
