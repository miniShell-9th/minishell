#include "minishell.h"

void	init_exec(t_exec *exec, t_cmd **pipeline, t_list **env)
{
	exec->count = count_pipe(pipeline);
	exec->repeat_fork = 0;
	exec->path = detec_path(env);
}

void	wait_child(pid_t pid, int count)
{
	int	fork_count;
	int	status;

	fork_count = 0;
	while (pid != 0 && fork_count < count)
	{
		if (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status))
				g_error_status = WEXITSTATUS(status);
			else if (WTERMSIG(status) == 2)
				g_error_status = 130;
			else if (WTERMSIG(status) == 3)
			{
				g_error_status = 131;
				printf("QUIT: 3\n");
			}
		}
		else
			return ;
		fork_count++;
	}
	return ;
}

void	close_fd(t_exec *arg)
{
	close(arg->fds_prev[0]);
	close(arg->fds_prev[1]);
	if (arg->count > 0)
	{
		close(arg->fds_next[1]);
		close(arg->fds_next[0]);
	}
}

void	fork_heredoc(t_cmd **pipeline)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		signal(SIGINT, c_handler);
		signal(SIGTERM, c_handler);
		signal(SIGQUIT, SIG_IGN);
		read_heredoc(pipeline);
	}
	if (waitpid(-1, &status, 0) > 0)
	{
		g_error_status = WEXITSTATUS(status);
	}
}
