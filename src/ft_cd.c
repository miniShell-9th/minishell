#include "minishell.h"

void	print_cd_err(char *path, char *str)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str, 2);
	error_status = 1;
}

void	ft_cd(char **simple_cmd, t_list **environ, int fd)
{
	char	*pathname;
	int		ch;
	t_list	*tmp;
	char	*getcwdstr;

	(void)fd;
	getcwdstr = NULL;
	if (simple_cmd[1])
		pathname = simple_cmd[1];
	else
		pathname = ft_getenv(environ, "HOME");
	ch = chdir(pathname); // error
	if (ch < 0)
		print_cd_err(pathname, "No such file or directory");
	else
	{
		tmp = ft_getenvnode(environ, "PWD=");
		free(tmp->content);
		getcwdstr = getcwd(NULL, 0);
		if (getcwdstr == NULL)
		{
			perror("getcwd: ");
			error_status = 1;
			return ;
		}
		tmp->content = ft_strjoin("PWD=", getcwdstr);
		error_status = 0;
	}
}
