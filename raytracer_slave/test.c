#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	main()
{
	char *scene = "<camera position=\"(0, 0, 100)\" lookat=\"(0, 0, 0)\" fov=\"90\"/>\n<light center=\"(0, 100, 0)\" intensity=\"0.5\" color=\"#FFFFFF\"/>\n<triangle center=\"(0, 0, 0)\" side1=\"(10,  0,  0)\" side2=\"(0, 10, 0)\" normal=\"(0, 1, 0)\" color=\"(50, 150, 50)\" translation=\"(0, 0, 0)\"/>\n";
	write(1, scene, strlen(scene));
	write(1, "!\n", 2);
	int a = 0;
	write(1, &a, 4);
	a = 720;
	write(1, &a, 4);
}
