#version 330 core

// Variable de sortie (sera utilisé comme couleur)
out vec4 color;

in vec3 coordonnee_3d;
in vec3 coordonnee_3d_locale;
in vec3 vnormale;
in vec4 vcolor;
in vec2 vtex;

uniform sampler2D texture;

// uniform vec3 lampetorcheposition;
uniform vec3 lampetorchedirection;

void main (void)
{

  vec3 lampetorcheposition = vec3(0,2,0);
  // vec3 lampetorchedirection = vec3(0,0,1);
  //vecteurs pour le calcul d'illumination
  vec3 n = normalize(vnormale);
  vec3 d = normalize(lampetorcheposition-coordonnee_3d_locale);
  vec3 r = reflect(d,n);
  vec3 o = normalize(-coordonnee_3d_locale);
  float faisseau = 0.5*pow(clamp(dot(lampetorchedirection,d),0.0,1.0),30.0);
  float pourcentage = 0.025;

  pourcentage = 0.7*faisseau;
  //float faisseau = 5;
  
  //calcul d'illumination
  float diffuse  = pourcentage*clamp(dot(n,d),0.0,1.0);
  float specular = pourcentage*pow(clamp(dot(r,o),0.0,1.0),128.0);
  float ambiant  = pourcentage;



  vec4 white = vec4(1.0,1.0,1.0,1.0);
  vec4 yellow = vec4(1.0,1.0,0.2,0.0);

  //recuperation de la texture
  vec4 color_texture = texture2D(texture, vtex);
  vec4 color_final   = vcolor*color_texture;

  //couleur finale
  color = (ambiant+diffuse)*color_final+specular*white+faisseau*yellow;
  //color = (ambiant+diffuse)*color_final+specular*white;
}
