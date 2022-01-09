
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif

#include "glhelper.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "mesh.hpp"

//matrice de transformation
struct transformation
{
  vec3 rotation_euler;    // rotation autour de x, y, z
  vec3 rotation_center;
  vec3 translation;

  transformation():rotation_euler(),rotation_center(),translation(){}
};

struct camera
{
  transformation tr;
  mat4 projection;
};

struct objet
{
  GLuint prog;        // identifiant du shader
  GLuint vao;         // identifiant du vao
  GLuint nb_triangle; // nombre de triangle du maillage
  GLuint texture_id;  // identifiant de la texture
  bool visible;       // montre ou cache l'objet
};

struct objet3d : public objet
{
  transformation tr;
};

struct text : public objet
{
  std::string value;           // Value of the text to display
  vec2 bottomLeft;
  vec2 topRight;
};

//Variables globales :
bool Move_Up = false;
bool Move_Down = false;
bool Move_Right = false;
bool Move_Left = false;

bool SpaceBar = false;
bool Jump = false;
bool Action = false;

bool Cursor_Up;
bool Cursor_Right;
bool Cursor_Left;
bool Cursor_Down;

//Prototypes des fonctions :
void init_text(text *t);
void draw_text(const text* const t);
void fonction_Intersection();
void init_model_ObjetsFixes();
void init_model_hands();
void init_model_ground();
void init_model_ceiling();
void init_model_wall1();
void init_model_wall2();
void init_model_wall3();
void init_model_wall4();
void init_model_lumiere();
void init_model_switch();
void textes();
void ConditionsLumiere();
void DisplayMenu();
void StopDisplayMenu();
void Deplacement();
void OuvertureBiblio(int, int);
void ChessBoard(int, int);
void OuvertureCoffreRouge(int, int);
void OuvertureCoffreVert(int, int);
void OuvertureCoffreInterrupteur(int, int);
void ChoixMusique(int, int);
void Ajouter1(int);
void init_menu();


void draw_obj3d(const objet3d* const obj, camera cam);
