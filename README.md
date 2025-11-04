# Packet Transmit/Receive Simulator

This small C++ program simulates breaking a text file into packets (5 characters per packet), assigning sequence numbers, sending them in an unordered (shuffled) order, receiving them, and reconstructing the original message.

Features:
- Prompts for a filename (or accept it as a command-line argument).
- Splits into 5-character packets, numbers each packet starting at 0.
- Sends (prints) packets in a shuffled order to simulate different routes.
- Receives packets and reconstructs the original message by ordering by sequence number.

Files:
- `data.cpp`: The simulator source.
- `sample_message.txt`: Example input file.

How to build (Windows with g++ available) and run:
```powershell
g++ -std=c++17 -O2 -o packet_simulator.exe data.cpp
./packet_simulator.exe sample_message.txt
