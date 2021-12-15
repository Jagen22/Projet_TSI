/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include "declaration.h"

vec2 PosAnc = vec2(300,300);
//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

int longu = 600;
int Cursor_Upe = 600;

float dL=0.1f;

camera cam;

const int nb_obj = 4;
objet3d obj[nb_obj];

const int nb_text = 2;
text text_to_draw[nb_text];

float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;

int torche = 0;
int lumiereR = 0;
int lumiereV = 0;
int lumiereB = 0;


static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();

  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  cam.tr.translation = vec3(1.0f, 2.0f, 0.0f);
  // cam.tr.translation = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_center = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_euler = vec3(M_PI/2., 0.0f, 0.0f);
  init_model_1();
  init_model_2();
  init_model_3();

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

  if (Move_Up){
    cam.tr.translation += rotation*vec3(0,0,dL);
    if (!SpaceBar){cam.tr.translation.y = 2;}
    //obj[0].tr.translation.x += dL;
  }
  if(Move_Down){
    cam.tr.translation -= rotation*vec3(0,0,dL);
    if (!SpaceBar){cam.tr.translation.y = 2;}
    
    //obj[0].tr.translation -= obj[0].tr.rotation_euler.z*vec3(0,0,dL);
  }
  if(Move_Left){
    cam.tr.translation -= rotation*vec3(dL,0,0);
    if (!SpaceBar){cam.tr.translation.y = 2;}
  }
  if(Move_Right){
    cam.tr.translation += rotation*vec3(dL,0,0);
    if (!SpaceBar){cam.tr.translation.y = 2;}
  }
  cam.tr.rotation_center = cam.tr.translation;
  //glTranslated(cos(cam.tr.rotation_euler.y) , sin(cam.tr.rotation_euler.z) ,0);


  //Affichage des differents objets
  for(int i = 0; i < nb_obj; ++i)
    draw_obj3d(obj + i, cam);
  
  // for(int i = 0; i < nb_text; ++i)
  //   draw_text(text_to_draw + i);

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
    // case 'k':
    //   angle_y_model_1 += d_angle;
    //   break;
    // case 'm':
    //   angle_y_model_1 -= d_angle;
    //   break;
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
    //std::cout << cam.tr.rotation_euler.x  << std::endl;
  }
  // mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x+M_PI, 1.0f, 0.0f, 0.0f);
  // mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
  // mat4 rotation = rotation_x*rotation_y;

 
  //Gestion du saut du personnage
  if (SpaceBar){
    //std::cout << obj[0].tr.translation.y << std::endl;
    while(obj[0].tr.translation.y <= 2 && Jump == false){
      cam.tr.translation.y += 0.1;
      obj[0].tr.translation.y += 0.1;
      if (obj[0].tr.translation.y >= 2) {
        Jump = true;
      }
      break;
    }
    if (Jump) {
      cam.tr.translation.y-=0.1;
      obj[0].tr.translation.y -= 0.1;
      if (obj[0].tr.translation.y <= 0) {
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

static void test(int xmous,int ymous){
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
  // vec2 difPos = posACT-PosAnc;

  // std::cout << cam.tr.rotation_euler << std::endl;
  // std::cout << "*************************" << std::endl;

  // cam.tr.rotation_euler.y = cam.tr.rotation_euler.y + difPos.x*0.001f;
  // cam.tr.rotation_euler.x = cam.tr.rotation_euler.x + difPos.y*0.001f;
  // if difPos.x*0.001f

  // glutWarpPointer(longu/2,Cursor_Upe/2);


}

int main(int argc, char** argv)
{
  
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
  glutPassiveMotionFunc(test);
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

void init_model_1()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/stegosaurus.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.2f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[0].tr.rotation_center = vec3(0.0f,0.0f,0.0f);

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[0].vao = upload_mesh_to_gpu(m);

  obj[0].nb_triangle = m.connectivity.size();
  obj[0].texture_id = glhelper::load_texture("data/stegosaurus.tga");
  obj[0].visible = true;
  obj[0].prog = shader_program_id;

  obj[0].tr.translation = vec3(0.0, 0.0, -5.0);
  
}

void init_model_2()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-25.0f,0.0f,-25.0f);
  vec3 p1=vec3( 25.0f,0.0f,-25.0f);
  vec3 p2=vec3( 25.0f,0.0f, 25.0f);
  vec3 p3=vec3(-25.0f,0.0f, 25.0f);

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

  obj[1].texture_id = glhelper::load_texture("data/grass.tga");

  obj[1].visible = true;
  obj[1].prog = shader_program_id;
}

void init_model_3()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/stegosaurus.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.1f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,matrice_rotation(M_PI/2.0f,1.0f,0.0f,0.0f));
  apply_deformation(&m,matrice_rotation(M_PI,0.0f,1.0f,0.0f));
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[2].vao = upload_mesh_to_gpu(m);

  obj[2].nb_triangle = m.connectivity.size();
  obj[2].texture_id = glhelper::load_texture("data/T_arms_A.png");

  obj[2].visible = true;
  obj[2].prog = shader_program_id;

  obj[2].tr.translation = vec3(1.0, 1.0, -10.0);
  
  obj[3].vao = obj[2].vao;

  obj[3].nb_triangle = obj[2].nb_triangle;
  obj[3].texture_id = obj[2].texture_id;

  obj[3].visible = obj[2].texture_id;
  obj[3].prog = obj[2].texture_id;

  obj[3].tr.translation = vec3(1.0, 1.0, -11.0);
}