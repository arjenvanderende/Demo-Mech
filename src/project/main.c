#include "main.h"

/*
 * Program entry point
 */
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
  glutCreateWindow("Final Project");
  init();

  /* Declare GLUT callback functions */
  glutIdleFunc(update);
	glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutKeyboardFunc(processKeyboard);
  glutKeyboardUpFunc(processKeyboardUp);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseMotion);
  glutPassiveMotionFunc(processMouseMotion);
  
	glutMainLoop();
}

/*
 * Initialize the variables
 */
void init(void)
{
  GLfloat color[4] = { 0.75, 0.75, 0.75, 1.0 };
  glClearColor(0.75, 0.75, 0.75, 1.0);
  glEnable(GL_DEPTH_TEST);                            // Depth testing
  glEnable(GL_CULL_FACE);                             // One-sided faces

  glEnable(GL_FOG);                                   // Fog
  glFogi(GL_FOG_MODE, GL_EXP2);
  glHint(GL_FOG_HINT, GL_NICEST);
  glFogf(GL_FOG_DENSITY, 0.025f);
  glFogfv(GL_FOG_COLOR, color);

  // Enable texture & sphere mapping
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT0_POSITION);
  glLightfv(GL_LIGHT0, GL_AMBIENT , LIGHT0_AMBIENT);
  glLightfv(GL_LIGHT0, GL_DIFFUSE , LIGHT0_DIFFUSE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT0_SPECULAR);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION, LIGHT1_POSITION);
  glLightfv(GL_LIGHT1, GL_AMBIENT , LIGHT1_AMBIENT);
  glLightfv(GL_LIGHT1, GL_DIFFUSE , LIGHT1_DIFFUSE);
  glLightfv(GL_LIGHT1, GL_SPECULAR, LIGHT1_SPECULAR);

  // Initialize texture
  if( LoadTextures() )
  {
    MessageBox(NULL, "An error occured when loading the textures.", "Initialization Error", MB_ICONERROR);
    exit(1);
  }

  // Load models
  camera_init();
  if(
    player_init() ||
    LoadModelA3D(OBJECT_LANDSCAPE, "../models/landscape.a3d") ||
    LoadModelA3D(OBJECT_JEEP     , "../models/jeep.a3d") ||
    LoadModelA3D(OBJECT_DOCK     , "../models/dock.a3d") ||
    LoadModelA3D(OBJECT_LANDING  , "../models/landing.a3d") ||
    LoadModelA3D(OBJECT_BARRACK  , "../models/barrack.a3d"))
  {
    MessageBox(NULL, "An error occured when loading the models.", "Initialization Error", MB_ICONERROR);
    exit(1);
  }

  // Initialize models (buildings)
  srand(GetTickCount());
  g_pModels[0].call_identifier = OBJECT_LANDING;
  g_pModels[0].position[0]     = -50;
  g_pModels[0].position[1]     = -50;
  g_pModels[0].rotation        = rand() % 360;
  g_pModels[1].call_identifier = OBJECT_DOCK;
  g_pModels[1].position[0]     = -20;
  g_pModels[1].position[1]     = -60;
  g_pModels[1].rotation        = rand() % 360;
  g_pModels[2].call_identifier = OBJECT_BARRACK;
  g_pModels[2].position[0]     = -35;
  g_pModels[2].position[1]     = -20;
  g_pModels[2].rotation        = rand() % 360;
  g_pModels[3].call_identifier = OBJECT_BARRACK;
  g_pModels[3].position[0]     = -15;
  g_pModels[3].position[1]     = -40;
  g_pModels[3].rotation        = rand() % 360;
  g_pModels[4].call_identifier = OBJECT_DOCK;
  g_pModels[4].position[0]     = -35;
  g_pModels[4].position[1]     = -60;
  g_pModels[4].rotation        = rand() % 360;
  g_pModelJeep.call_identifier = OBJECT_JEEP;
  g_pModelJeep.position[0]     = 20.0f;
  g_pModelJeep.position[1]     =  0;
  g_pModelJeep.rotation        =  0;

  // Hide mouse cursor
  ShowCursor(FALSE);
  SetCursorPos(g_iWindowCenterX, g_iWindowCenterY);

  // Store current time
  g_dLastTime = Wallclock();
}

/*
 * Resize the game window
 */
void resize(int width, int height)
{
  /* Resize viewport */
  if( height == 0 ) height = 1;
  g_iWindowCenterX = width / 2;
  g_iWindowCenterY = height / 2;

  glViewport(0, 0, width, height);

  /* Calculate the aspect ratio of the window */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 1000.0);

  /* Move back the camera 6 units */
  glTranslatef(0, 0, -6.0f);

  /* Switch back to modelview */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*
 * Update game variables
 */
void update(void)
{
  // Calculate the elapsed time between frames
  double current_time = Wallclock();
  double delta_time = current_time - g_dLastTime;
  g_dLastTime = current_time;

  // Update if the game isn't paused
  if( !g_bPause )
  {
    // Update player movement
    player_update(delta_time);

    // Update jeep movement
    g_pModelJeep.rotation += 0.05;
    g_pModelJeep.position[0] = sin(ToRad(g_pModelJeep.rotation)) * 20.0f;
    g_pModelJeep.position[1] = cos(ToRad(g_pModelJeep.rotation)) * 20.0f;

    // Update camera movement
    camera_update(delta_time, player_getRotation());
  }

  // Redraw the scene
	glutPostRedisplay();
  Sleep(1);
}

/*
 * Render the current frame
 */
void render(void)
{
  int index;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Move the camera to the correct position
  camera_render();

  // Render the player
  player_render();
  glCallList(OBJECT_LANDSCAPE);
  
  // Render the buildings in 3 quadrants of the map
  for( index = 0; index < MODEL_COUNT; index++ )
  {
    glPushMatrix();
      glTranslatef(g_pModels[index].position[0], 0, g_pModels[index].position[1]);
      glRotatef(g_pModels[index].rotation, 0, 1.0f, 0);
      glCallList(g_pModels[index].call_identifier);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-g_pModels[index].position[0], 0, g_pModels[index].position[1]);
      glRotatef(g_pModels[index].rotation - 90, 0, 1.0f, 0);
      glCallList(g_pModels[index].call_identifier);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(g_pModels[index].position[0], 0, -g_pModels[index].position[1]);
      glRotatef(g_pModels[index].rotation + 90, 0, 1.0f, 0);
      glCallList(g_pModels[index].call_identifier);
    glPopMatrix();
  }
 
  // Render the jeep
  glTranslatef(g_pModelJeep.position[0], 0, g_pModelJeep.position[1]);
  glRotatef(g_pModelJeep.rotation - 90, 0, 1.0f, 0);
  glCallList(g_pModelJeep.call_identifier);

  // Flip the front- and backbuffer
  glutSwapBuffers();
}

/*
 * Process keyboard input, when key is pressed
 */
void processKeyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    // Pause the game
    case ' ':
    {
      g_bPause = !g_bPause;
      break;
    }
    // Move forward
    case 'w':
    {
      player_walkStart();
      break;
    }
    // Rotate left
    case 'a':
    {
      player_rotateLeft(TRUE);
      break;
    }
    // Rotate right
    case 'd':
    {
      player_rotateRight(TRUE);
      break;
    }
    // Toggle fog
    case 'f':
    {
      if( glIsEnabled(GL_FOG) == FALSE )
      {
        glEnable(GL_FOG);
        glClearColor(0.75, 0.75, 0.75, 1.0);                 // Blue background
      }
      else
      {
        glDisable(GL_FOG);
        glClearColor(0.25, 0.75, 1.0, 1.0);                 // Blue background
      }
    }
    // Toggle freelook
    case 'c':
    {
      camera_toggleFreelook();
      break;
    }
    // Quit
    case 27:
    {
      PostQuitMessage(0);
      break;
    }
  }
}

/*
 * Process keyboard input, when key is released
 */
void processKeyboardUp(unsigned char key, int x, int y)
{
  switch(key)
  {
    // Stop moving forward
    case 'w':
    {
      player_walkEnd();
      break;
    }
    // Stop rotating left
    case 'a':
    {
      player_rotateLeft(FALSE);
    }
    // Stop rotating right
    case 'd':
    {
      player_rotateRight(FALSE);
    }
  }
}

/*
 * Process mouse clicks
 */
void processMouse(int button, int state, int x, int y)
{
  if( button == GLUT_LEFT_BUTTON )
  {
    if( state == GLUT_DOWN )
    {
      player_fireStart();
    }
    else
    {
      player_fireEnd();
    }
  }
}

/*
 * Process mouse movement
 */
void processMouseMotion(int x, int y)
{
  float angle;

  // HACK: Don't process the next mouse motion
  // The next motion callback is caused by repositioning the
  // mouse to the middle of the screen
  g_bProcessMouse = !g_bProcessMouse;
  if(g_bProcessMouse == FALSE)
  {
    if( !g_bPause )
    {
      // Calculate mouse offset
      angle = (x - (g_iWindowCenterX - 4)) / 4;
      if( camera_getFreelook() == TRUE )
      {
        // In freelook rotate the camera directly
        camera_rotate(angle);
      }
      else
      {
        // In normal mode rotate the cockpit,
        // the camera will follow automatically
        player_rotateCockpit(angle);
      }
    }
    SetCursorPos(g_iWindowCenterX, g_iWindowCenterY);
  }
}