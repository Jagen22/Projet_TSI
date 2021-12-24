/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * -------------- ALEX
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include "declaration.h"
#include <bits/stdc++.h>
//#include <irrKlang.h>

// using namespace irrklang;
// ISoundEngine *SoundEngine = createIrrKlangDevice();

    
vec2 PosAnc = vec2(300,300);
//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

int longu = 600;
int Cursor_Upe = 600;

float dL=0.1f;
camera cam;

const int nb_obj = 175;
objet3d obj[nb_obj];

const int nb_text = 2;
text text_to_draw[nb_text];

float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;

int torche = 1;
int lumiereR = 0;
int lumiereV = 0;
int lumiereB = 0;

int i = 0;
int k = 0;

float largMaison = 30.0;
float hautMaison = 20.0;
int compteur = 0;

static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();
  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  cam.tr.translation = vec3(0.0f, 2.0f, 0.0f);
  // cam.tr.translation = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_center = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_euler = vec3(M_PI/2., 0.0f, 0.0f);
  init_model_dino();
  init_model_ground();
  init_model_3();
  init_model_wall1();
  init_model_wall2();
  init_model_wall3();
  init_model_wall4();
  init_model_ceiling();
  init_model_switch();
  init_model_lumiere();

  // gui_program_id = glhelper::create_program_from_file("shaders/gui.vert", "shaders/gui.frag"); CHECK_GL_ERROR();

  // text_to_draw[0].value = "CE";
  // text_to_draw[0].bottomLeft = vec2(-0.2, 0.5);
  // text_to_draw[0].topRight = vec2(0.2, 1);
  // init_text(text_to_draw);

  // text_to_draw[1]=text_to_draw[0];
  // text_to_draw[1].value = "Lyon";
  // text_to_draw[1].bottomLeft.y = 0.0f;
  // text_to_draw[1].topRight.y = 0.5f;
}

static void display_callback()
{
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  //Deplacement du personnage
  mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x+M_PI, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
  mat4 rotation = rotation_x*rotation_y;

  //Si une touche de deplacement est activee, on teste si la future position depasse les limites de la map
  //Dans le cas ou le deplacement est valide, on deplace la camera de dL dans la direction d'orientation de la camera
  //On resalise ensuite un test : dans le cas où on ne saute pas on force la camera en y=2
  if (Move_Up){
    if (abs(cam.tr.translation.x + (rotation*vec3(0,0,dL)).x) < largMaison/2 - 1 && (abs(cam.tr.translation.z + (rotation*vec3(0,0,dL)).z) < largMaison/2 - 1)){
      cam.tr.translation += rotation*vec3(0,0,dL);
      if (!SpaceBar){cam.tr.translation.y = 2;}
    }

  }
  if(Move_Down){
    if (abs(cam.tr.translation.x - (rotation*vec3(0,0,dL)).x) < largMaison/2 - 1 && (abs(cam.tr.translation.z - (rotation*vec3(0,0,dL)).z) < largMaison/2 - 1)){
      cam.tr.translation -= rotation*vec3(0,0,dL);
      if (!SpaceBar){cam.tr.translation.y = 2;}
    }
  }

  if(Move_Left){
    if (abs(cam.tr.translation.x - (rotation*vec3(dL,0,0)).x) < largMaison/2 - 1 && (abs(cam.tr.translation.z - (rotation*vec3(dL,0,0)).z) < largMaison/2 - 1)){
      cam.tr.translation -= rotation*vec3(dL,0,0);
      if (!SpaceBar){cam.tr.translation.y = 2;}
    }
  }

  if(Move_Right){
    if (abs(cam.tr.translation.x + (rotation*vec3(dL,0,0)).x) < largMaison/2 - 1 && (abs(cam.tr.translation.z + (rotation*vec3(dL,0,0)).z) < largMaison/2 - 1)){
      cam.tr.translation += rotation*vec3(dL,0,0);
      if (!SpaceBar){cam.tr.translation.y = 2;}
    }
  }

  cam.tr.rotation_center = cam.tr.translation;
  //glTranslated(cos(cam.tr.rotation_euler.y) , sin(cam.tr.rotation_euler.z) ,0);
  
  obj[0].tr.translation = cam.tr.translation+vec3(0.0,0.0,5.0);
  obj[0].tr.rotation_euler.y = -cam.tr.rotation_euler.y;  


  //Affichage des differents objets
  for(int i = 0; i < nb_obj; ++i){
    draw_obj3d(obj + i, cam);
  }

  // for(int i = 0; i < nb_text; ++i)
  //   draw_text(text_to_draw + i);
  fonction_Intersection();

  glutSwapBuffers();
}

static void keyboard_callback(unsigned char key, int, int)
{
  float d_angle = 0.1f;

  switch (key)
  {
    case 'p':
      glhelper::print_screen();
      break;
    case 27:
      exit(0);
      break;
    case 'e':
      Action = true;
      break;
    case 'z':
      Move_Up = true;
      break;
    case 's':
      Move_Down = true;
      break;
    case 'q':
      Move_Left = true;
      break;
    case 'd':
      Move_Right = true;
      break;
    case 32:
      SpaceBar = true;
      break;
    case 't':
      torche = 1-torche;
      break;
    case 'r':
      lumiereR = 1-lumiereR;
      break;
    case 'v':
      lumiereV = 1-lumiereV;
      break;
    case 'b':
      lumiereB = 1-lumiereB;
      break;
    case 'm':
      text_to_draw[0].visible = !(text_to_draw[0].visible);
      break;
  }
}

static void keyboard_callback_stop(unsigned char key, int, int){
    switch (key)
  {
    case 'z':
      Move_Up = false;
      break;
    case 's':
      Move_Down = false;
      break;
    case 'q':
      Move_Left = false;
      break;
    case 'd':
    Move_Right = false;
      break;
  }
}

static void special_callback(int key, int, int)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      Move_Up = true;
      break;
    case GLUT_KEY_DOWN:
      Move_Down = true;
      break;
    case GLUT_KEY_LEFT:
      Move_Left = true;
      break;
    case GLUT_KEY_RIGHT:
      Move_Right = true;
      break;
  }
}

static void special_callback_stop(int key, int,int)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      Move_Up = false;
      break;
    case GLUT_KEY_DOWN:
      Move_Down = false;
      break;
    case GLUT_KEY_LEFT:
      Move_Left = false;
      break;
    case GLUT_KEY_RIGHT:
    Move_Right = false;
      break;

    }
}

static void mouse_clic(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
      std::cout << "clic"<< std::endl;
      std::cout << x << std::endl;
      std::cout << y << std::endl;
  }
}

static void timer_callback(int)
{ 
  glutTimerFunc(25, timer_callback, 0);

  //Gestion de la camera et du curseur
  float angle = 0.02;
  if (Cursor_Right){
    cam.tr.rotation_euler.y += angle;
    
  }
  if (Cursor_Left){
    cam.tr.rotation_euler.y -= angle;
  }
  if (Cursor_Up && cam.tr.rotation_euler.x < 0.5){
    cam.tr.rotation_euler.x += angle;
  }
  if (Cursor_Down && cam.tr.rotation_euler.x > -1){
    cam.tr.rotation_euler.x -= angle;
  }
  
 
  //Gestion du saut du personnage
  if (SpaceBar){
    //std::cout << obj[0].tr.translation.y << std::endl;
    while(cam.tr.translation.y <= 4 && Jump == false){
      cam.tr.translation.y += 0.1;
      if (cam.tr.translation.y >= 4) {
        Jump = true;
      }
      break;
    }
    if (Jump) {
      cam.tr.translation.y-=0.1;
      if (cam.tr.translation.y <= 2) {
        Jump = false;
        SpaceBar = false;
      }
    }
  }
 
  // mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
  // mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
  // mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
  // std::cout << rotation_x*rotation_y*rotation_z << std::endl;
  //cam.projection = rotation_x*rotation_y*rotation_z;
  

  // float Theta = cam.tr.rotation_euler.z;
  // float phi = cam.tr.rotation_euler.y;
  // float psi = cam.tr.rotation_euler.x;

  // cam.projection = mat4( x11,x12,x13,0,
  //                        x21,x22,x23,0,
  //                        x31,x32,x33,0,
  //                        0,0,0,0);
  // cam.tr.Projection = {{cos(psi)*cos(phi)-sin(phi)*cos(Theta)*sin(psi) ,sin(psi)*cos(phi)+cos(psi)*cos(Theta)*sin(phi) ,sin(Theta)*sin(phi)},
  //                      {-cos(psi)*sin(phi)-sin(phi)*cos(Theta)*cos(psi),-sin(psi)*sin(phi)-cos(phi)*cos(Theta)*cos(psi),sin(Theta)*cos(phi)},
  //                      {sin(phi)*sin(Theta),-cos(psi)*sin(Theta),cos(Theta)}};


  glutPostRedisplay();
}

static void mouse_move(int xmous,int ymous){
  vec2 posACT = vec2(xmous,ymous);
  int limminrectMousse = 15;
  int limmaxrectMousse = 30;
  Cursor_Right = false;
  Cursor_Left = false;
  Cursor_Up = false;
  Cursor_Down = false;
  if (posACT.x > longu/2+limminrectMousse){
    if (posACT.x > Cursor_Upe/2+limmaxrectMousse){
      glutWarpPointer(Cursor_Upe/2+limmaxrectMousse,posACT.y);
    }
    Cursor_Right = true;
  }
  if (posACT.x < longu/2-limminrectMousse){
    if (posACT.x < Cursor_Upe/2-limmaxrectMousse){
      glutWarpPointer(Cursor_Upe/2-limmaxrectMousse,posACT.y);
    }
    Cursor_Left = true;
  }
  if (posACT.y > Cursor_Upe/2+limminrectMousse){
    if (posACT.y > Cursor_Upe/2+limmaxrectMousse){
      glutWarpPointer(posACT.x,Cursor_Upe/2+limmaxrectMousse);
    }
    Cursor_Up = true;
  }
  if (posACT.y < Cursor_Upe/2-limminrectMousse){
    if (posACT.y < Cursor_Upe/2-limmaxrectMousse){
      glutWarpPointer(posACT.x,Cursor_Upe/2-limmaxrectMousse);
    }
    Cursor_Down = true;
  }
}

int main(int argc, char** argv)
{
  //SoundEngine->play2D("data/Sabaton.mp3", true);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(600, 600);
  
  glutCreateWindow("OpenGL");
  // glutSetCursor(GLUT_CURSOR_NONE) ; 
  glutWarpPointer(longu/2,Cursor_Upe/2);

  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutKeyboardUpFunc(keyboard_callback_stop);
  glutSpecialFunc(special_callback);
  glutSpecialUpFunc(special_callback_stop);
  glutPassiveMotionFunc(mouse_move);
  glutMouseFunc(mouse_clic);
  glutTimerFunc(25, timer_callback, 0);

  
  glewExperimental = true;
  glewInit();



  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  init();
  glutMainLoop();

  return 0;
}

/*void draw_text(const text * const t)
{
  if(!t->visible) return;

  glDisable(GL_DEPTH_TEST);
  glUseProgram(t->prog);

  vec2 size = (t->topRight - t->bottomLeft) / float(t->value.size());
  
  GLint loc_size = glGetUniformLocation(gui_program_id, "size"); CHECK_GL_ERROR();
  if (loc_size == -1) std::cerr << "Pas de variable uniforme : size" << std::endl;
  glUniform2f(loc_size,size.x, size.y);     CHECK_GL_ERROR();

  glBindVertexArray(t->vao);                CHECK_GL_ERROR();
  
  for(unsigned i = 0; i < t->value.size(); ++i)
  {
    GLint loc_start = glGetUniformLocation(gui_program_id, "start"); CHECK_GL_ERROR();
    if (loc_start == -1) std::cerr << "Pas de variable uniforme : start" << std::endl;
    glUniform2f(loc_start,t->bottomLeft.x+i*size.x, t->bottomLeft.y);    CHECK_GL_ERROR();

    GLint loc_char = glGetUniformLocation(gui_program_id, "c"); CHECK_GL_ERROR();
    if (loc_char == -1) std::cerr << "Pas de variable uniforme : c" << std::endl;
    glUniform1i(loc_char, (int)t->value[i]);    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, t->texture_id);                            CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);                    CHECK_GL_ERROR();
  }
}*/

void draw_obj3d(const objet3d* const obj, camera cam)
{
  if(!obj->visible) return;

  glEnable(GL_DEPTH_TEST);
  glUseProgram(obj->prog);
  
  {
    GLint loc_projection = glGetUniformLocation(obj->prog, "projection"); CHECK_GL_ERROR();
    if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
    glUniformMatrix4fv(loc_projection,1,false,pointeur(cam.projection));    CHECK_GL_ERROR();

    
    GLint loc_torche = glGetUniformLocation(obj->prog, "torche"); CHECK_GL_ERROR();
    if (loc_torche== -1) std::cerr << "Pas de variable uniforme : torche" << std::endl;
    glUniform1i(loc_torche, torche);    CHECK_GL_ERROR();

    GLint loc_lumiereR = glGetUniformLocation(obj->prog, "lumiereR"); CHECK_GL_ERROR();
    if (loc_lumiereR== -1) std::cerr << "Pas de variable uniforme : lumiereR" << std::endl;
    glUniform1i(loc_lumiereR, lumiereR);    CHECK_GL_ERROR();

    GLint loc_lumiereB = glGetUniformLocation(obj->prog, "lumiereB"); CHECK_GL_ERROR();
    if (loc_lumiereB== -1) std::cerr << "Pas de variable uniforme : lumiereB" << std::endl;
    glUniform1i(loc_lumiereB, lumiereB);    CHECK_GL_ERROR();

    GLint loc_lumiereV = glGetUniformLocation(obj->prog, "lumiereV"); CHECK_GL_ERROR();
    if (loc_lumiereV== -1) std::cerr << "Pas de variable uniforme : lumiereV" << std::endl;
    glUniform1i(loc_lumiereV, lumiereV);    CHECK_GL_ERROR();


    GLint loc_rotation_view = glGetUniformLocation(obj->prog, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 cv = cam.tr.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(obj->prog, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    vec3 tv = cam.tr.translation;
    GLint loc_translation_view = glGetUniformLocation(obj->prog, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }
  {
    GLint loc_rotation_model = glGetUniformLocation(obj->prog, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    mat4 rotation_x = matrice_rotation(obj->tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(obj->tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(obj->tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 c = obj->tr.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(obj->prog, "rotation_center_model");   CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                  CHECK_GL_ERROR();

    vec3 t = obj->tr.translation;
    GLint loc_translation_model = glGetUniformLocation(obj->prog, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }
  glBindVertexArray(obj->vao);                                              CHECK_GL_ERROR();

  glBindTexture(GL_TEXTURE_2D, obj->texture_id);                            CHECK_GL_ERROR();
  glDrawElements(GL_TRIANGLES, 3*obj->nb_triangle, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
}

/*void init_text(text *t){
  vec3 p0=vec3( 0.0f, 0.0f, 0.0f);
  vec3 p1=vec3( 0.0f, 1.0f, 0.0f);
  vec3 p2=vec3( 1.0f, 1.0f, 0.0f);
  vec3 p3=vec3( 1.0f, 0.0f, 0.0f);

  vec3 geometrie[4] = {p0, p1, p2, p3}; 
  triangle_index index[2] = { triangle_index(0, 1, 2), triangle_index(0, 2, 3)};

  glGenVertexArrays(1, &(t->vao));                                              CHECK_GL_ERROR();
  glBindVertexArray(t->vao);                                                  CHECK_GL_ERROR();

  GLuint vbo;
  glGenBuffers(1, &vbo);                                                       CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                          CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  GLuint vboi;
  glGenBuffers(1,&vboi);                                                      CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                                 CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  t->texture_id = glhelper::load_texture("data/fontB.tga");

  t->visible = true;
  t->prog = gui_program_id;
}*/

GLuint upload_mesh_to_gpu(const mesh& m)
{
  GLuint vao, vbo, vboi;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);                                 CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();
  
  return vao;
}

void init_model_dino()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/stegosaurus.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.55f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[0].tr.rotation_euler = vec3(0.0f,0.0f,0.0f);

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[0].vao = upload_mesh_to_gpu(m);

  obj[0].nb_triangle = m.connectivity.size();
  obj[0].texture_id = glhelper::load_texture("data/T_arms_A.tga");
  obj[0].visible = true;
  obj[0].prog = shader_program_id;


  obj[0].tr.translation = cam.tr.translation;
  obj[0].tr.rotation_euler = -1*cam.tr.rotation_euler;

  
}

void  init_model_lumiere(){
  mesh m = load_obj_file("data/lumiere.obj");
  float s = 0.05f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[171].vao = upload_mesh_to_gpu(m);

  obj[171].nb_triangle = m.connectivity.size();
  obj[171].texture_id = glhelper::load_texture("data/AM134_14_wood_1.tga");
  obj[171].visible = true;
  obj[171].prog = shader_program_id;
  obj[171].tr.translation = vec3(0,5,-1);

}
void init_model_ground()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-largMaison/2,0.0f,-largMaison/2); //D
  vec3 p1=vec3( largMaison/2,0.0f,-largMaison/2); //C
  vec3 p2=vec3( largMaison/2,0.0f, largMaison/2); //B
  vec3 p3=vec3(-largMaison/2,0.0f, largMaison/2); //A

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,1.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[1].nb_triangle = 2;
  obj[1].vao = upload_mesh_to_gpu(m);

  obj[1].texture_id = glhelper::load_texture("data/ground1.tga");

  obj[1].visible = true;
  obj[1].prog = shader_program_id;
}

void init_model_3()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/Gramophone/etagere.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.25f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[2].vao = upload_mesh_to_gpu(m);

  obj[2].nb_triangle = m.connectivity.size();
  obj[2].texture_id = glhelper::load_texture("data/gris.tga");

  obj[2].visible = true;
  obj[2].prog = shader_program_id;

  obj[2].tr.translation = vec3(-5, -0.5, -14.25);
  
  obj[3].vao = obj[2].vao;

  obj[3].nb_triangle = obj[2].nb_triangle;
  obj[3].texture_id = obj[2].texture_id;

  obj[3].visible = obj[2].visible;
  obj[3].prog = obj[2].prog;

  obj[3].tr.translation = vec3(2.5, -0.5, -14.25);


  m = load_obj_file("data/Gramophone/Disk.obj");
  fill_color(&m,vec3(1.0f,1.0f,1.0f));
  obj[9].vao = upload_mesh_to_gpu(m);
  obj[9].nb_triangle = m.connectivity.size();
  obj[9].visible = true;
  obj[9].prog = shader_program_id;
  obj[9].tr.translation = vec3(-4.9, 1.825, -14.25);
  for (k = 0;k<4;k++){
    for (i = 0; i<41;i++){
      obj[(40*k)+i+10].vao = obj[9].vao;
      obj[(40*k)+i+10].nb_triangle = obj[9].nb_triangle ;
      obj[(40*k)+i+10].visible = true;
      obj[(40*k)+i+10].prog = shader_program_id;
      obj[(40*k)+i+10].tr.translation = obj[9].tr.translation+vec3(0.25*i,(k)*1.2,0);
    }

  }

}

void init_model_wall1()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3( largMaison/2,hautMaison, -largMaison/2); //G
  vec3 p1=vec3( -largMaison/2,hautMaison,-largMaison/2); //H
  vec3 p2=vec3(-largMaison/2, 0.0,-largMaison/2); //D
  vec3 p3=vec3(largMaison/2, 0.0, -largMaison/2); //C

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,0.0f,1.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[4].nb_triangle = 2;
  obj[4].vao = upload_mesh_to_gpu(m);

  obj[4].texture_id = glhelper::load_texture("data/WoodHor2.tga");

  obj[4].visible = true;
  obj[4].prog = shader_program_id;
}

void init_model_wall2()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-largMaison/2,hautMaison, largMaison/2); //E
  vec3 p1=vec3( largMaison/2,hautMaison, largMaison/2); //F
  vec3 p2=vec3( largMaison/2,0.0f, largMaison/2); //B
  vec3 p3=vec3(-largMaison/2,0.0f, largMaison/2); //A

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,0.0f,1.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[5].nb_triangle = 2;
  obj[5].vao = upload_mesh_to_gpu(m);

  obj[5].texture_id = glhelper::load_texture("data/WoodHor2.tga");

  obj[5].visible = true;
  obj[5].prog = shader_program_id;
}

void init_model_wall3()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3( largMaison/2,hautMaison,-largMaison/2); //G
  vec3 p1=vec3( largMaison/2,hautMaison, largMaison/2); //F
  vec3 p2=vec3( largMaison/2,0.0f, largMaison/2); //B
  vec3 p3=vec3( largMaison/2,0.0f,-largMaison/2); //C

  //normales pour chaque sommet
  vec3 n0=vec3(1.0f,0.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[6].nb_triangle = 2;
  obj[6].vao = upload_mesh_to_gpu(m);

  obj[6].texture_id = glhelper::load_texture("data/WoodHor2.tga");

  obj[6].visible = true;
  obj[6].prog = shader_program_id;
}

void init_model_wall4()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-largMaison/2,hautMaison, largMaison/2); //E
  vec3 p1=vec3(-largMaison/2,hautMaison,-largMaison/2); //H
  vec3 p2=vec3(-largMaison/2,0.0f,-largMaison/2); //D
  vec3 p3=vec3(-largMaison/2,0.0f, largMaison/2); //A

  //normales pour chaque sommet
  vec3 n0=vec3(1.0f,0.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[7].nb_triangle = 2;
  obj[7].vao = upload_mesh_to_gpu(m);

  obj[7].texture_id = glhelper::load_texture("data/WoodHor2.tga");

  obj[7].visible = true;
  obj[7].prog = shader_program_id;
}

void init_model_ceiling()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-largMaison/2,hautMaison,-largMaison/2); //H
  vec3 p1=vec3( largMaison/2,hautMaison,-largMaison/2); //G
  vec3 p2=vec3( largMaison/2,hautMaison, largMaison/2); //F
  vec3 p3=vec3(-largMaison/2,hautMaison, largMaison/2); //E
  

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,1.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[8].nb_triangle = 2;
  obj[8].vao = upload_mesh_to_gpu(m);

  obj[8].texture_id = glhelper::load_texture("data/ceiling.tga");

  obj[8].visible = true;
  obj[8].prog = shader_program_id;
}
bool TestRayOBBIntersection(
	vec3 ray_origin ,        // Origine du rayon, dans le repère du monde
	vec3 ray_direction,     // Direction du rayon (PAS la cible !), dans le repère du monde. Doit être normalisé
	vec3 aabb_min,          // Coordonnées minimales X,Y,Z du modèle lorsqu'il n'est pas transformé du tout
	vec3 aabb_max,          // Coordonnées maximales X,Y,Z. Souvent aabb_min*-1 si votre modèle est centré, mais ce n'est pas toujours le cas
	vec3 ModelMatrix       // Transformation appliquée au modèle (et qui sera donc appliquée à la boîte englobante) 
){
float tMin = 0.0f;
float tMax = 100000.0f;

vec3 OBBposition_worldspace(ModelMatrix.x, ModelMatrix.y, ModelMatrix.y);

vec3 delta = OBBposition_worldspace - ray_origin;
vec3 xaxis(1,0, 0);
float e = dot(xaxis, delta);
float f = dot(ray_direction, xaxis);

// Beware, don't do the division if f is near 0 ! See full source code for details.
float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane

if (t1>t2){ // if wrong order
	float w=t1;t1=t2;t2=w; // swap t1 and t2
}
// tMax est l'intersection « lointaine » la plus proche (parmi les paires de plans X,Y et Z)
if ( t2 < tMax ) tMax = t2;
// tMin est l'intersection « proche » la plus lointaine (parmi les paires de plans X,Y et Z)
if ( t1 > tMin ) tMin = t1;
if (tMax < tMin )return false;
vec3 yaxis(0,1, 0);
e = dot(yaxis, delta);
f = dot(ray_direction, yaxis);

// Beware, don't do the division if f is near 0 ! See full source code for details.
t1 = (e+aabb_min.y)/f; // Intersection with the "left" plane
t2 = (e+aabb_max.y)/f; // Intersection with the "right" plane

if (t1>t2){ // if wrong order
	float w=t1;t1=t2;t2=w; // swap t1 and t2
}
// tMax est l'intersection « lointaine » la plus proche (parmi les paires de plans X,Y et Z)
if ( t2 < tMax ) tMax = t2;
// tMin est l'intersection « proche » la plus lointaine (parmi les paires de plans X,Y et Z)
if ( t1 > tMin ) tMin = t1;
if (tMax < tMin )return false;

vec3 zaxis(0,0, -1);
e = dot(zaxis, delta);
f = dot(ray_direction, zaxis);

// Beware, don't do the division if f is near 0 ! See full source code for details.
t1 = (e+aabb_min.z)/f; // Intersection with the "left" plane
t2 = (e+aabb_max.z)/f; // Intersection with the "right" plane

if (t1>t2){ // if wrong order
	float w=t1;t1=t2;t2=w; // swap t1 and t2
}
// tMax est l'intersection « lointaine » la plus proche (parmi les paires de plans X,Y et Z)
if ( t2 < tMax ) tMax = t2;
// tMin est l'intersection « proche » la plus lointaine (parmi les paires de plans X,Y et Z)
if ( t1 > tMin ) tMin = t1;
if (tMax < tMin )return false;
return true;
}

void fonction_Intersection(){
	vec3 aabb_min(-2.5f, -1.0f, -0.5f);
	vec3 aabb_max( 2.5f,  10.0f,  0.5f);

	// The ModelMatrix transforms :
	// - the mesh to its desired position and orientation
	// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
  mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
  mat4 rotation = rotation_z*rotation_y*rotation_x;
	vec3 ModelMatrix = rotation*obj[3].tr.translation-cam.tr.translation;
  vec3 ray_origin = cam.tr.translation ;

  
  
	vec3 ray_direction = rotation*vec3(0,0,1);
	if ( TestRayOBBIntersection(
		ray_origin, 
		ray_direction, 
		aabb_min, 
		aabb_max,
		ModelMatrix)
	){
		obj[3].texture_id = glhelper::load_texture("data/grisselected.tga");
	}
  else{
    obj[3].texture_id = glhelper::load_texture("data/gris.tga");
  	std::cout<< "test" << std::endl;
  }
}

void init_model_switch()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/Switch2.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.1f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[172].tr.rotation_center = vec3(0.0f,0.0f,0.0f);

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[172].vao = upload_mesh_to_gpu(m);
  obj[172].nb_triangle = m.connectivity.size();
  obj[172].texture_id = glhelper::load_texture("data/SwitchRed.jpg");
  obj[172].visible = true;
  obj[172].prog = shader_program_id;
  obj[172].tr.translation = vec3(-2.0, 2.0, largMaison/2-0.1);
  
  obj[173].vao = obj[172].vao;
  obj[173].nb_triangle = obj[172].nb_triangle;
  obj[173].texture_id = glhelper::load_texture("data/SwitchGreen.jpg");
  obj[173].visible = obj[172].visible;
  obj[173].prog = obj[172].prog;
  obj[173].tr.translation = vec3(0.0, 2.0, largMaison/2-0.1);
  
  obj[174].vao = obj[172].vao;
  obj[174].nb_triangle = obj[172].nb_triangle;
  obj[174].texture_id = glhelper::load_texture("data/SwitchBlue.jpg");
  obj[174].visible = obj[172].visible;
  obj[174].prog = obj[172].prog;
  obj[174].tr.translation = vec3(2.0, 2.0, largMaison/2-0.1);
}
