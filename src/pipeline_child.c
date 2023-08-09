#include "minishell.h"

char	**make_envp(t_list **env)
{
	t_list	*iter;
	char	**ret;
	int		size;

	iter = *env;
	size = 0;
	while (iter)
	{
		size++;
		iter = iter->next;
	}
	ret = (char **)malloc(sizeof(char *) * (size + 1));
	if (!ret)
		exit(1);
	iter = *env;
	size = 0;
	while (iter)
	{
		ret[size++] = ft_strdup(iter->content);
		iter = iter->next;
	}
	ret[size] = NULL;
	return (ret);
}

void	first_child(t_exec arg, t_cmd *cmd, t_list **env)
{
	char	**envp;

	envp = make_envp(env);
	if (init_redir(cmd) == 1)
	{
		unlink_temp_files(cmd);
		exit(1);
	}
	first_child_help(&arg, cmd);
	unlink_temp_files(cmd);
	if (is_built_in(cmd->simple_cmd) > -1)
	{
		run_cmd(cmd, env, is_built_in(cmd->simple_cmd), 0);
		exit(g_error_status);
	}
	if (cmd->simple_cmd[0] == NULL)
		exit(g_error_status);
	ft_execve(arg, cmd, envp);
}

void	middle_child(t_exec arg, t_cmd *cmd, t_list **env)
{
	char	**envp;

	envp = make_envp(env);
	close(arg.fds_next[0]);
	close(arg.fds_prev[1]);
	if (init_redir(cmd) == 1)
	{
		unlink_temp_files(cmd);
		exit(1);
	}
	middle_child_help(&arg, cmd);
	unlink_temp_files(cmd);
	if (is_built_in(cmd->simple_cmd) > -1)
	{
		run_cmd(cmd, env, is_built_in(cmd->simple_cmd), 0);
		exit(g_error_status);
	}
	if (cmd->simple_cmd[0] == NULL)
		exit(g_error_status);
	ft_execve(arg, cmd, envp);
}

void	last_child(t_exec arg, t_cmd *cmd, t_list **env)
{
	char	**envp;

	envp = make_envp(env);
	close(arg.fds_prev[1]);
	if (init_redir(cmd) == 1)
	{
		unlink_temp_files(cmd);
		exit(1);
	}
	last_child_help(&arg, cmd);
	unlink_temp_files(cmd);
	if (is_built_in(cmd->simple_cmd) > -1)
	{
		run_cmd(cmd, env, is_built_in(cmd->simple_cmd), 0);
		exit(g_error_status);
	}
	close(arg.fds_next[0]);
	close(arg.fds_next[1]);
	if (cmd->simple_cmd[0] == NULL)
		exit(g_error_status);
	ft_execve(arg, cmd, envp);
}
