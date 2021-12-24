#version 330 core

// Variable de sortie (sera utilisé comme couleur)
out vec4 color;

in vec3 coordonnee_3d;
in vec3 coordonnee_3d_locale;
in vec3 vnormale;
in vec4 vcolor;
in vec2 vtex;

uniform sampler2D texture;
uniform int torche;
uniform int lumiereR;
uniform int lumiereV;
uniform int lumiereB;

// uniform vec3 lampetorcheposition;
//uniform vec3 lampetorchedirection;

void main (void)
{ float faisseau = 0.0;
  vec4 white = vec4(1.0,1.0,1.0,1.0);
  vec4 yellow = vec4(0.9,0.7,0.4,1.0);
  vec3 Luz = vec3(0,5,0);
  vec3 nLuz = normalize(vnormale);
  vec3 dLuz = normalize(Luz-coordonnee_3d);
  vec3 rLuz = reflect(dLuz,nLuz);
  vec3 oLuz = normalize(-coordonnee_3d);
  //calcul d'illumination
  float diffuse  = 0.5*clamp(dot(nLuz,dLuz),0.0,1.0);
  float specular = 0.1*pow(clamp(dot(rLuz,oLuz),0.0,1.0),512.0);
  float ambiant  = 0.7;
  

  vec4 color_texture = texture2D(texture, vtex);
  vec4 color_final   = vcolor*color_texture;
  vec4 RED = vec4(0.0,0.0,0.0,1.0);
  vec4 BLUE = vec4(0.0,0.0,0.0,1.0);
  vec4 GREEN = vec4(0.0,0.0,0.0,1.0);

  if (torche == 1){
    vec3 lampetorcheposition = vec3(0,0,0);
    vec3 lampetorchedirection = vec3(0,0,1);
    //vecteurs pour le calcul d'illumination
    vec3 n = normalize(vnormale);
    vec3 d = normalize(lampetorcheposition-coordonnee_3d_locale);
    vec3 r = reflect(d,n);
    vec3 o = normalize(-coordonnee_3d_locale);
    faisseau = 0.5*pow(clamp(dot(lampetorchedirection,d),0.0,1.0),20.0);
    float pourcentage;

    pourcentage = 0.7*faisseau;
    
    //calcul d'illumination
    float diffuse  = pourcentage*clamp(dot(n,d),0.0,1.0);
    float specular = pourcentage*pow(clamp(dot(r,o),0.0,1.0),128.0);
    float ambiant  = pourcentage;

    //recuperation de la texture
    vec4 color_texture = texture2D(texture, vtex);
    vec4 color_final   = vcolor*color_texture;
  }


  if (lumiereR == 1){
    RED = vec4(0.75,0.25,0.25,1.0);
  }
  if (lumiereV == 1){
    BLUE = vec4(0.25,0.75,0.25,1.0);
  }
  if (lumiereB == 1){
    GREEN = vec4(0.25,0.25,0.75,1.0);
  }
  color = (ambiant+diffuse)*color_final*(RED+GREEN+BLUE)+specular*RED+specular*GREEN+specular*BLUE+(faisseau*yellow*(ambiant+diffuse)*color_final);
  // couleur finale
}
