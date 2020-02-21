#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	main()
{
	char *scene = "<scene ambiant=\"0.5\" AA=\"0\" reflection_depth=\"1\" refraction_depth=\"5\" filter=\" \" resolution=\"1000\"/>\n<camera position=\"(76.396449, 258.128544, 8.652867)\" lookat=\"(76.396449, 8.128544, 8.652867)\" fov=\"35\" rotation=\"(0, 0, 0)\"/>\n<light center=\"(1000, 1000, 1000)\" intensity=\"0.8\" color=\"#FFFFFF\"/>\n<plane center=\"(-1500, 0, 0)\" normal=\"(-1, 0, 0)\" color=\"#D3D3D3\"/>\n<plane center=\"(1500, 0, 0)\" normal=\"(-1, 0, 0)\" color=\"#D3D3D3\"/>\n<plane center=\"(0, -1500, 0)\" normal=\"(0, -1, 0)\" color=\"#D3D3D3\"/>\n<plane center=\"(0, 1500, 0)\" normal=\"(0, -1, 0)\" color=\"#D3D3D3\"/>\n<plane center=\"(0, 0, -1500)\" normal=\"(0, 0, -1)\" color=\"#D3D3D3\"/>\n<plane center=\"(0, 0, 1500)\" normal=\"(0, 0, -1)\" color=\"#D3D3D3\"/>\n";
	write(1, scene, strlen(scene));
	write(1, "!\n", 2);
	int a = 0;
	write(1, &a, 4);
	a = 720;
	write(1, &a, 4);
}
