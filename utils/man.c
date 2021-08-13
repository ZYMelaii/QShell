//! QShell Util: man

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>
#include <direct.h>

char _binpath[255];

void getbinpath()
{
	int len = strrchr(_pgmptr, '\\') - _pgmptr;
	char *p = _binpath;
	char *q = _pgmptr;
	while (len-- > 0)
	{
		*p++ = *q++;
	}
	*p = '\0';
}

int _matchany(const char *src, ...)
{
	va_list v_opts;
	va_start(v_opts, src);
	const char *opt = NULL;
	while ((opt = va_arg(v_opts, const char*)) != NULL)
	{
		if (strcmp(src, opt) == 0)
		{
			va_end(v_opts);
			return 1;
		}
	}
	va_end(v_opts);
	return 0;
}

#define matchany(src, opts...) _matchany(src, opts, NULL)

void getmanpath(const char *cmd, char *pathbuf)
{
	const char *rmost = cmd + strlen(cmd) - 1;

	const char *start1 = strrchr(cmd, '\\');
	const char *start2 = strrchr(cmd, '/');
	const char *start = start1 > start2 ? start1 : start2;
	if (start == NULL)
	{
		start = cmd;
	} else
	{
		++start;
	}

	const char *end = strrchr(cmd, '.');
	if (end == NULL)
	{
		end = rmost;
	}
	if (rmost == end)
	{
		++end;
	}

	int len = (size_t)(end - start);
	sprintf(pathbuf, "%s/man/", _binpath);
	char *p = pathbuf + strlen(pathbuf);
	const char *q = start;
	while (len-- > 0)
	{
		*p++ = *q++;
	}
	strcpy(p, ".man");
}

int buildman(const char *cmd)
{
	char dir[255];
	sprintf(dir, "%s/man", _binpath);
	if (access(dir, F_OK) != 0)
	{
		mkdir(dir);
	}

	char *const argv[] = { (char *const)cmd, "--help", NULL };

	char fnman[256];
	getmanpath(cmd, fnman);

	freopen(fnman, "w", stdout);
	fclose(stderr);
	
	int exitcode = 0;
	intptr_t hproc = spawnvp(_P_NOWAIT, cmd, argv);
	intptr_t hd = cwait(&exitcode, hproc, 0);

	fflush(stdout);
	fclose(stdout);

	FILE *fp = fopen(fnman, "r");
	char ch = fgetc(stdout);
	fclose(fp);
	if (ch == EOF)
	{
		remove(fnman);
	}

	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);

	fflush(stdout);
	fflush(stderr);

	return exitcode;
}

int main(int argc, char const *argv[])
{
	getbinpath();

	if (argc == 1)
	{
		goto help;
	} else if (argc == 2)
	{
		if (matchany(argv[1], "-h", "--help"))
		{
			goto help;
		} else if (matchany(argv[1], "-v", "--version"))
		{
			goto version;
		} else if (argv[1][0] == '-')
		{
			goto bad_option;
		} else
		{
			char fnman[256];
			getmanpath(argv[1], fnman);
			if (access(fnman, F_OK | R_OK) != 0)
			{
				int exitcode = buildman(argv[1]);
				if (exitcode < 0)
				{
					if (exitcode == -1)
					{
						goto bad_cmd;
					}
					goto bad_man;
				}
			}
			FILE *fp = fopen(fnman, "r");
			char ch;
			while((ch = fgetc(fp)) != EOF)
			{
				putchar(ch);
			}
			fclose(fp);
			goto done;
		}
	}

	goto fail;

	help:
		printf(
			"Usage:  man [OPTION|COMMAND]\n"
			"List help information about the command.\n"
			"\n"
			"Options:\n"
			"    -h  --help                  print this page\n"
			"    -v  --version               print version info\n"
			"\n"
			"Report bugs to <13282135976@sina.cn>.\n"
			);
		goto done;

	version:
		printf(
			"man (QShell utils) 1.0.0\n"
			"Authored by ZYMelaii; 2021-08-13"
			);
		goto done;

	bad_option:
		printf("man: unknown option \'%s\'\n", argv[1]);
		goto fail;

	bad_cmd:
		printf("man: unknown command \'%s\'\n", argv[1]);
		goto fail;

	bad_man:
		printf("man: cannot open manpage of \'%s\'", argv[1]);
		goto fail;

	done:
		return 0;

	fail:
		return -1;
}