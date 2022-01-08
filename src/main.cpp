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

    
//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

int longu = 1000;

float dL=0.1f;
camera cam;

const int nb_obj = 201;
objet3d obj[nb_obj];

const int nb_text = 17;
text text_to_draw[nb_text];

const int nb_menu = 1;
objet3d menu[nb_menu];

float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;

int torche = 1;
int lumiereR = 1;
int lumiereV = 1;
int lumiereB = 1;

vec3 lastLumiere = vec3(lumiereR,lumiereV,lumiereB);

int Disk = 0;
int numMusic = 0;
bool MenuCodeCoffre = false;
bool MenuChess = false;



int objselected = 0;
int objectselec[11] = {2,3,179,182,188,189,193,197,172,173,174};
vec3 dimensionsobjects[25] = {
	vec3 (-2.5f, -4.5f, -0.5f),//2
	vec3 ( 2.5f,  2.0f,  0.5f),
	vec3 (-2.5f, -4.5f, -0.5f),//3
	vec3 ( 2.5f,  2.0f,  0.5f),
	vec3 (-0.5f, -1.5f, -0.5f),//179
	vec3 ( 0.5f, 0.5f,  0.5f),
  vec3 (-1.3f, -1.5f, -0.5f),//182
	vec3 ( 1.3f, 0.5f,  0.5f),
  vec3 (-0.5f, 0.0f, -0.5f),//188
	vec3 ( 0.5f, 0.50f,  0.5f),
  vec3 (-1.3f, -1.5f, -0.5f),//189
	vec3 ( 1.3f, 0.5f,  0.5f),
  vec3 (-1.3f, -1.5f, -0.5f),//193
	vec3 ( 1.3f, 0.5f,  0.5f),
  vec3 (-1.3f, -1.5f, -0.5f),//197
	vec3 ( 1.3f, 0.5f,  0.5f),
  vec3 (-0.12f, -0.25f, -0.05f),//172
	vec3 ( 0.12f, 0.25f,  0.05f),
  vec3 (-0.12f, -0.25f, -0.05f),//173
	vec3 ( 0.24f, 0.25f,  0.05f),
  vec3 (-0.12f, -0.25f, -0.05f),//174
	vec3 ( 0.24f, 0.25f,  0.05f),
  };

bool CodeCoffre = false;
bool Coffre = false;
bool Animation = false;

bool AnimationRouge = false;
bool AnimationBleu = false;
bool AnimationVert = false;

bool CoffreRouge = false;
bool CoffreVert = false;
bool CoffreBleu = false;


int i = 0;
int k = 0;

float largMaison = 30.0;
float hautMaison = 20.0;
double timer;

bool ActionMenu = false;
bool GodMode = false;

static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();
  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  cam.tr.translation = vec3(0.0f, 2.0f, 0.0f);
  // cam.tr.translation = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_center = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_euler = vec3(M_PI/2., 0.0f, 0.0f);
  init_model_hands();
  init_model_ground();
  init_model_ObjetsFixes();
  init_model_wall1();
  init_model_wall2();
  init_model_wall3();
  init_model_wall4();
  init_model_ceiling();
  init_model_switch();
  init_model_lumiere();
  init_menu();

  gui_program_id = glhelper::create_program_from_file("shaders/gui.vert", "shaders/gui.frag"); CHECK_GL_ERROR();

  textes();
}

void textes(){

  text_to_draw[0].value = "Choose Music";
  text_to_draw[0].bottomLeft = vec2(-0.4, 0.7);
  text_to_draw[0].topRight = vec2(0.4, 1.7);
  text_to_draw[0].visible = false;
  init_text(text_to_draw);

  int itxt = 0;

  for(int xtxt = 0; xtxt < 3; ++xtxt){
    for(int ytxt = 0; ytxt < 3; ++ytxt){
    itxt++;
    char c = '0'+itxt;
    text_to_draw[itxt]=text_to_draw[0];
    text_to_draw[itxt].value = "Music ";
    text_to_draw[itxt].value += c;
    text_to_draw[itxt].bottomLeft = vec2(-0.8 + ytxt*0.6, 0.2 - xtxt*0.5);
    text_to_draw[itxt].topRight = vec2(-0.4 +  ytxt*0.6, 0.7 - xtxt*0.5);
    text_to_draw[itxt].visible = false;
    }
  }

  text_to_draw[10]=text_to_draw[0];
  text_to_draw[10].value = "Entrez le code";
  text_to_draw[10].bottomLeft = vec2(-0.4, 0.7);
  text_to_draw[10].topRight = vec2(0.4, 1.7);
  text_to_draw[10].visible = false;

  for (int itxt = 11; itxt < 15; itxt++){
  text_to_draw[itxt]=text_to_draw[0];
  text_to_draw[itxt].value = '0';
  text_to_draw[itxt].bottomLeft = vec2(-0.5 + (itxt-11)*0.2, -0.1);
  text_to_draw[itxt].topRight = vec2(-0.3 + (itxt-11)*0.2, 0.1);
  text_to_draw[itxt].visible = false;
  }
  
  text_to_draw[15]=text_to_draw[0];
  text_to_draw[15].value = "Retour";
  text_to_draw[15].bottomLeft = vec2(-0.2, -0.4);
  text_to_draw[15].topRight = vec2(0.15, 0.0);
  text_to_draw[15].visible = false;

  text_to_draw[16]=text_to_draw[0];
  text_to_draw[16].value = "Retour";
  text_to_draw[16].bottomLeft = vec2(-0.15, -0.9);
  text_to_draw[16].topRight = vec2(0.15, -0.5);
  text_to_draw[16].visible = false;

}



static void display_callback()
{
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();
  

  if(ActionMenu){
    DisplayMenu();
  }
  else{
    StopDisplayMenu();
    Deplacement();
    ConditionsLumiere();
  }


  //Affichage des differents objets
  for(i = 0; i < nb_obj; ++i){
    draw_obj3d(obj + i, cam);
  }

  for(i = 0; i < nb_menu; ++i){
    draw_obj3d(menu + i, cam);
  }

  for(i = 0; i < nb_text; ++i){
    draw_text(text_to_draw + i);
  }

  fonction_Intersection();
  glutSwapBuffers();
}

void ConditionsLumiere(){
  if (lumiereR==1 && lumiereV==0 && lumiereB==0){
    obj[7].texture_id = glhelper::load_texture("data/WoodHor4-2.tga");
  }
  else{
    obj[7].texture_id = glhelper::load_texture("data/WoodHor4.tga");
  }

  if (Disk==2 && numMusic == 6 && lumiereR==0 && lumiereV==0 && lumiereB==1){
    CoffreBleu = true;
  }

  if (lumiereR==0 && lumiereV==1 && lumiereB==0){
    //obj[x].texture_id = glhelper::load_texture("data/chessboard/chessboardG.tga");
  }
  else{
    //obj[x].texture_id = glhelper::load_texture("data/chessboard/chessboard.tga");
  }
}

void DisplayMenu(){
  glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
  menu[0].visible = true;
  obj[0].visible = false;
  obj[181].visible = false;
  lumiereR = 1;
  lumiereV = 1;
  lumiereB = 1;
  menu[0].tr.translation = cam.tr.translation;
  menu[0].tr.rotation_euler.y = -cam.tr.rotation_euler.y;
  cam.tr.rotation_euler.x = 0;
  menu[0].tr.rotation_euler.x = 0;

}

void StopDisplayMenu(){
  // glutSetCursor(GLUT_CURSOR_NONE);
  glutWarpPointer(longu/2,longu/2);
  lumiereR = lastLumiere.x;
  lumiereV = lastLumiere.y;
  lumiereB = lastLumiere.z;
  menu[0].visible = false;
  obj[0].visible = true;
  for(int itxt = 0; itxt < nb_text; ++itxt){
    text_to_draw[itxt].visible = false;
  }
}

void Deplacement(){

  //Deplacement du personnage
  mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x+M_PI, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
  mat4 rotation = rotation_x*rotation_y;

  if(GodMode){
    if (Move_Up){
        cam.tr.translation += rotation*vec3(0,0,dL);
        if (!SpaceBar){cam.tr.translation.y = 2;}
    }
    if(Move_Down){
        cam.tr.translation -= rotation*vec3(0,0,dL);
        if (!SpaceBar){cam.tr.translation.y = 2;}
    }

    if(Move_Left){
        cam.tr.translation -= rotation*vec3(dL,0,0);
        if (!SpaceBar){cam.tr.translation.y = 2;}
    }

    if(Move_Right){
        cam.tr.translation += rotation*vec3(dL,0,0);
        if (!SpaceBar){cam.tr.translation.y = 2;}
    }
  }
  else{
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
  }

  cam.tr.rotation_center = cam.tr.translation;
  
  timer += 0.05;
  if (timer >2*M_PI){
    timer = 0;
  }

  obj[0].tr.translation = cam.tr.translation+vec3(0.0,(float)0.01*cos(timer),0.0);
  obj[0].tr.rotation_euler.y = -cam.tr.rotation_euler.y;
  obj[181].tr.translation = obj[0].tr.translation;
  obj[181].tr.rotation_euler = obj[0].tr.rotation_euler;
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
    case 'e':
      ActionMenu = !ActionMenu;
      MenuChess = !MenuChess;
      text_to_draw[16].visible = true;
      if(lumiereR==0 && lumiereB==0 && lumiereV==1){
        menu[0].texture_id = glhelper::load_texture("data/menuChessG.tga");
      }
      else{
      menu[0].texture_id = glhelper::load_texture("data/menuChess.tga");
      }
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
    if (!ActionMenu){
      if (objselected == 2 || objselected == 3 ){
        if (Disk == 0){
          Disk = 1;
          ActionMenu = !ActionMenu;
          menu[0].texture_id = glhelper::load_texture("data/testmenu.tga");
          for(int itxt = 0; itxt < 10; ++itxt){
            text_to_draw[itxt].visible = true;
          }
        }
        else if (Disk == 1){
          Disk = 0;
        }
      }
      if (objselected == 179){
        if (Disk == 1){
          std::cout << "Vous deposez le disque ";
          std::cout << numMusic << std::endl;
          Disk = 2;
        }
        else if (Disk == 2){
          Disk = 1;
        }
      }
      if (objselected == 182){
        ActionMenu = !ActionMenu;
        menu[0].texture_id = glhelper::load_texture("data/menuCoffre.tga");
        for (int itxt = 11; itxt < 16; itxt++){
          text_to_draw[itxt].visible = true;
        }
        MenuCodeCoffre = true;
      }
      if (objselected == 189){
        if (!AnimationVert){
          CoffreVert = true;
          AnimationVert = true;
        }
      }
      if (objselected == 193){
        if (!AnimationBleu){
          CoffreBleu = true;
          AnimationBleu = true;
        }
      }
      if (objselected == 197){
        if (!AnimationRouge){
          CoffreRouge = true;
          AnimationRouge = true;
        }
      }
      if (objselected == 188){
        ActionMenu = !ActionMenu;
        MenuChess = true;
        text_to_draw[16].visible = true;
        if(lumiereR==0 && lumiereB==0 && lumiereV==1){
          menu[0].texture_id = glhelper::load_texture("data/menuChessG.tga");
        }
        else{
          menu[0].texture_id = glhelper::load_texture("data/menuChess.tga");
        }
      }

      if (objselected == 172){
        lumiereR = 1-lumiereR;
        lastLumiere.x = lumiereR;
      }
      if (objselected == 173){
        lumiereV = 1-lumiereV;
        lastLumiere.y = lumiereV;
      }
      if (objselected == 174){
        lumiereB = 1-lumiereB;
        lastLumiere.z = lumiereB;
      }
    }
    else{
      if (Disk==1){
        ChoixMusique(x,y);
      }
      if (MenuCodeCoffre == true){
        OuvertureCoffreInterrupteur(x,y);
      }

      if(MenuChess){
        ChessBoard(x,y);
      }
    }
  }
}

void ChessBoard(int x, int y){
  std::cout << x << std::endl;
  std::cout << y << std::endl;
  if (x>400 && x<600 && y>900 && y<950){
    MenuChess = false;
    ActionMenu = !ActionMenu;
  }
}

void OuvertureCoffreInterrupteur(int x, int y){
  if (x>400 && x<600 && y>650 && y<700){
    MenuCodeCoffre = false;
    ActionMenu = !ActionMenu;
  }
  else if (y>475 && y<550){
    if (x>300 && x<350)
    {
      Ajouter1(11);
    }
    else if (x>400 && x<450)
    {
      Ajouter1(12);
    }
    else if (x>500  && x<550)
    {
      Ajouter1(13);
    }
    else if (x>600 && x<650)
    {
      Ajouter1(14);
    }
  }

  if (text_to_draw[11].value == "7" && text_to_draw[12].value == "5" && text_to_draw[13].value == "9" && text_to_draw[14].value == "3"){
    std::cout << "code juste" << std::endl;
    MenuCodeCoffre = false;
    ActionMenu = !ActionMenu;
    Coffre = true;
    Animation = true;
  }
}

void Ajouter1(int indice){
  int value = std::stoi(text_to_draw[indice].value) + 1;
  if (value==10){
    value=0;
  }
  char c = '0'+value;
  text_to_draw[indice].value = c;
}

void ChoixMusique(int x, int y){
  if (x>100 && x<300){
      if (y>200 && y<400){
      numMusic = 1;
      ActionMenu = !ActionMenu;
      }
      else if (y>450 && y<650){
      numMusic = 4;
      ActionMenu = !ActionMenu;
      }
      else if (y>700 && y<900){
      numMusic = 7;
      ActionMenu = !ActionMenu;
      }
    }
    else if (x>400 && x<600){
      if (y>200 && y<400){
      numMusic = 2;
      ActionMenu = !ActionMenu;
      }
      else if (y>450 && y<650){
      numMusic = 5;
      ActionMenu = !ActionMenu;
      }
      else if (y>700 && y<900){
      numMusic = 8;
      ActionMenu = !ActionMenu;
      }
    }
    else if (x>700 && x<900){
      if (y>200 && y<400){
      numMusic = 3;
      ActionMenu = !ActionMenu;
      }
      else if (y>450 && y<650){
      numMusic = 6;
      ActionMenu = !ActionMenu;
      }
      else if (y>700 && y<900){
      numMusic = 9;
      ActionMenu = !ActionMenu;
      }
    }
}

static void timer_callback(int)
{ 
  glutTimerFunc(25, timer_callback, 0);

  if(!ActionMenu){
    
    if (Disk == 1){
      obj[181].visible = true;
      obj[96].visible = false;
    }
    if (Disk == 2){
      obj[181].visible = false;
      obj[96].visible = true;
      obj[96].tr.translation = vec3(-10.0,1.625,0.0);
      obj[96].tr.rotation_euler = vec3(0.0,0.0,M_PI/2);
    }
    if (Disk == 0){
      obj[181].visible = false;
      obj[96].visible = true;
      obj[96].tr.translation = vec3(-3.4,4.225, -14.25);
      obj[96].tr.rotation_euler = vec3(0.0,0.0,0.0);
    }

    if (Coffre){
      obj[184].tr.rotation_euler += vec3(0.1,0.0,0.0);
      obj[185].tr.rotation_euler += vec3(0.1,0.0,0.0);
      if (obj[184].tr.rotation_euler.x> 0){
        Coffre = false;
      }
    }
    if (CoffreVert){

      obj[191].tr.rotation_euler += vec3(0.1,0.0,0.0);
      obj[192].tr.rotation_euler += vec3(0.1,0.0,0.0);
      if (obj[191].tr.rotation_euler.x> 0){
        CoffreVert = false;
      }
    }
    if (CoffreBleu){

      obj[195].tr.rotation_euler += vec3(0.1,0.0,0.0);
      obj[196].tr.rotation_euler += vec3(0.1,0.0,0.0);
      if (obj[195].tr.rotation_euler.x> 0){
        CoffreBleu = false;
      }
    }
    if (CoffreRouge){
      obj[199].tr.rotation_euler += vec3(0.1,0.0,0.0);
      obj[200].tr.rotation_euler += vec3(0.1,0.0,0.0);
      if (obj[199].tr.rotation_euler.x> 0){
        CoffreRouge = false;
      }
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
  }

  glutPostRedisplay();
}

static void mouse_move(int xmous,int ymous){
  vec2 posACT = vec2(xmous,ymous);
  if(!ActionMenu){
    int delta;
    if (posACT.x > longu/2){
      delta = posACT.x-longu/2;
      cam.tr.rotation_euler.y += (M_PI/longu/2)*delta;
      glutWarpPointer(longu/2,longu/2);
      // Cursor_Right = true;
    }
    if (posACT.x < longu/2){
      delta = longu/2-posACT.x;
      cam.tr.rotation_euler.y -= (M_PI/longu/2)*delta;
      glutWarpPointer(longu/2,longu/2);
    }
    if (posACT.y > longu/2){
      delta = posACT.y-longu/2;
      cam.tr.rotation_euler.x += (M_PI/longu/2)*delta;
      if (cam.tr.rotation_euler.x > 0.5){
        cam.tr.rotation_euler.x = 0.5;
      }
      glutWarpPointer(longu/2,longu/2);
    }
    if (posACT.y < longu/2){
      delta = longu/2-posACT.y;
      cam.tr.rotation_euler.x -= (M_PI/longu/2)*delta;
      if (cam.tr.rotation_euler.x < -0.9){
        cam.tr.rotation_euler.x = -0.9;
      }
      glutWarpPointer(longu/2,longu/2);
    }
  }
}


int main(int argc, char** argv)
{
  //SoundEngine->play2D("data/Sabaton.mp3", true);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(longu, longu);
  
  glutCreateWindow("OpenGL");
  // glutSetCursor(GLUT_CURSOR_NONE); 
  glutWarpPointer(longu/2,longu/2);

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

void draw_text(const text * const t)
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
}

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

void init_text(text *t){
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

  t->texture_id = glhelper::load_texture("data/newFontR.tga");

  t->visible = true;
  t->prog = gui_program_id;
}

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

void init_model_hands()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/Arms.obj");

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

  m = load_obj_file("data/Gramophone/Disk_porte.obj");

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[181].vao = upload_mesh_to_gpu(m);

  obj[181].nb_triangle = m.connectivity.size();
  obj[181].texture_id = glhelper::load_texture("data/black.tga");
  obj[181].visible = false;
  obj[181].prog = shader_program_id;


  obj[181].tr.translation = obj[0].tr.translation-vec3(0.0,0.0,0.5);
  obj[181].tr.rotation_euler = -1*obj[0].tr.rotation_euler;
}

void init_model_lumiere(){
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
  obj[171].texture_id = glhelper::load_texture("data/marbre.tga");
  obj[171].visible = true;
  obj[171].prog = shader_program_id;
  obj[171].tr.translation = vec3(largMaison/2-0.1,4.0,0.0);

// lumière gauche
  obj[175].vao = obj[171].vao;

  obj[175].nb_triangle = obj[171].nb_triangle;
  obj[175].texture_id = glhelper::load_texture("data/marbre.tga");
  obj[175].visible = true;
  
  obj[175].tr.translation = vec3(-largMaison/2-0.1, 4.0, 0.0 );
  obj[175].tr.rotation_euler =  vec3(0.0,M_PI,0.0);
  obj[175].prog = shader_program_id;

// lumière gauche
  obj[176].vao = obj[171].vao;

  obj[176].nb_triangle = obj[171].nb_triangle;
  obj[176].texture_id = glhelper::load_texture("data/marbre.tga");
  obj[176].visible = true;
  
  obj[176].tr.translation = vec3(0.0, 7.0, -largMaison/2-0.1 );
  obj[176].tr.rotation_euler =  vec3(0.0,M_PI/2,0.0);
  obj[176].prog = shader_program_id;

// lumière gauche
  obj[177].vao = obj[171].vao;

  obj[177].nb_triangle = obj[171].nb_triangle;
  obj[177].texture_id = glhelper::load_texture("data/marbre.tga");
  obj[177].visible = true;
  
  obj[177].tr.translation = vec3(0.0, 7.0, largMaison/2-0.1 );
  obj[177].tr.rotation_euler =  vec3(0.0,-M_PI/2,0.0);
  obj[177].prog = shader_program_id;

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

void init_model_ObjetsFixes()
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
  obj[2].tr.translation = vec3(-2.5, -0.5, -14.25);

  obj[3].vao = obj[2].vao;
  obj[3].nb_triangle = obj[2].nb_triangle;
  obj[3].texture_id = obj[2].texture_id;
  obj[3].visible = obj[2].visible;
  obj[3].prog = obj[2].prog;
  obj[3].tr.translation = vec3(2.5, -0.5, -14.25);


  m = load_obj_file("data/Gramophone/table.obj");
  // Affecte une transformation sur les sommets du maillage
  s = 0.75f;
  transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[178].vao = upload_mesh_to_gpu(m);
  obj[178].nb_triangle = m.connectivity.size();
  obj[178].texture_id = glhelper::load_texture("data/noirmarbre.tga");
  obj[178].visible = true;
  obj[178].prog = shader_program_id;
  obj[178].tr.translation = vec3(-largMaison/2+5, -0.5, 0.0);


  m = load_obj_file("data/Gramophone/table.obj");
  // Affecte une transformation sur les sommets du maillage
  s = 1.0f;
  transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[187].vao=upload_mesh_to_gpu(m);
  obj[187].nb_triangle = m.connectivity.size();
  obj[187].texture_id = glhelper::load_texture("data/noirmarbre.tga");
  obj[187].visible = true;
  obj[187].prog = shader_program_id;
  obj[187].tr.translation = vec3(largMaison/2-5, -0.5, 0.0);


  m = load_obj_file("data/chessboard/Final_chessboard.obj");
  // Affecte une transformation sur les sommets du maillage
  s = 5.0f;
  transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[188].vao = upload_mesh_to_gpu(m);
  obj[188].nb_triangle = m.connectivity.size();
  obj[188].texture_id = glhelper::load_texture("data/acier.tga");
  obj[188].visible = true;
  obj[188].prog = shader_program_id;
  obj[188].tr.translation = vec3(largMaison/2-5, 1.15, 0.0);


  m = load_obj_file("data/Gramophone/gramophone_lecteur.obj");
  // Affecte une transformation sur les sommets du maillage
  s = 0.40f;
  transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[179].vao = upload_mesh_to_gpu(m);
  obj[179].nb_triangle = m.connectivity.size();
  obj[179].texture_id = glhelper::load_texture("data/acier.tga");
  obj[179].visible = true;
  obj[179].prog = shader_program_id;
  obj[179].tr.translation = vec3(-largMaison/2+5, 0.9, 0.0);


  m = load_obj_file("data/Gramophone/gramophone_son.obj");
  // Affecte une transformation sur les sommets du maillage
  apply_deformation(&m,transform);

  update_normals(&m);
  //invert_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[180].vao = upload_mesh_to_gpu(m);
  obj[180].nb_triangle = m.connectivity.size();
  obj[180].texture_id = glhelper::load_texture("data/dore.tga");
  obj[180].visible = true;
  obj[180].prog = shader_program_id;
  obj[180].tr.translation = vec3(-largMaison/2+5, 0.9, 0.0);


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

  obj[4].texture_id = glhelper::load_texture("data/WoodHor1.tga");

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

  obj[5].texture_id = glhelper::load_texture("data/WoodHor3.tga");

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

  obj[7].texture_id = glhelper::load_texture("data/WoodHor4.tga");

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

vec3 OBBposition_worldspace(ModelMatrix.x, ModelMatrix.y, ModelMatrix.z);

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

vec3 zaxis(0,0, 1);
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
  vec3 aabb_min;
  vec3 aabb_max;
  if (Animation){
    objectselec[3] = 174;
    dimensionsobjects[6] = vec3 (-0.12f, -0.25f, -0.05f);
	  dimensionsobjects[7] =vec3 ( 0.24f, 0.25f,  0.05f);
    // std::cout << dimensionsobjects[10] << std::endl;
    }
  if (AnimationVert){
    objectselec[5] = 174;
    dimensionsobjects[10] = vec3 (-0.12f, -0.25f, -0.05f);
	  dimensionsobjects[11] = vec3 ( 0.24f, 0.25f,  0.05f);
    // std::cout << dimensionsobjects[10] << std::endl;
    }
  if (AnimationBleu){
    objectselec[6] = 174;
    dimensionsobjects[12] = vec3 (-0.12f, -0.25f, -0.05f);
	  dimensionsobjects[13] = vec3 ( 0.24f, 0.25f,  0.05f);
    // std::cout << dimensionsobjects[10] << std::endl;
    }
  if (AnimationRouge){
    objectselec[7] = 174;
    dimensionsobjects[12] = vec3 (-0.12f, -0.25f, -0.05f);
	  dimensionsobjects[13] = vec3 ( 0.24f, 0.25f,  0.05f);
    // std::cout << dimensionsobjects[10] << std::endl;
    }
  
  for (int cptselect = 0; cptselect<=9; cptselect++){
    aabb_min = dimensionsobjects[2*cptselect];
    aabb_max = dimensionsobjects[2*cptselect+1];
    mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x+M_PI, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation = rotation_x*rotation_y;
    vec3 posObj ;
    posObj.x = obj[objectselec[cptselect]].tr.translation.x;
    posObj.y = 2;
    posObj.z = obj[objectselec[cptselect]].tr.translation.z;
    vec3 ModelMatrix = posObj;
    vec3 ray_origin = cam.tr.translation;
    vec3 ray_direction = rotation*vec3(0,0,1);
    if ( TestRayOBBIntersection(
      ray_origin,
      ray_direction, 
      aabb_min, 
      aabb_max,
      ModelMatrix)
    ){
      if ((171 < objectselec[cptselect]) & (objectselec[cptselect] <175)){
        objselected = objectselec[cptselect];
        return;
      }
      else {
        obj[objectselec[cptselect]].texture_id = glhelper::load_texture("data/grisselected.tga");
        objselected = objectselec[cptselect];
        return;

      }
    }
    else{
      if (!((171 < objectselec[cptselect]) & (objectselec[cptselect] <175))){
        obj[objectselec[cptselect]].texture_id = glhelper::load_texture("data/gris.tga");
        objselected = 0;
      }
    }
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
  obj[172].tr.rotation_center = vec3(-2.0f,2.0f,-2.0f);

  update_normals(&m);
  fill_color(&m,vec3(0.5f,0.5f,0.5f));

  obj[172].vao = upload_mesh_to_gpu(m);
  obj[172].nb_triangle = m.connectivity.size();
  obj[172].texture_id = glhelper::load_texture("data/SwitchRed.tga");
  obj[172].visible = true;
  obj[172].prog = shader_program_id;
  obj[172].tr.translation = vec3(2*largMaison/5-0.7, 2.0,largMaison/2-0.1);

  obj[173].vao = obj[172].vao;
  obj[173].nb_triangle = obj[172].nb_triangle;
  obj[173].texture_id = glhelper::load_texture("data/SwitchGreen.tga");
  //obj[173].tr.rotation_euler = vec3(0.0,M_PI/2,0.0);
  obj[173].visible = obj[172].visible;
  obj[173].prog = obj[172].prog;
  obj[173].tr.translation = vec3(2*largMaison/5+0.0, 2.0, largMaison/2-0.1);
  
  obj[174].vao = obj[172].vao;
  obj[174].nb_triangle = obj[172].nb_triangle;
  obj[174].texture_id = glhelper::load_texture("data/SwitchBlue.tga");
  obj[174].visible = obj[172].visible;
  obj[174].prog = obj[172].prog;
  obj[174].tr.translation = vec3(2*largMaison/5+0.7, 2.0, largMaison/2-0.1);
  
  m = load_obj_file("data/coffre/fondCoffreAcier.obj");

  // Affecte une transformation sur les sommets du maillage
  s = 0.75f;
  transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  // coffre 
  apply_deformation(&m,transform);
  fill_color(&m,vec3(0.5f,0.5f,0.5f));
  obj[182].vao = upload_mesh_to_gpu(m);
  obj[182].nb_triangle = m.connectivity.size();
  obj[182].texture_id = glhelper::load_texture("data/dore.tga");
  obj[182].visible = true;
  obj[182].prog = shader_program_id;
  obj[182].tr.translation = vec3(2*largMaison/5, 1.0,largMaison/2-0.65);
  obj[182].tr.rotation_euler = vec3(0.0,M_PI,0.0);

  m = load_obj_file("data/coffre/fondCoffrebois.obj");
  apply_deformation(&m,transform);
  fill_color(&m,vec3(0.5f,0.5f,0.5f));
  obj[183].vao = upload_mesh_to_gpu(m);
  obj[183].nb_triangle = m.connectivity.size();
  obj[183].texture_id = glhelper::load_texture("data/wood.png");
  obj[183].visible = true;
  obj[183].prog = shader_program_id;
  obj[183].tr.translation = obj[182].tr.translation;
  obj[183].tr.rotation_euler = obj[182].tr.rotation_euler;

  m = load_obj_file("data/coffre/CouvercleCoffreAcier.obj");
  apply_deformation(&m,transform);
  fill_color(&m,vec3(0.5f,0.5f,0.5f));
  obj[184].vao = upload_mesh_to_gpu(m);
  obj[184].nb_triangle = m.connectivity.size();
  obj[184].texture_id = glhelper::load_texture("data/dore.tga");
  obj[184].visible = true;
  obj[184].prog = shader_program_id;
  obj[184].tr.translation = obj[182].tr.translation+vec3(0.0,1.72,-0.18);
  obj[184].tr.rotation_euler = obj[182].tr.rotation_euler+vec3(-1.6,0,0);

  m = load_obj_file("data/coffre/CouvercleCoffrebois.obj");
  apply_deformation(&m,transform);
  fill_color(&m,vec3(0.5f,0.5f,0.5f));
  obj[185].vao = upload_mesh_to_gpu(m);
  obj[185].nb_triangle = m.connectivity.size();
  obj[185].texture_id = glhelper::load_texture("data/wood.png");
  obj[185].visible = true;
  obj[185].prog = shader_program_id;
  obj[185].tr.translation = obj[182].tr.translation+vec3(0.0,1.72,-0.18);
  obj[185].tr.rotation_euler = obj[182].tr.rotation_euler+vec3(-1.6,0,0);

 //******************************** Coffre Vert ***************************************
  obj[189].vao = obj[182].vao;
  obj[189].nb_triangle = m.connectivity.size();
  obj[189].texture_id = glhelper::load_texture("data/dore.tga");
  obj[189].visible = true;
  obj[189].prog = shader_program_id;
  obj[189].tr.translation = vec3(-largMaison/5, 1.0,largMaison/2-0.65);
  obj[189].tr.rotation_euler = vec3(0.0,M_PI,0.0);

  obj[190].vao = obj[183].vao;
  obj[190].nb_triangle = m.connectivity.size();
  obj[190].texture_id = glhelper::load_texture("data/GREEN.tga");
  obj[190].visible = true;
  obj[190].prog = shader_program_id;
  obj[190].tr.translation = obj[189].tr.translation;
  obj[190].tr.rotation_euler = obj[189].tr.rotation_euler;

  obj[191].vao = obj[184].vao; 
  obj[191].nb_triangle = m.connectivity.size();
  obj[191].texture_id = glhelper::load_texture("data/dore.tga");
  obj[191].visible = true;
  obj[191].prog = shader_program_id;
  obj[191].tr.translation = obj[189].tr.translation+vec3(0.0,1.72,-0.18);
  obj[191].tr.rotation_euler = obj[189].tr.rotation_euler+vec3(-1.6,0.0,0.0);

  obj[192].vao = obj[185].vao;
  obj[192].nb_triangle = m.connectivity.size();
  obj[192].texture_id = glhelper::load_texture("data/GREEN.tga");
  obj[192].visible = true;
  obj[192].prog = shader_program_id;
  obj[192].tr.translation = obj[191].tr.translation;
  obj[192].tr.rotation_euler = obj[191].tr.rotation_euler;

//******************************** Coffre bleu ***************************************
  obj[193].vao = obj[182].vao;
  obj[193].nb_triangle = m.connectivity.size();
  obj[193].texture_id = glhelper::load_texture("data/dore.tga");
  obj[193].visible = true;
  obj[193].prog = shader_program_id;
  obj[193].tr.translation = vec3(largMaison/5, 1.0,largMaison/2-0.65);
  obj[193].tr.rotation_euler = vec3(0.0,M_PI,0.0);

  obj[194].vao = obj[183].vao;
  obj[194].nb_triangle = m.connectivity.size();
  obj[194].texture_id = glhelper::load_texture("data/BLUE.tga");
  obj[194].visible = true;
  obj[194].prog = shader_program_id;
  obj[194].tr.translation = obj[193].tr.translation;
  obj[194].tr.rotation_euler = obj[193].tr.rotation_euler;

  obj[195].vao = obj[184].vao; 
  obj[195].nb_triangle = m.connectivity.size();
  obj[195].texture_id = glhelper::load_texture("data/dore.tga");
  obj[195].visible = true;
  obj[195].prog = shader_program_id;
  obj[195].tr.translation = obj[193].tr.translation+vec3(0.0,1.72,-0.18);
  obj[195].tr.rotation_euler = obj[193].tr.rotation_euler+vec3(-1.6,0.0,0.0);

  obj[196].vao = obj[185].vao;
  obj[196].nb_triangle = m.connectivity.size();
  obj[196].texture_id = glhelper::load_texture("data/BLUE.tga");
  obj[196].visible = true;
  obj[196].prog = shader_program_id;
  obj[196].tr.translation = obj[195].tr.translation;
  obj[196].tr.rotation_euler = obj[195].tr.rotation_euler;

//******************************** Coffre Rouge ***************************************
  obj[197].vao = obj[182].vao;
  obj[197].nb_triangle = m.connectivity.size();
  obj[197].texture_id = glhelper::load_texture("data/dore.tga");
  obj[197].visible = true;
  obj[197].prog = shader_program_id;
  obj[197].tr.translation = vec3(-2*largMaison/5, 1.0,largMaison/2-0.65);
  obj[197].tr.rotation_euler = vec3(0.0,M_PI,0.0);

  obj[198].vao = obj[183].vao;
  obj[198].nb_triangle = m.connectivity.size();
  obj[198].texture_id = glhelper::load_texture("data/RED.tga");
  obj[198].visible = true;
  obj[198].prog = shader_program_id;
  obj[198].tr.translation = obj[197].tr.translation;
  obj[198].tr.rotation_euler = obj[197].tr.rotation_euler;

  obj[199].vao = obj[184].vao; 
  obj[199].nb_triangle = m.connectivity.size();
  obj[199].texture_id = glhelper::load_texture("data/dore.tga");
  obj[199].visible = true;
  obj[199].prog = shader_program_id;
  obj[199].tr.translation = obj[197].tr.translation+vec3(0.0,1.72,-0.18);
  obj[199].tr.rotation_euler = obj[197].tr.rotation_euler+vec3(-1.6,0.0,0.0);

  obj[200].vao = obj[185].vao;
  obj[200].nb_triangle = m.connectivity.size();
  obj[200].texture_id = glhelper::load_texture("data/RED.tga");
  obj[200].visible = true;
  obj[200].prog = shader_program_id;
  obj[200].tr.translation = obj[199].tr.translation;
  obj[200].tr.rotation_euler = obj[199].tr.rotation_euler;
}


void init_menu(){
  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(1, 1, -1);
  vec3 p1=vec3(-1,1,-1);
  vec3 p2=vec3(-1,-1,-1);
  vec3 p3=vec3(1,-1,-1);

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

  menu[0].nb_triangle = 2;
  menu[0].vao = upload_mesh_to_gpu(m);

  menu[0].texture_id = glhelper::load_texture("data/testmenu.tga");

  menu[0].visible = false;
  menu[0].prog = shader_program_id;

  
}
