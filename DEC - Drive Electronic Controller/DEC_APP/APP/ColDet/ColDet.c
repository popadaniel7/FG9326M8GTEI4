#include "ColDet.h"
#include "Ain.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "time.h"
#include "Dem.h"

#define TIME_TO_COLLISION_WARN   2.0
#define TIME_TO_COLLISION_BRAKE  1.0
#define DISTANCE_STABLE_THRESHOLD_CM  1

static uint32 ColDet_MainCounter = 0u;
CollisionState ColDet_CanTx_CollisionState = COLLISION_SAFE;
uint8 ColDet_CanTx_BrakeLevel = 0;  // current brake application level (as a percentage)=
uint8 ColDet_CanTx_IrSenStat = 0u;
uint8 ColDet_CanRx_CalculatedSpeed = 0u;

void ColDet_MainFunction(void);

void ColDet_MainFunction(void)
{
    // For simulation, define a max distance that indicates "no obstacle"
    const uint32 NO_OBSTACLE_DISTANCE = 100000;  // 1000 m (just a large number)
    // Variables to hold sensor readings
    uint32 distanceCm = NO_OBSTACLE_DISTANCE;
    uint32 lastDistanceCm = NO_OBSTACLE_DISTANCE;
    double speed = 0.0;
    double ttc = 0.0;
    // 1. Read current sensor values
    speed = ColDet_CanRx_CalculatedSpeed;         // current vehicle speed
    distanceCm = IRSensorValue; // distance to obstacle from IR sensor
    // 2. Filter sensor noise: ignore tiny fluctuations in distance
    if (abs(distanceCm - lastDistanceCm) <= DISTANCE_STABLE_THRESHOLD_CM)
    {
        // If the distance changed by 1 cm or less, treat it as essentially unchanged
        distanceCm = lastDistanceCm;
    }
    else
    {
        /* Do nothing. */
    }

    lastDistanceCm = distanceCm;
    // Determine if an obstacle is detected within sensor range
    bool objectDetected = (distanceCm < NO_OBSTACLE_DISTANCE);
    // 3. Calculate Time-to-Collision (TTC) if object is detected and car is moving
    if (objectDetected && speed > 0.0)
    {
        // Convert distance and speed to consistent units (e.g., meters and m/s) if needed
        // Here assuming distanceCm is in cm and speed is in cm/s for simplicity
        ttc = distanceCm / speed;
    }
    else
    {
        ttc = 9999;  // No collision imminent (no object or not moving)
    }
    // (These represent the distances at which TTC equals the thresholds)
    // 5. State machine: decide state transitions and actions
    switch (ColDet_CanTx_CollisionState)
    {
        case COLLISION_SAFE:
            if (objectDetected && ttc <= TIME_TO_COLLISION_BRAKE)
            {
                // Obstacle extremely close – immediate emergency brake
                ColDet_CanTx_CollisionState = COLLISION_BRAKE;
                ColDet_CanTx_BrakeLevel = 100;            // full braking
                // (In a real system, also trigger audible/visual collision alert)
            }
            else if (objectDetected && ttc <= TIME_TO_COLLISION_WARN) {
                // Obstacle within warning range – enter warning state
                ColDet_CanTx_CollisionState = COLLISION_WARNING;
                // Initialize gradual braking
                ColDet_CanTx_BrakeLevel = 30;             // e.g., 30% brake to start slowing
                // (Also perhaps trigger a warning light/sound for driver)
            }
            else
            {
                // No threat – stay in SAFE
                ColDet_CanTx_CollisionState = COLLISION_SAFE;
                ColDet_CanTx_BrakeLevel = 0;
            }
            break;

        case COLLISION_WARNING:
            if (!objectDetected || ttc > TIME_TO_COLLISION_WARN)
            {
                // Obstacle gone or now far away – go back to SAFE
                ColDet_CanTx_CollisionState = COLLISION_SAFE;
                ColDet_CanTx_BrakeLevel = 0;
            }
            else if (ttc <= TIME_TO_COLLISION_BRAKE)
            {
                // Situation worsened – switch to emergency brake
                ColDet_CanTx_CollisionState = COLLISION_BRAKE;
                ColDet_CanTx_BrakeLevel = 100;
            }
            else
            {
                // Still in warning range – continue gradual braking
                // Increase brake pressure stepwise but avoid harsh braking
                if (ColDet_CanTx_BrakeLevel < 80)
                {       // cap gradual braking at ~80%
                    ColDet_CanTx_BrakeLevel += 10;        // ramp up brakes by 10% (adjust step as needed)
                }
                else
                {
                    /* Do nothing. */
                }
                // (The obstacle is still in front but not extremely close, so we are slowing down smoothly)
            }
            break;

        case COLLISION_BRAKE:
            if (!objectDetected || ttc > TIME_TO_COLLISION_WARN)
            {
                // Obstacle cleared or moved far – return to SAFE
                ColDet_CanTx_CollisionState = COLLISION_SAFE;
                // Optionally, if the car is almost stopped, keep brake applied a moment to be sure
                ColDet_CanTx_BrakeLevel = 0;
            }
            else if (ttc > TIME_TO_COLLISION_BRAKE && ttc <= TIME_TO_COLLISION_WARN)
            {
                // Obstacle still ahead but not an immediate collision (TTC improved to >1s)
                ColDet_CanTx_CollisionState = COLLISION_WARNING;
                // Rather than full release, maintain moderate braking as a precaution
                ColDet_CanTx_BrakeLevel = 50;         // e.g., reduce braking to 50%
            }
            else
            {
                // Remain in emergency brake state (ttc still <= 1s)
                ColDet_CanTx_CollisionState = COLLISION_BRAKE;
                ColDet_CanTx_BrakeLevel = 100;
            }
            break;
    }

    ColDet_CanTx_IrSenStat = ColDet_CanTx_CollisionState;

    ColDet_MainCounter++;
}
