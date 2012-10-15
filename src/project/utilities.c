#include "utilities.h"

/*
 * Load a bitmap image
 */
AUX_RGBImageRec* LoadBMP(char *filename)
{
	FILE *file = NULL;
  
  // Open the file, to see if it exists
	file = fopen(filename,"r");	
	if( file )
	{
		fclose(file);

    // Load the bitmap image
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

/*
 * Load all the necessary textures
 */
boolean LoadTextures()
{
  int index;
  boolean status = TRUE;
  AUX_RGBImageRec* textureImage[TEXTURE_COUNT];
  memset(textureImage, 0, sizeof(AUX_RGBImageRec*) * TEXTURE_COUNT);

  // Load textures
  textureImage[TEXTURE_REFLECTION] = LoadBMP("../textures/reflection.bmp");
  textureImage[TEXTURE_GRASS]      = LoadBMP("../textures/grass.bmp");
  textureImage[TEXTURE_ROCK]       = LoadBMP("../textures/rock.bmp");
  textureImage[TEXTURE_JEEP]       = LoadBMP("../textures/jeep.bmp");
  textureImage[TEXTURE_BARRACK_1]  = LoadBMP("../textures/metal_door.bmp");
  textureImage[TEXTURE_BARRACK_2]  = LoadBMP("../textures/metal_door_windows.bmp");
  textureImage[TEXTURE_BARRACK_3]  = LoadBMP("../textures/metal_windows.bmp");
  textureImage[TEXTURE_BARRACK_4]  = LoadBMP("../textures/metal.bmp");
  textureImage[TEXTURE_BARRACK_5]  = LoadBMP("../textures/metal_door_2.bmp");
  textureImage[TEXTURE_NUCLEAR]    = LoadBMP("../textures/metal_nuclear.bmp");
  textureImage[TEXTURE_LANDING_1]  = LoadBMP("../textures/metal_windows_2.bmp");
  textureImage[TEXTURE_LANDING_2]  = LoadBMP("../textures/metal_sign.bmp");
  textureImage[TEXTURE_LANDING_3]  = LoadBMP("../textures/stripe.bmp");
  textureImage[TEXTURE_GALVANIC]   = LoadBMP("../textures/galvanic.bmp");

  // Check if the texture are correctly loaded
  if(textureImage[0] && textureImage[1] && textureImage[2])
  {
    status = FALSE;

    // Generate texture identifiers
    glGenTextures(TEXTURE_COUNT, &g_iTextures[0]);

    // Create the texture
    for( index = 0; index < TEXTURE_COUNT; index++ )
    {
      // Generate the texture with linear filtering
      glBindTexture(GL_TEXTURE_2D, g_iTextures[index]);
//      glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImage[index]->sizeX, textureImage[index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage[index]->data);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, textureImage[index]->sizeX, textureImage[index]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, textureImage[index]->data);
    }
  }
  
  // Free (unnecessary) reserved memory
  for( index = 0; index < TEXTURE_COUNT; index++ )
  {
    if (textureImage[index])
	  {
		  if (textureImage[index]->data)
        free(textureImage[index]->data);
		  free(textureImage[index]);
	  }
  }  
  return status;
}


/*
 * Create a new display list for the SGF-model that is stored in the
 * specified file.
 */
boolean LoadModelSGF(int index, const char* filename) {
  FILE* stream;                           // Handle to input-file stream
  char mode[16];                          // Buffer for render mode from input-file
  int  i, j,
       vertex_count,                      // Vertex count of the model
       glMode;                            // OpenGL render mode for the model
  float buffer[6];                        // Buffer for vertex data
  boolean error = FALSE;                  // Has an error occured while loading the model?

  // Open the file
  stream = fopen(filename, "r");
  if( stream == NULL ) {
    return TRUE;
  }

  // Start a new call list
  glNewList(index, GL_COMPILE);
  fseek(stream, 0, SEEK_SET);
  while( fscanf(stream, "%s%d", mode, &vertex_count) != EOF ) {
    
    // Determine render mode
    if     ( strcmp(mode, "POINTS")         == 0) { glMode = GL_POINTS; }
    else if( strcmp(mode, "LINES")          == 0) { glMode = GL_LINES; }
    else if( strcmp(mode, "LINE_STRIP")     == 0) { glMode = GL_LINE_STRIP; }
    else if( strcmp(mode, "LINE_LOOP")      == 0) { glMode = GL_LINE_LOOP; }
    else if( strcmp(mode, "TRIANGLES")      == 0) { glMode = GL_TRIANGLES; }
    else if( strcmp(mode, "QUADS")          == 0) { glMode = GL_QUADS; }
    else if( strcmp(mode, "TRIANGLE_STRIP") == 0) { glMode = GL_TRIANGLE_STRIP; }
    else if( strcmp(mode, "TRIANGLE_FAN")   == 0) { glMode = GL_TRIANGLE_FAN; }
    else if( strcmp(mode, "QUAD_STRIP")     == 0) { glMode = GL_QUAD_STRIP; }
    else if( strcmp(mode, "POLYGON")        == 0) { glMode = GL_POLYGON; }
    else {
      error = TRUE; break;
    }

    // Set material color
    for(i = 0; i < 4; i++) {
      if(fscanf(stream, "%f", buffer + i) == EOF) { error = TRUE; break; }
    }
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, buffer);
    glBegin(glMode);
      // Consume vertex position & normal data
      for(i = 0; i < vertex_count; i++) {
        for(j = 0; j < 6; j++) {
          if(fscanf(stream, "%f", buffer + j) == EOF) { error = TRUE; break; }
        }
        glNormal3fv(buffer +3);
        glVertex3fv(buffer);
      }
    glEnd();
  }
  glEndList();
  fclose(stream);

  // Delete the list if an error occured
  if(error == TRUE)
  {
    glDeleteLists(index, 1);
    return TRUE;
  }
  return FALSE;
}

/*
 * Create a new display list for the A3F-model that is stored in the
 * specified file. A3F is a custom file format.
 */
boolean LoadModelA3D(int index, const char* filename) {
  FILE* stream;                           // Handle to input-file stream
  char mode[16];                          // Buffer for render mode from input-file
  int  i, j, k,
       triangle_count,                    // Triangle count of the model
       prevMatId,                         // Previous material ID of a face
       matId,                             // Material ID of a face
       glMode;                            // OpenGL render mode for the model
  float buffer[8];                        // Buffer for vertex data
  boolean error = FALSE;                  // Has an error occured while loading the model?

  // Open the file
  stream = fopen(filename, "r");
  if( stream == NULL ) {
    return TRUE;
  }

  // Start a new call list
  glNewList(index, GL_COMPILE);
  fseek(stream, 0, SEEK_SET);
  while( fscanf(stream, "%s%d", mode, &triangle_count) != EOF ) {
    
    // Determine render mode
    if     ( strcmp(mode, "POINTS")         == 0) { glMode = GL_POINTS; }
    else if( strcmp(mode, "LINES")          == 0) { glMode = GL_LINES; }
    else if( strcmp(mode, "LINE_STRIP")     == 0) { glMode = GL_LINE_STRIP; }
    else if( strcmp(mode, "LINE_LOOP")      == 0) { glMode = GL_LINE_LOOP; }
    else if( strcmp(mode, "TRIANGLES")      == 0) { glMode = GL_TRIANGLES; }
    else if( strcmp(mode, "QUADS")          == 0) { glMode = GL_QUADS; }
    else if( strcmp(mode, "TRIANGLE_STRIP") == 0) { glMode = GL_TRIANGLE_STRIP; }
    else if( strcmp(mode, "TRIANGLE_FAN")   == 0) { glMode = GL_TRIANGLE_FAN; }
    else if( strcmp(mode, "QUAD_STRIP")     == 0) { glMode = GL_QUAD_STRIP; }
    else if( strcmp(mode, "POLYGON")        == 0) { glMode = GL_POLYGON; }
    else {
      error = TRUE; break;
    }

    prevMatId = -1;
    glBegin(glMode);
        // Consume vertex position & normal data
        triangle_count /= 3;
        for(i = 0; i < triangle_count; i++) {

          // Get material id
          if(fscanf(stream, "%d\n", &matId) == EOF) { error = TRUE; break; }
          if(prevMatId != matId) {
            glEnd();
              SetMaterial(matId);
              prevMatId = matId;
            glBegin(glMode);
          }
          for(k = 0; k < 3; k++)
          {
            // Get vertices and normal vectors
            for(j = 0; j < 8; j++) {
              if(fscanf(stream, "%f", buffer + j) == EOF) { error = TRUE; break; }
            }
            glNormal3fv(buffer +3);
            glTexCoord2fv(buffer +6);
            glVertex3fv(buffer);
          }
        }
      glEnd();
  }
  glEndList();
  fclose(stream);

  // Delete the list if an error occured
  if(error == TRUE)
  {
    glDeleteLists(index, 1);
    return TRUE;
  }
  return FALSE;
}

/*
 * Setup the material for the given identifier
 */
void SetMaterial(int materialID) {
  // Update new material settings
  switch(materialID)
  {
    // Reflective metal
    case 1:
    {
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_TEXTURE_GEN_S); glEnable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_REFLECTION]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Red metal
    case 2:
    {
      glDisable(GL_TEXTURE_2D);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_darkred);
      break;
    }
    // Rocket top
    case 3:
    {
      glDisable(GL_TEXTURE_2D);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_black);
      break;
    }
    // Rocket hull
    case 4:
    {
      glDisable(GL_TEXTURE_2D);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Reflective cockpit glass
    case 5:
    {
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_TEXTURE_GEN_S); glEnable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_REFLECTION]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_orange);
      break;
    }
    // Landscape grass
    case 6:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_GRASS]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Landscape rock
    case 7:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_ROCK]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Jeep material
    case 8:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_JEEP]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal doors
    case 9:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_BARRACK_1]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal windows with windows
    case 10:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_BARRACK_2]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal windows
    case 11:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_BARRACK_3]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal
    case 12:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_BARRACK_4]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal doors
    case 13:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_BARRACK_5]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Nuclear sign
    case 14:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_NUCLEAR]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Metal windows
    case 15:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_LANDING_1]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Sign
    case 16:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_LANDING_2]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
    // Yellow/black stripe
    case 17:
    {
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T);
      glBindTexture(GL_TEXTURE_2D, g_iTextures[TEXTURE_LANDING_3]);
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_white);
      break;
    }
  }
}