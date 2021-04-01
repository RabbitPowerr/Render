
#include <iostream>
#include "Render.h"
#include<string >
	

int main(int argc, char** argv) {
	if (argc <= 1) {
		std::cout << "Err" << std::endl;
		return 0;
	}
	
	if (strcmp(argv[1],"-h")==0) {
		std::cout << "-obj <path to obj>\n -txtr <path to texture>\n (optional)-eye <x,y,z point of eye>\n";
		std::cout << "(optional)-light <x,y,z light direction>\n (optional)-out <name outfile>";
		return 0;
	}
	if (argc < 3) {
		std::cout << "Err" << std::endl;
		return 0;
	}
	Vec3<float> eye(2, 2, 3);
	if (argc >= 9) {
		eye.x =atof(argv[6]);
		eye.y =atof(argv[7]);
		eye.z =atof(argv[8]);
	}
	Vec3<float> light(1, 1,1);
	if (argc >= 13) {
		light.x = atof(argv[10]);
		light.y = atof(argv[11]);
		light.z = atof(argv[12]);
	}
	light.normalize();
	Renderer render(argv[2],argv[4],eye,light);
	render.start_render((argc >= 15) ? argv[14] : "out_file");
	return 0;
}

