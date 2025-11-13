#!/bin/bash
cd "/Users/kumaraswamy/Desktop/cgm main 3"
echo "Starting traffic simulator..."
./traffic_simulator 2>&1 &
PID=$!
sleep 2
echo "Simulator started with PID: $PID"
echo "Check the terminal output for model loading messages"
echo "Press any key to stop..."
