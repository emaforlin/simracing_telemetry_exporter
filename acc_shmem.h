#pragma once
#include <windows.h>

// Assetto Corsa Competizione Shared Memory reader interface
// https://www.assettocorsa.net/forum/index.php?threads/acc-shared-memory-documentation.59965/

#define ACC_SHARED_MEMORY_VERSION "v1.8.12"

// ACC shared memory names are wide strings (OpenFileMappingW requires wchar_t*)
#define ACC_PHYSICS  L"Local\\acpmf_physics"
#define ACC_GRAPHICS L"Local\\acpmf_graphics"
#define ACC_STATIC   L"Local\\acpmf_static"

#pragma pack(push, 4)

// Physics shared memory
// The following members change at each graphic step. They all refer to the player’s car.
// IMPORTANT: not all the members are implemented in this interface

struct SPageFilePhysics {
    int packetId;                   // Current step index
    float gas;                      // Gas pedal input value (from -0 to 1.0)
    float brake;                    // Brake pedal input value (from -0 to 1.0)
    float fuel;                     // Fuel amount remaining in kg
    int gear;                       // Current gear
    int rpm;                        // Engine rpm
    float steerAngle;               // Sterring input value (from -1.0 to 1.0)
    float speedKmh;                 // Speed in km/h
    float velocity[3];              // Car velocity vector in global coordinates
    float accG[3];                  // Car acceleration vector in global coordinates 
    float wheelSlip[4];             // Tyre slip for each tyre  [FL, FR, RL, RR]  
    float wheelPressure[4];         // Tyre pressure [FL, FR, RL, RR]
    float wheelAngularSpeed[4];     // Wheel angular speed in rad/s  [FL, FR, RL, RR] 
    float tyreCoreTemperature[4];   // Tyre rubber core temperature [FL, FR, RL, RR]  
    float suspensionTravel[4];      // Suspension travel [FL, FR, RL, RR]  
    float tc;                       // TC in action
    float heading;                  // Car yaw orientation
    float pitch;                    // Car pitch orientation
    float roll;                     // Car roll orientation
    float carDamage[5];             // Car damage: front 0, rear 1, left 2, right 3, centre 4  
    int pitLimiterOn;               // Pit limiter on
    float abs;                      // ABS in action
    int autoShifterOn;              // Automatic transmission on 
    float turboBoost;               // Car turbo level
    float airTemp;                  // Air temperature
    float roadTemp;                 // Road temperature
    float localAngularVel[3];       // Car angular velocity vector in local coordinates 
    float finalFF;                  // Force feedback signal 
    float brakeTemp[4];             // Brake discs temperatures [FL, FR, RL, RR] 
    float clutch;                   // Clutch input value (from -0 to 1.0)
    int isAIControlled;             // Car is controlled by the AI  
    float tyreContactPoint[4][3];   // Tyre contact point global coordinates [FL, FR, RL, RR] [x,y,z]
    float tyreContactNormal[4][3];  // Tyre contact normal [FL, FR, RL, RR] [x,y,z]
    float tyreContactHeading[4][3]; // Tyre contact heading [FL, FR, RL, RR] [x,y,z]
    float brakeBias;                // Front brake bias, see Appendix 4
    float localVelocity[3];         // Car velocity vector in local coordinates
    float slipRatio[4];             // Tyre slip ratio [FL, FR, RL, RR]
    float slipAngle[4];             // Tyre slip angle [FL, FR, RL, RR] in radians
    float waterTemp;                // Water temperature
    float brakePressure[4];         // Brake pressure [FL, FR, RL, RR] see Appendix 2 
    int frontbrakeCompund;          // Brake pad compund front 
    int rearBrakeCompound;          // Brake pad compund rear 
    float padLife[4];               // Brake pad wear [FL, FR, RL, RR]
    float discLife[4];              // Brake disc wear [FL, FR, RL, RR]
    int ignitionOn;                 // Ignition switch set to on? 
    int starterEngineOn;            // Starter Switch set to on? 
    int isEngineRunning;            // Engine running?
};

#pragma pack(pop)