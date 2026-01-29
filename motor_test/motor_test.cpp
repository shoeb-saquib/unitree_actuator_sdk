#include <unistd.h>
#include "serialPort/SerialPort.h"
#include "unitreeMotor/unitreeMotor.h"
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::vector<float>> loadTorques(const std::string &filename) {
  std::vector<std::vector<float>> torques;
  std::ifstream file(filename);
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string value;
    std::vector<float> row;

    while (std::getline(ss, value, ',')) {
      row.push_back(std::stof(value));
    }

    if (row.size() == 6)
      torques.push_back(row);
}

return torques;
}

int main() {

  SerialPort  serial("/dev/ttyUSB0");
  MotorCmd    cmd;
  MotorData   data;
  cmd.mode = queryMotorMode(MotorType::GO_M8010_6,MotorMode::FOC);
  cmd.kp   = 0.0;
  cmd.kd   = 0.0;
  cmd.q    = 0.0;
  cmd.dq   = 0.0;

  auto torque_table = loadTorques("torques.csv");
  size_t step = 0

  while(step < torque_table.size()) {
    cmd.motorType = MotorType::GO_M8010_6;
    data.motorType = MotorType::GO_M8010_6;
    for (int i = 0; i < 6; i++) {
      cmd.id   = i;
      cmd.tau  = torque_table[step][i] * queryGearRatio(MotorType::GO_M8010_6);
      serial.sendRecv(&cmd,&data);
    }
    step++;
    usleep(200);
  }

}