#pragma once
#include <GL/glut.h>
#include <util/glutil.h>
#include <stdio.h>
#include "player.h"

/* Constant definitions */
#define CAMERA_ACCELERATION_MODIFIER 100  // Acceleration multiplier
#define CAMERA_MAX_VELOCITY 2.5f          // Maximum velocity of the camera
#define CAMERA_DELAY 0.25f                // Delay before the camera follows the player

/* Forward declarations */
void camera_init(void);
void camera_update(double, float);
void camera_render(void);
void camera_rotate(float);
void camera_toggleFreelook(void);
boolean camera_getFreelook(void);

boolean g_bFreelook;                      // Is freelook enabled?
float   g_fCameraVelocity;                // Velocity of the camera
float   g_fCameraAngle;                   // Angle of the camera
float   g_fCameraDelay;                   // Delay counter