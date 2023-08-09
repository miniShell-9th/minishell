#include "minishell.h"

char	*ft_getenv(t_list **environ, char *word)
{
	char	*ret;
	char	*str;
	int		is_match;
	size_t	wd_len;
	t_list	*iter;

	iter = *environ;
	wd_len = ft_strlen(word);
	is_match = 1;
	ret = NULL;
	str = ft_strjoin(word, "=");
	while (iter)
	{
		is_match = ft_strncmp(iter->content, str, wd_len + 1);
		if (!is_match)
			break ;
		iter = iter->next;
	}
	free(str);
	if (!is_match)
		ret = ft_substr(iter->content, wd_len + 1, ft_strlen(iter->content));
	return (ret);
}

char	*valid(char **path, char *command)
{
	char	*tmp;
	char	*path_cmd;
	int		i;

	i = 0;
	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	else
	{
		path_cmd = ft_strjoin("/", command);
		while (path && path[i])
		{
			tmp = ft_strjoin(path[i], path_cmd);
			if (access(tmp, F_OK) > -1)
			{
				free(path_cmd);
				return (tmp);
			}
			free(tmp);
			i++;
		}
	}
	free(path_cmd);
	return (NULL);
}

int	ft_is_blank(int c)
{
	return (c == ' ' || c == '\t');
}
