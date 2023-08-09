#include "minishell.h"

void	print_cd_err(char *path, char *str)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str, 2);
	g_error_status = 1;
}

char	*check_path(char *word, t_list **environ)
{
	char	*pathname;

	if (word)
		pathname = word;
	else
	{
		pathname = ft_getenv(environ, "HOME");
		if (!pathname)
		{
			print_cd_err("HOME", "not set");
			return (NULL);
		}
	}
	return (pathname);
}

char	*check_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		ft_putstr_fd("cd: error retrieving current directory ", 2);
		perror(CD_ERROR_MSG);
		return (NULL);
	}
	return (cwd);
}

void	ft_cd(char **simple_cmd, t_list **environ, int fd)
{
	char	*pathname;
	int		ch;
	t_list	*tmp;
	char	*cwd;

	(void)fd;
	cwd = NULL;
	g_error_status = 0;
	pathname = check_path(simple_cmd[1], environ);
	if (!pathname)
		return ;
	ch = chdir(pathname);
	if (ch < 0)
		print_cd_err(pathname, "No such file or directory");
	else
	{
		cwd = check_cwd();
		if (!cwd)
			return ;
		tmp = ft_getenvnode(environ, "PWD");
		if (tmp == NULL)
			return ;
		free(tmp->content);
		tmp->content = ft_strjoin("PWD=", cwd);
	}
}
