// Packet transmit/receive simulator
// Breaks a file into packets of 5 characters, numbers them, sends them in
// unordered sequence (shuffled), then receives packets and reconstructs
// the original message.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Packet {
    int seq; // sequence number (0-based)
    string data; // up to 5 chars
};

string read_file_all(const string &path) {
    ifstream in(path, ios::binary);
    if(!in) throw runtime_error("Could not open file: " + path);
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    return contents;
}

vector<Packet> make_packets(const string &msg, size_t chunk_size = 5) {
    vector<Packet> packets;
    int seq = 0;
    for(size_t i = 0; i < msg.size(); i += chunk_size) {
        Packet p;
        p.seq = seq++;
        p.data = msg.substr(i, chunk_size);
        packets.push_back(move(p));
    }
    return packets;
}

int main(int argc, char** argv) {
    cout << "Packet transmit/receive simulator\n";
    cout << "Breaks file into packets of 5 characters, sends unordered, then reconstructs.\n\n";

    string filename;
    if(argc > 1) filename = argv[1];
    else {
        cout << "Enter path to text file to send: ";
        if(!getline(cin, filename)) return 0;
    }

    string message;
    try {
        message = read_file_all(filename);
    } catch(const exception &e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    if(message.empty()) {
        cout << "File is empty. Nothing to send.\n";
        return 0;
    }

    // Create packets of 5 characters each
    auto packets = make_packets(message, 5);

    cout << "Created " << packets.size() << " packets:\n";
    for(const auto &p : packets) {
        cout << "  Packet #" << p.seq << ": '" << p.data << "'\n";
    }
    cout << '\n';

    // Simulate sending unordered by shuffling
    vector<Packet> sent = packets;
    random_device rd;
    mt19937 g(rd());
    shuffle(sent.begin(), sent.end(), g);

    cout << "Sending packets in unordered sequence:\n";
    for(const auto &p : sent) {
        cout << "  Sent Packet #" << p.seq << ": '" << p.data << "'\n";
    }
    cout << "\n";

    // Simulate receiving one by one
    cout << "Receiving packets (arrival order shown):\n";
    unordered_map<int, string> received_map;
    received_map.reserve(sent.size()*2);
    for(const auto &p : sent) {
        // "Receive" this packet
        cout << "  Received Packet #" << p.seq << ": '" << p.data << "'\n";
        received_map[p.seq] = p.data;
    }

    cout << "\nReconstructing message from received packets...\n";
    string reconstructed;
    for(size_t i = 0; i < packets.size(); ++i) {
        auto it = received_map.find(static_cast<int>(i));
        if(it == received_map.end()) {
            cerr << "Missing packet #" << i << " -- message incomplete\n";
            return 2;
        }
        reconstructed += it->second;
    }

    cout << "\nOriginal message (showing visible whitespace as-is):\n";
    cout << message << "\n\n";

    cout << "Reconstructed message:\n";
    cout << reconstructed << "\n\n";

    if(reconstructed == message) cout << "Success: reconstructed message matches original.\n";
    else cout << "Warning: reconstructed message differs from original.\n";

    return 0;
}