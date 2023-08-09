#include "minishell.h"

t_list	*dup_envp(char **envp)
{
	t_list	*environ;
	int		i;

	environ = NULL;
	i = -1;
	while (envp[++i])
		ft_lstadd_back(&environ, ft_lstnew(ft_strdup(envp[i])));
	return (environ);
}

char	*get_pwd(void)
{
	char	**spl;
	char	*tmp;
	int		size;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (".-$ ");
	spl = ft_split(tmp, '/');
	free(tmp);
	size = 0;
	while (spl[size])
		size++;
	return (ft_strjoin(spl[size - 1], "-$ "));
}

void	do_execve(t_cmd **pipeline, t_list	**environ)
{
	if (count_pipe(pipeline) == 1 && \
		is_built_in((*pipeline)->simple_cmd) > -1)
	{
		if (init_redir(*pipeline) == 1)
		{
			unlink_temp_files(*pipeline);
			exit(1);
		}
		run_cmd(*pipeline, environ, is_built_in((*pipeline)->simple_cmd), 1);
	}
	else
		pipexline(pipeline, environ);
}

int	do_parse(t_list	**environ, t_list **token_list, t_token **type_list)
{
	char	*input;

	signal(SIGINT, p_handler);
	signal(SIGQUIT, SIG_IGN);
	input = readline(get_pwd());
	if (!input)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	if (*input)
		add_history(input);
	if (tokenizer(input, token_list))
	{
		ft_lstclear(token_list, free);
		free(input);
		return (1);
	}
	expand_env(token_list, environ);
	identify_token_type(token_list, type_list);
	free(input);
	if (syntax_error(type_list) == SYNTAX_ERROR)
		return (1);
	dequotenize(type_list);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_list	*environ;
	t_list	*token_list;
	t_token	*type_list;
	t_cmd	*pipeline;

	token_list = NULL;
	type_list = NULL;
	environ = dup_envp(envp);
	(void)ac;
	(void)av;
	print_minishell();
	while (1)
	{
		if (do_parse(&environ, &token_list, &type_list))
			continue ;
		pipeline = struct_cmd(&type_list);
		change_heredoc(&pipeline);
		do_execve(&pipeline, &environ);
		ft_cmdclear(&pipeline, free);
		ft_tokenclear(&type_list, free);
	}
}
