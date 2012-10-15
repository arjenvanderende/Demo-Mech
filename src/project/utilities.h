#pragma once

/* Disable deprecation warnings */
#pragma warning(disable:4996)

/* Includes */
#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glaux.h>

/* Calllist definitions for the textures */
#define TEXTURE_COUNT     14
#define TEXTURE_REFLECTION 0
#define TEXTURE_GRASS      1
#define TEXTURE_ROCK       2
#define TEXTURE_JEEP       3
#define TEXTURE_BARRACK_1  4
#define TEXTURE_BARRACK_2  5
#define TEXTURE_BARRACK_3  6
#define TEXTURE_BARRACK_4  7
#define TEXTURE_BARRACK_5  8
#define TEXTURE_NUCLEAR    9
#define TEXTURE_LANDING_1 10
#define TEXTURE_LANDING_2 11
#define TEXTURE_LANDING_3 12
#define TEXTURE_GALVANIC  13

/* Color constants */
static const float color_darkred[4]   = {0.4f, 0.0f, 0.0f, 1.0f};
static const float color_red[4]       = {1.0f, 0.0f, 0.0f, 1.0f};
static const float color_lightred[4]  = {1.0f, 0.5f, 0.5f, 1.0f};
static const float color_orange[4]    = {1.0f, 0.5f, 0.0f, 1.0f};
static const float color_yellow[4]    = {1.0f, 1.0f, 0.0f, 1.0f};
static const float color_green[4]     = {0.0f, 1.0f, 0.0f, 1.0f};
static const float color_darkgreen[4] = {0.0f, 0.5f, 0.0f, 1.0f};
static const float color_lightblue[4] = {0.5f, 0.5f, 1.0f, 1.0f};
static const float color_white[4]     = {1.0f, 1.0f, 1.0f, 1.0f};
static const float color_black[4]     = {0.1f, 0.1f, 0.1f, 1.0f};

/* Function forward declarations */
boolean LoadModelSGF(int, const char*);
boolean LoadModelA3D(int, const char*);
boolean LoadTextures(void);
void    SetMaterial(int);

/* Global variables */
GLuint g_iTextures[TEXTURE_COUNT];  // Texture identifier storage	