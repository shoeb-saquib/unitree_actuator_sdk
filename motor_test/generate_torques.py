import csv
import math

NUM_MOTORS = 6
DT = 0.0002
DURATION = 5.0      # seconds

MODE = "sin"        # "constant" or "sin"

CONST_TORQUE = 0.05 # Nm
SIN_AMPL = 0.1      # Nm
SIN_FREQ = 0.5      # Hz

num_steps = int(DURATION / DT)

with open("torques.csv", "w", newline="") as f:
    writer = csv.writer(f)

    for k in range(num_steps):
        t = k * DT

        if MODE == "constant":
            row = [CONST_TORQUE] * NUM_MOTORS
        elif MODE == "sin":
            tau = SIN_AMPL * math.sin(2 * math.pi * SIN_FREQ * t)
            row = [tau] * NUM_MOTORS
        else:
            raise ValueError("Unknown MODE")

        writer.writerow(row)
