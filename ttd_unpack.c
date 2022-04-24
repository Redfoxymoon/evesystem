#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#include "readbytes.h"

int main(int argc, char *argv[])
{
	char filename[40];
	FILE *archive, *out;
	unsigned char buf[40];
	unsigned char magic[] = {0x62, 0x02, 0x00, 0x00};
	unsigned int file_offset, file_size, end_of_file;

	if(argc != 2)
	{
		printf("%s: <archive.ttd>\n", basename(argv[0]));
		exit(1);
	}

	archive = fopen(argv[1], "rb");

	if(fread(buf, 1, 4, archive) != 4)
	{
		puts("not an evesystem archive, bailing");
		exit(1);
	}

	/* magic? */
	if(memcmp(buf, magic, 4))
	{
		puts("not an evesystem archive, bailing");
		exit(1);
	}

	fseek(archive, 0, SEEK_END);
	end_of_file = ftell(archive);
	fseek(archive, 4, SEEK_SET);

	while(1)
	{
		fread(buf, 1, 40, archive);

		/* buf[0] is full entry size, useless? always 40? */
		file_size = read_uint32_le(&buf[4]);
		file_offset = read_uint32_le(&buf[8]);

		/* archive is terminated w/ null entry */
		if(!file_size || !file_offset)
			break;

		if(file_offset > end_of_file)
		{
			puts("unexpected end of archive");
			exit(1);
		}

		strcpy(filename, (const char *)&buf[16]);

#if 0
		printf("%s\nfile_offset %d\nfile_size %d\n", filename, file_offset, file_size);
#else
		printf("%s\n", filename);
#endif

		out = fopen(filename, "wb");

		while(file_size)
		{
			putc(fgetc(archive), out);
			file_size--;
		}
		fclose(out);
	}

	return 0;
}
