#include "camera.h"

/*
 * Initialize camera variables
 */
void camera_init(void)
{
  g_fCameraAngle = 135.0f;;
  g_bFreelook = FALSE;
  g_fCameraDelay = CAMERA_DELAY;
}

/*
 * Update the camera information
 *   This method updates the rotation of the camera when a certain
 *   amount of time has elapsed. This function can only be called
 *   when freelook is disabled. Otherwise use camera_rotate when
 *   the camera is rotated.
 */
void camera_update(double time_elapsed, float angle)
{
  // Is freelook disabled?
  if( g_bFreelook == FALSE )
  {
    // Wait before CAMERA_DELAY seconds before the rotation kicks in
    g_fCameraDelay -= time_elapsed;
    if( g_fCameraDelay <= 0 )
    {
      // Determine rotation speed based on how far the camera has to move
      float delta_angle  = angle - g_fCameraAngle;
      g_fCameraVelocity += time_elapsed * (delta_angle / CAMERA_ACCELERATION_MODIFIER);
      g_fCameraAngle += g_fCameraVelocity;

      // Check if the rotation is finished
      if((delta_angle >  0.005 && g_fCameraAngle > angle) ||
         (delta_angle < -0.005 && g_fCameraAngle < angle))
      {
        g_fCameraDelay = CAMERA_DELAY;
        g_fCameraVelocity = 0;
        g_fCameraAngle = angle;
      }
    }
  }
}

/*
 * Render the camera
 *   This does not actually render anything, but instead rotates the scene
 *   to reflect the position of the camera.
 */
void camera_render()
{
  float* position = player_getPosition(),
         x =  sin(ToRad(g_fCameraAngle)) + position[0],
         z = -cos(ToRad(g_fCameraAngle)) + position[1];
  gluLookAt( x, 2.5f, z, position[0], 2.5f, position[1], 0, 1.0f, 0);
}

/*
 * Toggle freelook on and off
 */
void camera_toggleFreelook()
{
  g_bFreelook = !g_bFreelook;
  if(g_bFreelook == FALSE)
  {
    while( g_fCameraAngle >  180.0f ) g_fCameraAngle -= 360.0f;
    while( g_fCameraAngle < -180.0f ) g_fCameraAngle += 360.0f;
    g_fCameraVelocity = 0;
  }
}


/*
 * Gets whether freelook is enabled
 */
boolean camera_getFreelook(void)
{
  return g_bFreelook;
}

/*
 * Rotate the camera 'angle' degrees, directly. Only use this
 * function when freelook is enabled. Otherwise use update when
 * the camera movement has to be updated.
 */
void camera_rotate(float angle)
{
  g_fCameraAngle -= angle;
}