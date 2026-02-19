/*                                                                  */
/*   RUNLEN.C  :  Compression by Run-Length Encoding   	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <string.h>
#include <dir.h>

#define ESCAPE 0xB4
#define IDENT1 0x11
#define IDENT2 0x22

void make_dstname(char dstname[], char srcname[])
{
	char temp[256];
	fnsplit(srcname, temp, temp, dstname, temp);
	strcat(dstname, ".rle");
}

long file_length(char filename[])
{
	FILE *fp;
	long l;
	if ((fp = fopen(filename, "rb")) == NULL)
		return 0L;
	fseek(fp, 0, SEEK_END);
	l = ftell(fp);
	fclose(fp);
	return l;
}

void flush(char code[], int len, FILE *fp)
{
	int i;
	for (i = 0; i < len; i++)
		putc(code[i], fp);
}

void run_length_comp(FILE *src, char *srcname)
{
	int cur;
	int code_len;
	int length;
	unsigned char code[10];
	char dstname[13];
	FILE *dst;

	make_dstname(dstname, srcname);
	if ((dst = fopen(dstname, "wb")) == NULL)
	{
		printf("\n Error : Can't create file.");
		fcloseall();
		exit(1);
	}

	putc(IDENT1, dst); /* make header */
	putc(IDENT2, dst);
	fputs(srcname, dst);
	putc(NULL, dst);

	cur = getc(src);
	code_len = length = 0;

	while (!feof(src))
	{
		if (length == 0)
		{
			if (cur != ESCAPE)
			{
				code[code_len++] = cur;
				length++;
			}
			else
			{
				code[code_len++] = ESCAPE;
				code[code_len++] = ESCAPE;
				flush(code, code_len, dst);
				length = code_len = 0;
			}
			cur = getc(src);
		}
		else if (length == 1)
		{
			if (cur != code[0])
			{
				flush(code, code_len, dst);
				length = code_len = 0;
			}
			else
			{
				length++;
				code[code_len++] = cur;
				cur = getc(src);
			}
		}
		else if (length == 2)
		{
			if (cur != code[1])
			{
				flush(code, code_len, dst);
				length = code_len = 0;
			}
			else
			{
				length++;
				code_len = 0;
				code[code_len++] = ESCAPE;
				code[code_len++] = cur;
				code[code_len++] = length;
				cur = getc(src);
			}
		}
		else if (length > 2)
		{
			if (cur != code[1] || length > 254)
			{
				flush(code, code_len, dst);
				length = code_len = 0;
			}
			else
			{
				code[code_len - 1]++;
				length++;
				cur = getc(src);
			}
		}
	}
	flush(code, code_len, dst);
	fclose(dst);
}

void run_length_decomp(FILE *src)
{
	int cur;
	char srcname[13];
	FILE *dst;
	int i = 0, j;
	int length;

	cur = getc(src);
	if (cur != IDENT1 || getc(src) != IDENT2)
	{
		printf("\n Error : That file is not Run-Length Encoding file");
		fcloseall();
		exit(1);
	}
	while ((cur = getc(src)) != NULL)
		srcname[i++] = cur;
	srcname[i] = NULL;
	if ((dst = fopen(srcname, "wb")) == NULL)
	{
		printf("\n Error : Disk full? ");
		fcloseall();
		exit(1);
	}
	cur = getc(src);
	while (!feof(src))
	{
		if (cur != ESCAPE)
			putc(cur, dst);
		else
		{
			cur = getc(src);
			if (cur == ESCAPE)
				putc(ESCAPE, dst);
			else
			{
				length = getc(src);
				for (j = 0; j < length; j++)
					putc(cur, dst);
			}
		}
		cur = getc(src);
	}
	fclose(dst);
}

void main(int argc, char *argv[])
{
	FILE *src;
	long s, d;
	char dstname[13];

	if (argc < 3)
	{
		printf("\n Usage : RUNLEN <a or x> <filename>");
		exit(1);
	}

	s = file_length(argv[2]);

	if ((src = fopen(argv[2], "rb")) == NULL)
	{
		printf("\n Error : That file does not exist.");
		exit(1);
	}
	if (strcmp(argv[1], "a") == 0)
	{
		run_length_comp(src, argv[2]);
		make_dstname(dstname, argv[2]);
		d = file_length(dstname);
		printf("\nFile compressed to %d%%", (int)((double)d / s * 100.));
	}
	else if (strcmp(argv[1], "x") == 0)
	{
		run_length_decomp(src);
		printf("\nFile decompressed & created.");
	}
	fclose(src);
}
