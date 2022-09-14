#include "include/my_input.h"

int my_exit(int val)
{
	(val == 0) ? 0 : 1;
}

char* my_input(void)
{
	unsigned int len_max = 128, current_size = 0, i = 0;
	int c = EOF;
	char* str = malloc(len_max);

	if (str == NULL)
		my_exit(1);

	current_size = len_max;
	for (; (c = getchar()) != '\n' && c != EOF;) {
		str[i++] = (char)c;
		if (i == current_size)
		{
			current_size = i + len_max;
			str = realloc(str, current_size);
		}
	}
	str[i] = '\0';
	return str;
}

void my_puts(char* str)
{
	for (int i = 0; str[i] != '\0'; i++)
		putc(str[i], stdout);
	fflush(stdout);
}

void my_free(char* str)
{
	free(str);
	str = ((void*)0);
}


/*
Example :
	char *str = my_input();

	my_puts(str);
	my_free(str);
	return 0;


*/
