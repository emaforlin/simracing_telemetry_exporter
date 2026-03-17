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
    float velocity[3];              // Car velocity vector in global coordinates (m/s)
    float accG[3];                  // Car acceleration vector in global coordinates 
    float wheelSlip[4];             // Tyre slip for each tyre  [FL, FR, RL, RR]  
    float wheelLoad[4];             // (Field not used. DO NOT REMOVE to keep memory alignment)
    float wheelPressure[4];         // Tyre pressure [FL, FR, RL, RR]
    float wheelAngularSpeed[4];     // Wheel angular speed in rad/s  [FL, FR, RL, RR]
    float tyreWear[4];              // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tyreDirtyLevel[4];        // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tyreCoreTemperature[4];   // Tyre rubber core temperature [FL, FR, RL, RR]
    float camberRAD[4];             // (Field not used. DO NOT REMOVE to keep memory alignment)
    float suspensionTravel[4];      // Suspension travel [FL, FR, RL, RR]
    float drs;                      // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tc;                       // TC in action
    float heading;                  // Car yaw orientation
    float pitch;                    // Car pitch orientation
    float roll;                     // Car roll orientation
    int cgHeight;                   // (Field not used. DO NOT REMOVE to keep memory alignment)
    float carDamage[5];             // Car damage: front 0, rear 1, left 2, right 3, centre 4  
    int numberOfTyresOut;           // (Field not used. DO NOT REMOVE to keep memory alignment)
    int pitLimiterOn;               // Pit limiter on
    float kersCharge;               // (Field not used. DO NOT REMOVE to keep memory alignment)
    float kersInput;                // (Field not used. DO NOT REMOVE to keep memory alignment)
    float abs;                      // ABS in action
    int autoShifterOn;              // Automatic transmission on 
    float rideHeight[2];            // (Field not used. DO NOT REMOVE to keep memory alignment)
    float turboBoost;               // Car turbo level
    float ballast;                  // (Field not used. DO NOT REMOVE to keep memory alignment)   
    float airDensity;               // (Field not used. DO NOT REMOVE to keep memory alignment)
    float airTemp;                  // Air temperature
    float roadTemp;                 // Road temperature
    float localAngularVel[3];       // Car angular velocity vector in local coordinates 
    float finalFF;                  // Force feedback signal
    float performaceMeter;          // (Field not used. DO NOT REMOVE to keep memory alignment)
    int engineBrake;                // (Field not used. DO NOT REMOVE to keep memory alignment)
    int ersRecoveryLevel;           // (Field not used. DO NOT REMOVE to keep memory alignment)
    int ersPowerLevel;              // (Field not used. DO NOT REMOVE to keep memory alignment)
    int ersHeatCharging;            // (Field not used. DO NOT REMOVE to keep memory alignment)
    int ersIsCharging;              // (Field not used. DO NOT REMOVE to keep memory alignment)
    float kersCurrentKJ;            // (Field not used. DO NOT REMOVE to keep memory alignment)
    int drsAvailable;               // (Field not used. DO NOT REMOVE to keep memory alignment)
    int drsEnabled;                 // (Field not used. DO NOT REMOVE to keep memory alignment)
    float brakeTemp[4];             // Brake discs temperatures [FL, FR, RL, RR] 
    float clutch;                   // Clutch input value (from -0 to 1.0)
    float tyreTempI[4];             // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tyreTempM[4];             // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tyreTempO[4];             // (Field not used. DO NOT REMOVE to keep memory alignment)
    int isAIControlled;             // Car is controlled by the AI  
    float tyreContactPoint[4][3];   // Tyre contact point global coordinates [FL, FR, RL, RR] [x,y,z]
    float tyreContactNormal[4][3];  // Tyre contact normal [FL, FR, RL, RR] [x,y,z]
    float tyreContactHeading[4][3]; // Tyre contact heading [FL, FR, RL, RR] [x,y,z]
    float brakeBias;                // Front brake bias, see Appendix 4
    float localVelocity[3];         // Car velocity vector in local coordinates
    int P2PActivation;              // (Field not used. DO NOT REMOVE to keep memory alignment)
    int P2PStatus;                  // (Field not used. DO NOT REMOVE to keep memory alignment)
    float currentMaxRpm;            // (Field not used. DO NOT REMOVE to keep memory alignment)
    float mz[4];                    // (Field not used. DO NOT REMOVE to keep memory alignment)
    float fx[4];                    // (Field not used. DO NOT REMOVE to keep memory alignment)
    float fy[4];                    // (Field not used. DO NOT REMOVE to keep memory alignment)
    float slipRatio[4];             // Tyre slip ratio [FL, FR, RL, RR]
    float slipAngle[4];             // Tyre slip angle [FL, FR, RL, RR] in radians
    int tcinAction;                 // (Field not used. DO NOT REMOVE to keep memory alignment)
    int absInAction;                // (Field not used. DO NOT REMOVE to keep memory alignment)
    float suspensionDamage[4];      // (Field not used. DO NOT REMOVE to keep memory alignment)
    float tyteTemp[4];              // (Field not used. DO NOT REMOVE to keep memory alignment)
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