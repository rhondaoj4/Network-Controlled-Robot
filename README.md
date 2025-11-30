# Network-Controlled RobotVehicle

## I. System Description: Goals and Scope

### **Goal**: To design, implement, and demonstrate a two-part client-server system that uses the TCP protocol and a custom Application Layer Protocol to achieve remote physical control of a robot.

### **Motivation**: I chose this project because I have a passion for robotics and wanted to merge TCP connection to that, which has been really helpful, because I now know how to control a robot from the computer, tying the abstract concepts of network stacks to tangible physical action.

### **Core Functionality**: The system allows a user running a C++ client program on an Arch Linux machine to send immediate, reliable movement commands (FORWARD, LEFT, etc.) to a separate, battery-powered ESP32 microcontroller over Wi-Fi, which demonstrates a complete application-to-physical layer communication pipeline.

## II. Main Modules and Network Stack Relation

The system is split into two modules communicating over a local area network (LAN), emphasizing the client-server model learned in CS 1652.

### Module 1: The Client (Controller)
#### Connection (TCP): Open a persistent TCP socket connection to the robot's IP address and Port 8080. This guarantees reliable, in-order delivery of control commands, ensuring a STOP command is not lost or delivered after a subsequent FORWARD.
#### (Application): Takes user input (w, s, a, d, x) and translates it into the custom, fixed-string protocol (e.g., "FORWARD", "STOP").
#### Sending (TCP <--> Applicatoin): Sends the command string over the established TCP socket.

### Module 2: The Server (Robot Brain)
#### Connection (TCP): Connects to the local Wi-Fi, acquires an IP, and starts a TCP listener on Port 8080.
#### Decoding (Application): Continuously checks the socket (client.available()) for incoming commands. It reads the string and uses conditional logic (if (command == "FORWARD")) to determine the requested action.
#### Control (Physical <--> Link): Translates the decoded command into specific electrical signals (HIGH/LOW on GPIO pins) to control the L298N motor driver. The final implementation included correcting function logic (swapping direction functions) to match the physical wiring.

### III. System Architecture and Hardware

The physical architecture highlights the necessity of the L298N Motor Driver as a buffer between the sensitive low-voltage logic (ESP32) and the high-current power required by the actuators (motors).

| Component | Function | Networking Role |
| ------------- | ------------- | ----------- |
| ESP32 | Microcontroller | Runs TCP Server; handles TCP and Application layer processing |
| L298N | Motor Driver | Power Amplifier |
| 4xAA Batteries | External Power Source |  |

#### Pin Mapping Instructions

| ESP32 GPIO Pin | L298N Terminal | Purpose |
| ------------- | ------------- | ----------- |
| GPIO 26 | IN1 | Left Motor 1 |
| GPIO 27 | IN2 | Left Motor 2 |
| GPIO 14 | ENA | Left Motor Enable |
| GPIO 25 | IN3 | Right Motor 1 |
| GPIO 33 | IN4 | Right Motor 2 |
| GPIO 32 | ENB | Right Motor Enable |


### IV. How To Run/Use This Code (Arch Linux)

#### A. Required Components
##### - ESP32 Development Board (programmed via Arduino IDE)
##### - L298N Motor Driver Module
##### - 2WD Chassis with DC Motors
##### - 4x AA Battery Pack
##### - Jumper Wires (M/F for logic, M/M for power/ground connections)
##### - Screwdriver & Soldering Iron (for motor/power connections)
##### - Breadboard (optional)

#### B. Software Setup (Arch Linux)
##### Install Dependencies:

```
sudo pacman -S python-pyserial g++
```

##### Compile Client: Compile the C++ controller

```
g++ robot_client.cpp -o robot_client
```

#### C. ESP32 Server

##### -Update Network: In RobotServer.ino, set your Wi-Fi name and password.
##### -Upload Code: Use Arduino IDE to upload RobotServer.ino via USB. (Jarring that they still use micro usb instead of type-c.)
##### -Obtain IP: Open the Serial Monitor (set to 115200 baud). The ESP32 will connect to Wi-Fi and print the IP address assigned by the router.



