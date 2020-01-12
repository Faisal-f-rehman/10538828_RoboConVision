#!/bin/bash

#cd GUI && python RoboConGui.py &
#P1=$!
#cd RoboConVision && ./RoboConVision && fg
#P2=$!
#wait $P1 $P2


#gnome-terminal -e 'bash -c "cd RoboConVision && ./RoboConVision";exec bash"'
#gnome-terminal -e 'bash -c "second-script.sh; exec bash"'
#cd GUI && python RoboConGui.py & 
#cd RoboConVision && ./RoboConVision

#trap "exit" INT TERM ERR
#trap "kill 0" EXIT

cd GUI && python RoboConGui.py &
P1=$!
gnome-terminal -x sh -c "cd RoboConVision && ./RoboConVision; exit" &
P2=$!

wait $P1 $P2

