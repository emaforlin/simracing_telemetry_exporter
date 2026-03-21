#include "acc_shmem.h"
#include "serial/serial.h"
#include "serial_telemetry_protocol/serial_telemetry_protocol.h"
#include "shmem_reader.cpp"
#include "telemetry_packer/telemetry_packer.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

int main() {
  SharedMemReader<SPageFilePhysics> reader(ACC_PHYSICS);
  if (!reader.init()) {
    std::cerr << "Failed to init physics reader" << std::endl;
    std::cout << "Could not connect to ACC. Is the game running?" << std::endl;
    return 1;
  }

  std::cout << "Connected! Reading telemetry..." << std::endl;

  /* ── Serial Export ─────────────────────────────────────────────── */
  Serial serial("COM1", 115200);
  if (!serial.isOpen()) {
    std::cerr << "Failed to open serial port" << std::endl;
    return 1;
  }

  int lastPacketId = -1;
  const auto *physics = reader.data();

  while (physics && physics->packetId != lastPacketId) {
    lastPacketId = physics->packetId;
    std::system("cls");
    std::cout << "Packet ID: " << physics->packetId << std::endl;
    std::cout << "Gas: " << physics->gas << std::endl;
    std::cout << "Brake: " << physics->brake << std::endl;
    std::cout << "Fuel: " << physics->fuel << " kg" << std::endl;
    if (physics->gear == 0) {
      std::cout << "Gear:  R" << std::endl;
    } else if (physics->gear == 1) {
      std::cout << "Gear:  N" << std::endl;
    } else {
      std::cout << "Gear:  " << physics->gear - 1 << std::endl;
    }
    std::cout << "RPM:   " << physics->rpm << std::endl;
    // std::cout << "Steer Angle: " << physics->steerAngle << std::endl;
    // std::cout << "Speed: " << physics->speedKmh << " km/h" << std::endl;

    // std::cout << "Velocity [x,y,z]: [" << physics->velocity[0] << ", " <<
    // physics->velocity[1] << ", " << physics->velocity[2] << "]" << std::endl;
    // std::cout << "AccG [x,y,z]: [" << physics->accG[0] << ", " <<
    // physics->accG[1] << ", " << physics->accG[2] << "]" << std::endl;
    // std::cout << "Wheel Slip [FL,FR,RL,RR]: [" << physics->wheelSlip[0] << ",
    // " << physics->wheelSlip[1] << ", " << physics->wheelSlip[2] << ", " <<
    // physics->wheelSlip[3] << "]" << std::endl;

    // std::cout << "Clutch: " << physics->clutch << std::endl;
    // std::cout << "Wheel Pressure [FL,FR,RL,RR]: [" <<
    // physics->wheelPressure[0] << ", " << physics->wheelPressure[1] << ", " <<
    // physics->wheelPressure[2] << ", " << physics->wheelPressure[3] << "]" <<
    // std::endl; std::cout << "Wheel Angular Speed [FL,FR,RL,RR]: [" <<
    // physics->wheelAngularSpeed[0] << ", " << physics->wheelAngularSpeed[1] <<
    // ", " << physics->wheelAngularSpeed[2] << ", " <<
    // physics->wheelAngularSpeed[3] << "]" << std::endl; std::cout << "Tyre
    // Core Temp [FL,FR,RL,RR]: [" << physics->tyreCoreTemperature[0] << ", " <<
    // physics->tyreCoreTemperature[1] << ", " <<
    // physics->tyreCoreTemperature[2] << ", " <<
    // physics->tyreCoreTemperature[3] << "]" << std::endl; std::cout <<
    // "Suspension Travel [FL,FR,RL,RR]: [" << physics->suspensionTravel[0] <<
    // ", " << physics->suspensionTravel[1] << ", " <<
    // physics->suspensionTravel[2] << ", " << physics->suspensionTravel[3] <<
    // "]" << std::endl; std::cout << "TC: " << physics->tc << " | ABS: " <<
    // physics->abs << std::endl; std::cout << "Heading/Pitch/Roll: [" <<
    // physics->heading << ", " << physics->pitch << ", " << physics->roll <<
    // "]" << std::endl; std::cout << "Car Damage [F,R,L,R,C]: [" <<
    // physics->carDamage[0] << ", " << physics->carDamage[1] << ", " <<
    // physics->carDamage[2] << ", " << physics->carDamage[3] << ", " <<
    // physics->carDamage[4] << "]" << std::endl;

    // std::cout << "Pit Limiter: " << physics->pitLimiterOn << " | Auto
    // Shifter: " << physics->autoShifterOn << std::endl; std::cout << "Turbo
    // Boost: " << physics->turboBoost << std::endl; std::cout << "Air Temp: "
    // << physics->airTemp << " | Road Temp: " << physics->roadTemp << " | Water
    // Temp: " << physics->waterTemp << std::endl;

    // std::cout << "Local Angular Vel [x,y,z]: [" <<
    // physics->localAngularVel[0] << ", " << physics->localAngularVel[1] << ",
    // " << physics->localAngularVel[2] << "]" << std::endl; std::cout << "Final
    // FF: " << physics->finalFF << std::endl; std::cout << "Brake Temp
    // [FL,FR,RL,RR]: [" << physics->brakeTemp[0] << ", " <<
    // physics->brakeTemp[1] << ", " << physics->brakeTemp[2] << ", " <<
    // physics->brakeTemp[3] << "]" << std::endl; std::cout << "Is AI
    // Controlled: " << physics->isAIControlled << std::endl;

    // for (int i = 0; i < 4; ++i) {
    //     std::cout << "Tyre Contact Point " << i << " [x,y,z]: [" <<
    //     physics->tyreContactPoint[i][0] << ", " <<
    //     physics->tyreContactPoint[i][1] << ", " <<
    //     physics->tyreContactPoint[i][2] << "]" << std::endl; std::cout <<
    //     "Tyre Contact Normal " << i << " [x,y,z]: [" <<
    //     physics->tyreContactNormal[i][0] << ", " <<
    //     physics->tyreContactNormal[i][1] << ", " <<
    //     physics->tyreContactNormal[i][2] << "]" << std::endl; std::cout <<
    //     "Tyre Contact Heading " << i << " [x,y,z]: [" <<
    //     physics->tyreContactHeading[i][0] << ", " <<
    //     physics->tyreContactHeading[i][1] << ", " <<
    //     physics->tyreContactHeading[i][2] << "]" << std::endl;
    // }

    // std::cout << "Brake Bias: " << physics->brakeBias << std::endl;
    // std::cout << "Local Velocity [x,y,z]: [" << physics->localVelocity[0] <<
    // ", " << physics->localVelocity[1] << ", " << physics->localVelocity[2] <<
    // "]" << std::endl; std::cout << "Slip Ratio [FL,FR,RL,RR]: [" <<
    // physics->slipRatio[0] << ", " << physics->slipRatio[1] << ", " <<
    // physics->slipRatio[2] << ", " << physics->slipRatio[3] << "]" <<
    // std::endl; std::cout << "Slip Angle [FL,FR,RL,RR]: [" <<
    // physics->slipAngle[0] << ", " << physics->slipAngle[1] << ", " <<
    // physics->slipAngle[2] << ", " << physics->slipAngle[3] << "]" <<
    // std::endl; std::cout << "Brake Pressure [FL,FR,RL,RR]: [" <<
    // physics->brakePressure[0] << ", " << physics->brakePressure[1] << ", " <<
    // physics->brakePressure[2] << ", " << physics->brakePressure[3] << "]" <<
    // std::endl;

    // std::cout << "Front Brake Compound: " << physics->frontbrakeCompund << "
    // | Rear Brake Compound: " << physics->rearBrakeCompound << std::endl;
    // std::cout << "Pad Life [FL,FR,RL,RR]: [" << physics->padLife[0] << ", "
    // << physics->padLife[1] << ", " << physics->padLife[2] << ", " <<
    // physics->padLife[3] << "]" << std::endl; std::cout << "Disc Life
    // [FL,FR,RL,RR]: [" << physics->discLife[0] << ", " << physics->discLife[1]
    // << ", " << physics->discLife[2] << ", " << physics->discLife[3] << "]" <<
    // std::endl;

    // std::cout << "Ignition On: " << physics->ignitionOn << " | Starter Engine
    // On: " << physics->starterEngineOn << " | Engine Running: " <<
    // physics->isEngineRunning << std::endl;

    /* ── Build & display packed dashboard packet ───────────────────── */
    uint8_t pkt_buf[TELEMETRY_MAX_FRAME_SIZE];
    size_t pkt_len = 0;

    if (build_dashboard_packet(physics, pkt_buf, &pkt_len)) {
      std::cout << "\n--- Dashboard Packet (" << pkt_len << " bytes) ---\n";
      for (size_t i = 0; i < pkt_len; ++i) {
        std::cout << std::hex << std::uppercase << std::setfill('0')
                  << std::setw(2) << static_cast<int>(pkt_buf[i]) << " ";
      }
      std::cout << std::dec << std::endl;

      serial.write(pkt_buf, pkt_len);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}