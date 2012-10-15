#include "player.h"

boolean g_bPlayerWalk,          // Should the player walk?
        g_bPlayerRotateLeft,    // Should the player rotate left?
        g_bPlayerRotateRight;   // Should the player rotate right?

/*
 * Initialize the mech model for the player
 */
boolean player_init(void)
{
  // Initialize variables
  g_fPosition[0] = 25;
  g_fPosition[1] = 25;
  g_fAngle = -135.0f;
  g_fPelvisHeight = 1.3f;
  g_bPlayerWalk = FALSE;
  g_faFireAction = faIdle;
  g_waWalkAction = waIdle;
  g_wfFoot = wfLeft;

  // Load models and create display lists and return errorcode
  return
    LoadModelA3D(PLAYER_COCKPIT         , "../models/mech/cockpit.a3d")         ||
    LoadModelA3D(PLAYER_UPPERARM_LEFT   , "../models/mech/arm_left_upper.a3d")  ||
    LoadModelA3D(PLAYER_LOWERARM_LEFT   , "../models/mech/arm_left_lower.a3d")  ||
    LoadModelA3D(PLAYER_UPPERARM_RIGHT  , "../models/mech/arm_right_upper.a3d") ||
    LoadModelA3D(PLAYER_LOWERARM_RIGHT  , "../models/mech/arm_right_lower.a3d") ||
    LoadModelA3D(PLAYER_PELVIS          , "../models/mech/pelvis.a3d")          ||
    LoadModelA3D(PLAYER_UPPERLEG_LEFT   , "../models/mech/leg_left_upper.a3d")  ||
    LoadModelA3D(PLAYER_LOWERLEG_LEFT   , "../models/mech/leg_left_lower.a3d")  ||
    LoadModelA3D(PLAYER_UPPERLEG_RIGHT  , "../models/mech/leg_right_upper.a3d") ||
    LoadModelA3D(PLAYER_LOWERLEG_RIGHT  , "../models/mech/leg_right_lower.a3d");
}

/*
 * Update the model of the mech
 */
void player_update(float time_elapsed)
{
  // Update fire animation
  player_fireUpdate(time_elapsed);

  // Update walk animation
  if( g_wfFoot == wfLeft )
    player_walkUpdate(time_elapsed, &g_fAngleUpperlegLeft, &g_fAngleLowerlegLeft, &g_fAngleUpperlegRight, &g_fAngleLowerlegRight);
  else
    player_walkUpdate(time_elapsed, &g_fAngleUpperlegRight, &g_fAngleLowerlegRight, &g_fAngleUpperlegLeft, &g_fAngleLowerlegLeft);
}

/*
 * Render the model of the mech
 */
void player_render(void)
{
  glPushMatrix();
    // Move the pelvis to the correct height
    glTranslatef(g_fPosition[0], g_fPelvisHeight, g_fPosition[1]);
    glRotatef(g_fAngle, 0, 1.0f, 0);
    glCallList(PLAYER_PELVIS);

    // Render the left leg
    glPushMatrix();
      glTranslatef(-0.335f, 0.0f, 0.005f);
      glRotatef(g_fAngleUpperlegLeft, 1.0f, 0, 0);
      glCallList(PLAYER_UPPERLEG_LEFT);
      glTranslatef(-0.115, -0.395, -0.455);
      glRotatef(g_fAngleLowerlegLeft, 1.0f, 0, 0);
      glCallList(PLAYER_LOWERLEG_LEFT);
    glPopMatrix();

    // Render the right leg
    glPushMatrix();
      glTranslatef(0.335f, 0.0f, 0.005f);
      glRotatef(g_fAngleUpperlegRight, 1.0f, 0, 0);
      glCallList(PLAYER_UPPERLEG_RIGHT);
      glTranslatef(0.115, -0.395, -0.455);
      glRotatef(g_fAngleLowerlegRight, 1.0f, 0, 0);
      glCallList(PLAYER_LOWERLEG_RIGHT);
    glPopMatrix();

    // Render the cockpit
    glPushMatrix();
      glTranslatef(0, 0.2f, 0);
      glRotatef(g_fAngleCockpit, 0, 1.0f, 0);
      glCallList(PLAYER_COCKPIT);

      // Render the left cannon arm
      glPushMatrix();
        glTranslatef(-0.455, 0.465, -0.497);
        glRotatef(g_fAngleUpperarm, 0, 0, 1.0f);
        glCallList(PLAYER_UPPERARM_LEFT);
        
        glTranslatef(-0.180, -0.328, 0);
        glRotatef(g_fAngleLowerarm, 0, 0, 1.0f);
        glCallList(PLAYER_LOWERARM_LEFT);
      glPopMatrix();
      
      // Render the right cannon arm
      glPushMatrix();
        glTranslatef(0.455, 0.465, -0.497);
        glRotatef(-g_fAngleUpperarm, 0, 0, 1.0f);
        glCallList(PLAYER_UPPERARM_RIGHT);

        glTranslatef(0.180, -0.328, 0);
        glRotatef(-g_fAngleLowerarm, 0, 0, 1.0f);
        glCallList(PLAYER_LOWERARM_RIGHT);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

/*
 * Set if the mech should rotate left during walking
 */
void player_rotateLeft(boolean shouldRotate)
{
  g_bPlayerRotateLeft = shouldRotate;
}

/*
 * Set if the mech should rotate right during walking
 */
void player_rotateRight(boolean shouldRotate)
{
  g_bPlayerRotateRight = shouldRotate;
}

/*
 * Rotate the cockpit of the mech
 */
void player_rotateCockpit(float angle) {
  g_fAngleCockpit += angle;
}

/*
 * Get the current (absolute) rotation of the cockpit.
 * This is used when positioning the camera in freelook.
 */
float player_getRotation(void)
{
  return -(g_fAngle + g_fAngleCockpit);
}

/*
 * Start walking
 */
void player_walkStart()
{
  g_bPlayerWalk = TRUE;
}

/*
 * Move the player 'distance' units in the current direction
 */
void player_position_update(float distance)
{
  g_fPosition[0] += sin(ToRad(g_fAngle)) * distance;
  g_fPosition[1] += cos(ToRad(g_fAngle)) * distance;
}

/*
 * Update the walking animation.
 * If the player is still walking at the end of the animation, the
 * animation cycle will be adapted to keep a continuous motion.
 */
void player_walkUpdate(float time_elapsed, float *AngleUpperlegLeft, float *AngleLowerlegLeft, float *AngleUpperlegRight, float *AngleLowerlegRight)
{
  // Update walk animation
  switch( g_waWalkAction )
  {
    case waIdle:
    {
      if( g_bPlayerWalk == TRUE )
      {
        g_waWalkAction = waState1;
      }
      break;
    }
    case waState1:
    {
      // Update leg rotation values
      *AngleUpperlegLeft += 30 * time_elapsed;
      *AngleLowerlegLeft -= 100 * time_elapsed;

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft >= 15.0f )
      {
        *AngleUpperlegLeft =  15.0f;
        *AngleLowerlegLeft = -50.0f;
        g_waWalkAction = waState2;
      }
      break;
    }
    case waState2:
    {
      // Update leg rotation values
      *AngleUpperlegLeft -=  110 * time_elapsed; 

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft < -40.0f )
      {
        g_waWalkAction = waState3;
      }
      break;
    }
    case waState3:
    {
      // Update leg rotation values
      *AngleUpperlegLeft -= 110 * time_elapsed; 
      *AngleLowerlegLeft += 290 * time_elapsed;

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft <= -95.0f )
      {
        *AngleUpperlegLeft = -95.0f;
        *AngleLowerlegLeft =  95.0f;
        g_waWalkAction = waState4;
      }
      break;
    }
    case waState4:
    {
      // Update leg rotation values
      *AngleUpperlegLeft += 95 * time_elapsed; 
      *AngleLowerlegLeft -= 95 * time_elapsed;
      *AngleUpperlegRight += 40 * time_elapsed; 
      *AngleLowerlegRight -= 80 * time_elapsed;

      // Move the pelvis to simulate 'bobbing'
      g_fPelvisHeight = 1.3f + sin((1 - *AngleLowerlegLeft / 95) * 3.14156) * 0.195;
      player_position_update(PLAYER_MOVE_SPEED);
      if( g_bPlayerRotateLeft )  { g_fAngle += PLAYER_ROTATION_SPEED; }
      if( g_bPlayerRotateRight ) { g_fAngle -= PLAYER_ROTATION_SPEED; }

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft >= -47.5 ) {
        *AngleUpperlegRight = 20; 
        *AngleLowerlegRight = -40;
        
        // Determine if the mech stop or continues walking
        if(g_bPlayerWalk == FALSE) { g_waWalkAction = waState5; }
        else { g_waWalkAction = waState6; }
      }
      break;
    }
    case waState5:
    {
      // Update leg rotation values
      *AngleUpperlegLeft += 95 * time_elapsed; 
      *AngleLowerlegLeft -= 95 * time_elapsed;
      *AngleUpperlegRight -= 40 * time_elapsed; 
      *AngleLowerlegRight += 80 * time_elapsed;

      // Move the pelvis to simulate 'bobbing'
      g_fPelvisHeight = 1.3f + sin((1 - *AngleLowerlegLeft / 95) * 3.14156) * 0.195;
      player_position_update(PLAYER_MOVE_SPEED);

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft >= 0 )
      {
        g_fPelvisHeight = 1.3f;
        *AngleUpperlegLeft = 0;
        *AngleLowerlegLeft = 0;
        *AngleUpperlegRight = 0; 
        *AngleLowerlegRight = 0;
        g_waWalkAction = waIdle;
        if( g_wfFoot == wfLeft ) { g_wfFoot = wfRight; }
        else { g_wfFoot = wfLeft; }
      }
      break;
    }
    case waState6:
    {
      // Update leg rotation values
      *AngleUpperlegLeft += 95 * time_elapsed; 
      *AngleLowerlegLeft -= 95 * time_elapsed;
      *AngleUpperlegRight -= 230 * time_elapsed; 

      // Move the pelvis to simulate 'bobbing'
      g_fPelvisHeight = 1.3f + sin((1 - *AngleLowerlegLeft / 95) * 3.14156) * 0.195;
      player_position_update(PLAYER_MOVE_SPEED);
      if( g_bPlayerRotateLeft )  { g_fAngle += PLAYER_ROTATION_SPEED; }
      if( g_bPlayerRotateRight ) { g_fAngle -= PLAYER_ROTATION_SPEED; }

      // Is the trigger for the next state reached?
      if( *AngleUpperlegRight <= -37.5 )
      {
        // Cycle animation
        g_waWalkAction = waState7;
      }
      break;
    }
    case waState7:
    {
      // Update leg rotation values
      *AngleUpperlegLeft += 95 * time_elapsed; 
      *AngleLowerlegLeft -= 95 * time_elapsed;
      *AngleUpperlegRight -= 230 * time_elapsed; 
      *AngleLowerlegRight += 540 * time_elapsed;

      // Move the pelvis to simulate 'bobbing'
      g_fPelvisHeight = 1.3f + sin((1 - *AngleLowerlegLeft / 95) * 3.14156) * 0.195;
      player_position_update(PLAYER_MOVE_SPEED);
      if( g_bPlayerRotateLeft )  { g_fAngle += PLAYER_ROTATION_SPEED; }
      if( g_bPlayerRotateRight ) { g_fAngle -= PLAYER_ROTATION_SPEED; }

      // Is the trigger for the next state reached?
      if( *AngleUpperlegLeft >= 0 )
      {
        *AngleUpperlegLeft  =   0;
        *AngleLowerlegLeft  =   0;
        *AngleUpperlegRight = -95;
        *AngleLowerlegRight =  95;
        g_fPelvisHeight = 1.3f;
       
        // Switch to other foot
        if( g_wfFoot == wfLeft ) { g_wfFoot = wfRight; }
        else { g_wfFoot = wfLeft; }

        // Cycle animation
        g_waWalkAction = waState4;
      }
      break;
    }
  }
}

/*
 * Stop walking as soon as walk animation is over
 */
void player_walkEnd()
{
  g_bPlayerWalk = FALSE;
}

/*
 * Prepare cannons to fire
 */
void player_fireStart()
{
  if(g_faFireAction == faIdle || g_faFireAction == faUnprepare)
  {
    g_faFireAction = faPrepare;
  }
}

/*
 * Update the fire animation
 * Prepare the cannons by raising the arm, before allowing the model
 * to fire from its cannons.
 */
void player_fireUpdate(float time_elapsed)
{
  // Update cannon animation
  switch( g_faFireAction )
  {
    // Raise the cannons
    case faPrepare:
    {
      g_fAngleUpperarm -= 45 * time_elapsed;
      g_fAngleLowerarm += 55 * time_elapsed;
      if( g_fAngleUpperarm <= -45.0f )
      {
        g_fAngleUpperarm = -45.0f;
        g_fAngleLowerarm =  55.0f;
        g_faFireAction = faFire;
      }
      break;
    }
    // Lower the cannons
    case faUnprepare:
    {
      g_fAngleUpperarm += 45 * time_elapsed;
      g_fAngleLowerarm -= 55 * time_elapsed;
      if( g_fAngleUpperarm >= 0 )
      {
        g_fAngleUpperarm = 0;
        g_fAngleLowerarm = 0;
        g_faFireAction = faIdle;
      }
      break;
    }
  }
}

/*
 * Prepare cannons after firing
 */
void player_fireEnd()
{
  if(g_faFireAction == faPrepare || g_faFireAction == faFire)
  {
    g_faFireAction = faUnprepare;
  }
}

/*
 * Get the position of the player
 */
float* player_getPosition()
{
  return &g_fPosition[0];
}