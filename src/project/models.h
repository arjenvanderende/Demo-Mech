#pragma once

/* Defines for calllist */
#define PLAYER_COCKPIT 1
#define PLAYER_UPPERARM_LEFT 2
#define PLAYER_LOWERARM_LEFT 3
#define PLAYER_UPPERARM_RIGHT 4
#define PLAYER_LOWERARM_RIGHT 5
#define PLAYER_PELVIS 6
#define PLAYER_UPPERLEG_LEFT 7
#define PLAYER_LOWERLEG_LEFT 8
#define PLAYER_UPPERLEG_RIGHT 9
#define PLAYER_LOWERLEG_RIGHT 10

#define OBJECT_LANDSCAPE 11

#define OBJECT_BARRACK   12
#define OBJECT_DOCK      13
#define OBJECT_LANDING   14
#define OBJECT_JEEP      15

typedef struct __MODEL {
  float position [2];
  float rotation;
  int   call_identifier;
} Model;