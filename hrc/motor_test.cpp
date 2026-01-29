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
  const int NUM_MOTORS = 6;
  MotorCmd    cmd[NUM_MOTORS];
  MotorData   data[NUM_MOTORS];

  for (int i = 0; i < NUM_MOTORS; i++) {
    cmd[i].motorType = MotorType::GO_M8010_6;
    data[i].motorType = MotorType::GO_M8010_6;
    cmd[i].mode = queryMotorMode(MotorType::GO_M8010_6,MotorMode::FOC);
    cmd[i].id = i;
    cmd[i].kp = 0.0;
    cmd[i].kd = 0.0;
    cmd[i].q = 0.0;
    cmd[i].dq = 0.0;
  }

  auto torque_table = loadTorques("torques.csv");
  size_t step = 0;

  while(step < torque_table.size()) {
    for (int i = 0; i < NUM_MOTORS; i++) {
      cmd[i].tau = torque_table[step][i] * queryGearRatio(MotorType::GO_M8010_6);
      serial.sendRecv(&cmd[i], &data[i]);
    }
    step++;
    usleep(200);
  }

}