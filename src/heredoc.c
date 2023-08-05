#include "minishell.h"

char	*heredoc_file()
{
	int		heredoc_idx;
	char	*tmp;
	char	*itoa;

	heredoc_idx = 0;
	itoa = ft_itoa(heredoc_idx);
	tmp = ft_strjoin(".tmp_", itoa);
	while (access(tmp, F_OK) >= 0)
	{
		heredoc_idx++;
		free(tmp);
		free(itoa);
		itoa = ft_itoa(heredoc_idx);
		tmp = ft_strjoin(".tmp_", itoa);
	}
	free(itoa);
	return (tmp);
}

void	heredoc_readline(int fd, char *lim)
{
	char	*input;

	while (1)
	{
		input = readline("heredoc> ");
		if (!input)
		{
			return ;
		}
		if (!ft_strcmp(input, lim))
		{
			free(input);
			return ;
		}
		else
			ft_putendl_fd(input, fd);
		free(input);
	}
}

void change_heredoc(t_cmd **pipeline)
{
	t_cmd *iter;
	t_token *red_iter;
	int fd;
	char *file_str;

	iter = *pipeline;
	while (iter)
	{
		red_iter = iter->redir_header;
		while (red_iter)
		{
			if (red_iter->type == D_LSR)
			{
				file_str = heredoc_file();
				fd = open(file_str, O_RDWR | O_CREAT, 0644);
				if (fd == -1)
				{
					perror("minishell: ");
					exit(1);
				}
				heredoc_readline(fd, red_iter->content);
				free(red_iter->content);
				red_iter->content = file_str;
				close(fd);
			}
			red_iter = red_iter->next;
		}
		iter = iter->next;
	}
}