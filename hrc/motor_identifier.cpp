#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

#include "serialPort/SerialPort.h"
#include "unitreeMotor/unitreeMotor.h"

int main()
{
    std::vector<std::string> ports = {
        "/dev/ttyUSB0",
        "/dev/ttyUSB1",
        "/dev/ttyUSB2"
    };

    const int MAX_ID = 8;

    for(const auto& port : ports) {
        std::cout << "\n====================================\n";
        std::cout << "Scanning port: " << port << std::endl;
        std::cout << "====================================\n";

        try {
            SerialPort serial(port);

            for(int id = 0; id < MAX_ID; id++) {
                MotorCmd cmd;
                MotorData data;
                /*data.q = 9999;
                data.dq = 9999;
                data.temp = -1;
                data.merror = -1;*/

                cmd.motorType = MotorType::GO_M8010_6;
                data.motorType = MotorType::GO_M8010_6;

                cmd.mode = queryMotorMode(MotorType::GO_M8010_6, MotorMode::FOC);

                cmd.id = id;
                cmd.kp = 0;
                cmd.kd = 0;
                cmd.q  = 0;
                cmd.dq = 0;
                cmd.tau = 0;

                serial.sendRecv(&cmd, &data);

                std::cout <<  std::endl;
                std::cout << "MOTOR ID: " << id << std::endl;
                std::cout <<  "motor.q: " << data.q <<  std::endl;
                std::cout <<  "motor.temp: " << data.temp <<  std::endl;
                std::cout <<  "motor.W: " << data.dq <<  std::endl;
                std::cout <<  "motor.merror: " << data.merror <<  std::endl;
                std::cout <<  std::endl;

                usleep(2000);
            }
        }
        catch(...) {
            std::cout << "Could not open port." << std::endl;
        }
    }

    std::cout << "\nScan complete.\n";

    return 0;
}