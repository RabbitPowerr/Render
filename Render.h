
#include <iostream>
#include "Geomety.h"
#include "vector.h"

class Renderer{
private:
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor black = TGAColor(0, 0, 0, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const int height = 800, width = 800, depth = 255;
	const Vec3<float> ligth_dir;
	std::vector<std::vector<float>> zbuffer;
	Model model;
	Vec3<float> eye;
	Vec3<float> center;

public:
	Renderer();
	Renderer(const char* objfilename, const char* texturefile, Vec3<float> _eye = {0,0,4}, Vec3<float> _light = { 0,0,-1 });
	void start_render(const char* outfile);
	~Renderer();
	


};

