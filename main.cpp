#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "shmem_reader.cpp"
#include "acc_shmem.h"

int main() {
    SharedMemReader<SPageFilePhysics> reader(ACC_PHYSICS);
    if (!reader.init()) {
        std::cerr << "Failed to init physics reader" << std::endl;
        std::cout << "Could not connect to ACC. Is the game running?" << std::endl;
        return 1;
    }

    std::cout << "Connected! Reading telemetry..." << std::endl;

    while (true) {
        const auto* physics = reader.data();

        if (physics) {
            std::system("cls"); // Clear console
            std::cout << "Speed: " << physics->speedKmh << " km/h" << std::endl;
            std::cout << "RPM:   " << physics->rpms << std::endl;
            if (physics->gear == 0) {
                std::cout << "Gear:  R" << std::endl;
            } else if (physics->gear == 1) {
                std::cout << "Gear:  N" << std::endl;
            } else {
                std::cout << "Gear:  " << physics->gear - 1 << std::endl;
            }
            // std::cout<< "Clutch: " << physics->clutch << std::endl;
            std::cout<< "Brake: " << physics->brake << std::endl;
            std::cout<< "Gas: " << physics->gas << std::endl;
            std::cout<< "Brake Bias: " << physics->brakeBias << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}