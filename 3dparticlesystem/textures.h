#ifndef _textures_h
#define _textures_h

//material class
class material {
public:
	material();

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shinyness;

};


#endif