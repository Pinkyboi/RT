#include <unistd.h>

int	main(void)
{
	char c;
	int error = -1;
	while (read(0, &c, 1))
	{
		if (c == '!')
		{
			read(0, &c, 1);
			break ;
		}
	}
	write(1, &error, 4);
}
