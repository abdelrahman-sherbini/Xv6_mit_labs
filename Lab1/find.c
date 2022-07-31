#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
//#include "grep.c"
//#include <cstring.h>
int matchhere(char *, char *);
int matchstar(int, char *, char *);
int match(char *re, char *text)
{
	if (re[0] == '^')
		return matchhere(re + 1, text);
	do
	{ // must look at empty string
		if (matchhere(re, text))
			return 1;
	} while (*text++ != '\0');
	return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
	if (re[0] == '\0')
		return 1;
	if (re[1] == '*')
		return matchstar(re[0], re + 2, text);
	if (re[0] == '$' && re[1] == '\0')
		return *text == '\0';
	if (*text != '\0' && (re[0] == '.' || re[0] == *text))
		return matchhere(re + 1, text + 1);
	return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
	do
	{ // a * matches zero or more instances
		if (matchhere(re, text))
			return 1;
	} while (*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}
char *mstrcat(char *dst2, char *destination, const char *source)  //strconcat
{
	int i, j;

	for (i = 0; destination[i] != '\0'; i++)
		dst2[i] = destination[i];

	for (j = 0; source[j] != '\0'; j++)
	{
		dst2[i + j] = source[j];
	}

	// null terminate destination string
	dst2[i + j] = '\0';

	return dst2;
}

char *
fmtname(char *path)
{
	static char buf[DIRSIZ + 1];
	char *p;

	// Find first character after last slash.
	for (p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	// Return blank-padded name.
	if (strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
	return buf;
}
char *rstrip(char *src) //strip white spaces from right
{
	char *dst = '\0';
	int kk, jj;
	for (kk = strlen(src) - 3; kk > 0; kk--)
	{
		if (src[kk] != '\x20')
			break;
	}
	for (jj = 0; jj <= kk; jj++)
	{
		dst[jj] = src[jj];
	}
	dst[jj] = '\0';
	return dst;
}
int cotar = 0, indent = 0, slcnt = 0;
char concat[10][80];
void ls(char *path, int indent, char *re) //modified ls
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if ((fd = open(path, 0)) < 0)
	{
		printf(2, "ls: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0)
	{
		printf(2, "ls: cannot stat %s\n", path);
		close(fd);
		return;
	}

	switch (st.type)
	{
	case T_FILE:
		//printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
		//return(fmtname(path));
		break;

	case T_DIR:
		if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
		{
			printf(1, "ls: path too long\n");
			break;
		}
		strcpy(buf, path);
		p = buf + strlen(buf);
		*p++ = '/';
		while (read(fd, &de, sizeof(de)) == sizeof(de))
		{
			if (de.inum == 0)
				continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if (stat(buf, &st) < 0)
			{
				printf(1, "ls: cannot stat %s\n", buf);
				continue;
			}
			if (fmtname(buf)[0] != '.')
			{
				char *slas = "/";
				char *tmpstr = rstrip(fmtname(buf));
				if (cotar == 0) //if first level
				{
					strcpy(concat[0], path);

					mstrcat(concat[cotar], concat[cotar], slas); //add slash to path
					slcnt++;	//count of slash added
				}
				if (concat[cotar][strlen(concat[cotar])] != '/') //if not ending with slash add slash
				{
					mstrcat(concat[cotar + 1], concat[cotar], slas);
					//	mstrcat(concat[0],concat[0],slas);
					// printf(1, "ytq%s\n", concat);
					slcnt++;
				}
				if (cotar > 0) //if not in first level add indent count 
					indent++;
				mstrcat(concat[cotar + 1], concat[cotar], tmpstr); //add curret file fmtname stripped to new path for recursion
				//	mstrcat(concat[0],concat[0],tmpstr);
				slcnt++;
				slcnt++;
				while (--slcnt)
				{
					for (int qwer = 0; qwer < strlen(concat[cotar + 1]); qwer++) 	//remove repeating slashed
					{

						if (concat[cotar + 1][qwer] == '/' && concat[cotar + 1][qwer + 1] == '/')
							memmove(&concat[cotar + 1][qwer], &concat[cotar + 1][qwer + 1], strlen(concat[cotar + 1]) - qwer);
					}
				}
				if (match(re, concat[cotar + 1]))	//if the string matched print it
					printf(1, "%s\n", concat[cotar + 1]);

				ls(concat[cotar + 1], indent++, re); //recurse for next paths with indent 
				indent--; //after leaving ls decrease indent

				strcpy(concat[cotar + 1], concat[cotar - indent]);	//fix cwd
			}
		}
		break;
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	//int i;
	//char* var='\0';
	if (argc == 3)
	{
		ls(argv[1], 0, argv[2]);
		//printf(1,"\n %s \n ",jk);
		exit();
	}
	printf(1, "Usage: find [dir] [regex]\n");
	exit();
}
