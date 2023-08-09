#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <curses.h>
# include <term.h>

# define SYNTAX_SUCCESS	0
# define SYNTAX_ERROR	258
# define SYNTAX_ERROR_MSG "minishell: syntax error near unexpected token: "
# define CD_ERROR_MSG "getcwd: cannot access parent "
# define HEREDOC_STATUS 4242

int	g_error_status;

enum	e_type
{
	GRT,
	D_GRT,
	LSR,
	D_LSR,
	PIPE,
	WORD
};

typedef struct s_exp_vars
{
	char	*exp;
	char	*new;
	char	*parsed_env;
	int		env_size;
}	t_exp_vars;

typedef struct s_token
{
	enum e_type		type;
	char			*content;
	char			*temp;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	char			**simple_cmd;
	t_token			*redir_header;
	int				io_fd[2];
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec
{
	int		repeat_fork;
	int		count;
	int		fds_prev[2];
	int		fds_next[2];
	char	**path;
}	t_exec;

/* tokenize */
int		is_delim_in_dquote(char c);
int		tokenizer(char *input, t_list **token_list);
void	make_token(char *input, t_list **token_list, int token_size);
void	check_pipe(int *token_size, char **input, t_list **token_list);
void	check_lsr_grt(int *token_size, char **input, t_list **token_list);
int		check_quto(int *flag, int *token_size, char **input);
int		help_tokenizer(char **input, int *token_size, \
						int *flag, t_list **token_list);

/* expand */
void	expand_env(t_list **token_list, t_list **environ);
void	expansion(t_list *node, char *content, int *idx, t_list **environ);
void	identify_token_type(t_list **lst, t_token **token_lst);
int		syntax_error(t_token **type_list);

/* util */
char	*ft_lst_strjoin(t_list **lst);
int		ft_strcmp(char *s1, char *s2);
t_token	*ft_token_new(int type, void *content);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void	ft_tokenclear(t_token **lst, void (*del)(void *));
void	ft_tokeniter(t_token *lst, void (*f)(t_token *));
void	ft_tokendel_mid(t_token **lst, t_token *node);
int		ft_error(t_token **type_list, char *content);
int		ft_is_blank(int c);
int		ft_atouc(char *str, unsigned char *num);
char	*ft_getenv(t_list **environ, char *word);
t_cmd	*ft_cmd_new(void);
void	ft_cmdadd_back(t_cmd **lst, t_cmd *new);
void	ft_cmdclear(t_cmd **lst, void (*del)(void *));
void	dequotenize(t_token **type_list);
t_cmd	*struct_cmd(t_token **type_list);
char	**detec_path(t_list **environ);
int		ft_is_blank(int c);
void	print_minishell(void);

/* built_in */
void	ft_export(char **simple_cmd, t_list **environ, int fd);
void	ft_cd(char **simple_cmd, t_list **environ, int fd);
void	ft_pwd(char **simple_cmd, t_list **environ, int fd);
void	ft_unset(char **simple_cmd, t_list **environ, int fd);
void	ft_exit(char **simple_cmd, t_list **environ, int fd);
void	ft_env(char **simple_cmd, t_list **environ, int fd);
void	ft_echo(char **simple_cmd, t_list **environ, int fd);
int		is_built_in(char **simple_cmd);
int		run_cmd(t_cmd *cmd, t_list **environ, int flag, int is_p);
t_list	*ft_getenvnode(t_list **environ, char *word);

/* here_doc */
void	change_heredoc(t_cmd **pipeline);
void	read_heredoc(t_cmd **pipeline);
void	unlink_temp_files(t_cmd *cmd);

/* execute */
int		init_redir(t_cmd *cmd);
void	ft_exec(t_cmd **pipeline, t_list **environ);
int		count_pipe(t_cmd **pipeline);
void	pipexline(t_cmd **pipeline, t_list **env);
char	**make_envp(t_list **env);
void	init_exec(t_exec *exec, t_cmd **pipeline, t_list **env);
void	wait_child(pid_t pid, int count);
void	close_fd(t_exec *arg);
void	fork_heredoc(t_cmd **pipeline);
void	first_child(t_exec arg, t_cmd *cmd, t_list **env);
void	first_child_help(t_exec *arg, t_cmd *cmd);
void	middle_child(t_exec arg, t_cmd *cmd, t_list **env);
void	middle_child_help(t_exec *arg, t_cmd *cmd);
void	last_child(t_exec arg, t_cmd *cmd, t_list **env);
void	last_child_help(t_exec *arg, t_cmd *cmd);
void	ft_execve(t_exec arg, t_cmd *cmd, char **envp);
char	*valid(char **path, char *command);
char	*get_pwd(void);

/* signal */
void	p_handler(int sig);
void	c_handler(int sig);

#endif
