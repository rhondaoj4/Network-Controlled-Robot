#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // --- Configuration ---
    // Update this IP and Port when connecting to the ESP32
    const char* SERVER_IP = "10.0.0.40"; 
    int PORT = 8080; 

    // 1. Setup Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error: Socket creation failure." << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address/Address not supported." << std::endl;
        return -1;
    }

    // 2. Connect to the Robot Server
    std::cout << "Connecting to robot at " << SERVER_IP << ":" << PORT << "..." << std::endl;
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed. Check if the robot is on." << std::endl;
        return -1; 
    }

    std::cout << "Connection successful. Enter w/a/s/d to move, 'x' to stop, 'q' to quit." << std::endl;
    std::cout << "NOTE: You must press ENTER after each keypress." << std::endl;


    // 3. Command Loop (Blocking Input)
    char input;
    while (true) {
        std::cout << "> ";
        std::cin >> input; 

        const char *msg;
        if (input == 'w') msg = "FORWARD";
        else if (input == 's') msg = "BACKWARD"; 
        else if (input == 'a') msg = "LEFT";
        else if (input == 'd') msg = "RIGHT";
        else if (input == 'x') msg = "STOP"; 
        else if (input == 'q') break;
        else msg = "INVALID";

        if (strcmp(msg, "INVALID") != 0) {
            send(sock, msg, strlen(msg), 0);
            std::cout << "Sent: " << msg << std::endl;
        }
    }

    close(sock);
    return 0;
}