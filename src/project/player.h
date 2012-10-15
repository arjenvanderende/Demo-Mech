#pragma once
#include <math.h>
#include "utilities.h"
#include "models.h"
#include "camera.h"

/* Constant definiations */
#define PLAYER_MOVE_SPEED     time_elapsed
#define PLAYER_ROTATION_SPEED time_elapsed * 40

/* Macro definitions */
#define ToRad(x) x * 0.017453292519943295769236907684886

/* Enumerations */
enum PlayerFireAction { faIdle, faPrepare, faFire, faUnprepare } g_faFireAction;
enum PlayerWalkAction { waIdle, waState1, waState2, waState3, waState4, waState5, waState6, waState7 } g_waWalkAction;
enum PlayerWalkFoot   { wfLeft, wfRight } g_wfFoot;

/* Forward declarations */
boolean player_init(void);
void    player_update(float);
void    player_render(void);
void    player_rotateLeft(boolean);
void    player_rotateRight(boolean);
void    player_rotateCockpit(float);
void    player_fireStart(void);
void    player_fireUpdate(float);
void    player_fireEnd(void);
void    player_walkStart(void);
void    player_walkUpdate(float, float*, float*, float*, float*);
void    player_walkEnd(void);
float*  player_getPosition(void);
float   player_getRotation(void);

/* Player model variables */
float g_fPosition[2],         // The position of the player
      g_fAngle,               // The angle of the player
      g_fPelvisHeight,        // The height of the pelvis object
      g_fAngleCockpit,        // Angle of the cockpit
      g_fAngleUpperarm,       // Angle of the upperarms
      g_fAngleLowerarm,       // Angle of the lowerarms
      g_fAngleUpperlegLeft,   // Angle of the upper left leg
      g_fAngleLowerlegLeft,   // Angle of the lower left leg
      g_fAngleUpperlegRight,  // Angle of the upper right leg
      g_fAngleLowerlegRight;  // Angle of the lower right leg