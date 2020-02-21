#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	uint32_t size;
	int i;
	char	*content;
	uint64_t result;
	int ret;

	FILE *fd = fopen("/dev/ttys000", "w");

	ret = read(0, &size, sizeof(uint32_t));

	fprintf(fd, "\nsize: %d, ret: %d\n", size, ret);
	content = malloc(sizeof(char) * size);
	ret = read(0, content, size);

	fprintf(fd, "\ninput: %s, ret: %d\n", content, ret);
	result = 0;
	i = -1;
	while (++i < size)
		result += content[i];
	size = sizeof(result);

	fprintf(fd, "\nsize: %d\n", size);
	write(1, &size, sizeof(uint32_t));

	fprintf(fd, "\noutput: %llu\n", result);
	write(1, &result, size);
	return (0);
}
