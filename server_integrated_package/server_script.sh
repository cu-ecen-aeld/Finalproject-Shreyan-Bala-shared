#!/bin/sh

echo "Starting Server and Sensor programs"

make clean

make

server &

sensor 
