#pragma once

/* Disable deprecation warnings */
#pragma warning(disable:4996)

/* Includes */
#include <GL/glut.h>
#include <util/glutil.h>
#include <stdio.h>
#include "utilities.h"
#include "camera.h"
#include "player.h"
#include "models.h"

/* Function forward declaration */
int  main(int, char **);
void init(void);
void update(void);
void render(void);
void resize(int, int);
void processKeyboard(unsigned char, int, int);
void processKeyboardUp(unsigned char, int, int);
void processMouse(int, int, int, int);
void processMouseMotion(int, int);

/* Light constants */
const GLfloat LIGHT0_POSITION[] = { 50.0f,  150.0f, 50.0f, 1.0f };
const GLfloat LIGHT0_DIFFUSE[]  = { 1.0f,  1.0f, 1.0f, 1.0f };
const GLfloat LIGHT0_AMBIENT[]  = { 0.1f,  0.1f, 0.1f, 1.0f };
const GLfloat LIGHT0_SPECULAR[] = { 1.0f,  1.0f, 1.0f, 1.0f };

const GLfloat LIGHT1_POSITION[] = { -50.0f,  -50.0f, -50.0f, 1.0f };
const GLfloat LIGHT1_DIFFUSE[]  = { 1.0f,  1.0f, 1.0f, 1.0f };
const GLfloat LIGHT1_AMBIENT[]  = { 0.1f,  0.1f, 0.1f, 1.0f };
const GLfloat LIGHT1_SPECULAR[] = { 1.0f,  1.0f, 1.0f, 1.0f };


/* Global variables */
boolean g_bPause = FALSE;         // Is the game paused?
boolean g_bProcessMouse = FALSE;  // Process mouse motion
double  g_dLastTime;              // Time of last frame
int     g_iWindowCenterX = 400;   // X-center of the screen (for mouse position)
int     g_iWindowCenterY = 300;   // Y-center of the screen (for mouse position)

#define MODEL_COUNT 5             // The number of buildings
Model   g_pModels[MODEL_COUNT],   // The models for the buildings in the scene
        g_pModelJeep;             // The model for the jeep