#include "minishell.h"

void	ft_env(char **simple_cmd, t_list **environ, int fd)
{
	t_list	*iter;

	g_error_status = 0;
	if (!simple_cmd)
		return ;
	iter = *environ;
	while (iter)
	{
		if (ft_strchr(iter->content, '='))
			ft_putendl_fd(iter->content, fd);
		iter = iter->next;
	}
}
