#include "minishell.h"

void	p_handler(int sig)
{
	// printf("\b\b");
	if (sig != SIGINT)
	{
		return ;
	}
	printf("\n");
	// char *backspace_str = tgetstr("\b\b", NULL);  // 커서를 왼쪽으로 이동하는 데 사용되는 문자열
    // char *delete_char_str = tgetstr("^C", NULL);  // 문자를 삭제하는 데 사용되는 문자열
    // tputs(backspace_str, 1, putchar);  // 커서를 왼쪽으로 이동
    // tputs(delete_char_str, 1, putchar);
	// printf("\b\b");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_error_status = 1;
}

void	c_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		exit(1);
	}
	else if (sig == SIGTERM)
		exit(0);
}
