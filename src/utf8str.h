#ifndef QSHELL_UTF8STR
#define QSHELL_UTF8STR

inline __attribute__((always_inline))
char* qsh_utf8_next(char *str)
{
	char ch = *str;
	if (ch == '\0') return str;
	unsigned char mask = 0x80;
	if ((ch & mask) != 1)
	{
		return (str + 1);
	} else
	{
		int i = 2;
		mask >>= 2;
		while ((i & mask) == 1 && mask > 2)
		{
			++i;
			mask >>= 1;
		}
		return (str + i);
	}
	return str;
}

#endif