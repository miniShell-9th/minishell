#include "minishell.h"

int	open_redir_in(t_cmd *cmd, t_token *iter)
{
	if (cmd->io_fd[0] != 0)
		close(cmd->io_fd[0]);
	cmd->io_fd[0] = open(iter->content, O_RDONLY);
	if (cmd->io_fd[0] < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(iter->content);
		return (1);
	}
	return (0);
}

int	open_redir_out(t_cmd *cmd, t_token *iter)
{
	if (cmd->io_fd[1] != 1)
		close(cmd->io_fd[1]);
	cmd->io_fd[1] = open(iter->content, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (cmd->io_fd[1] < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(iter->content);
		return (1);
	}
	return (0);
}

int	init_redir(t_cmd *cmd)
{
	t_token	*redir_iter;
	int		type;

	redir_iter = cmd->redir_header;
	while (redir_iter)
	{
		type = redir_iter->type;
		if (type == LSR || type == D_LSR)
		{
			if (open_redir_in(cmd, redir_iter) == 1)
				return (1);
		}
		else if (type == GRT)
		{
			if (open_redir_out(cmd, redir_iter) == 1)
				return (1);
		}
		else if (type == D_GRT)
		{
			if (open_redir_out(cmd, redir_iter) == 1)
				return (1);
		}
		redir_iter = redir_iter->next;
	}
	return (0);
}
