#include "minishell.h"

int	is_delim(char c)
{
	return (c == '$' || c == '\'' || c == '\"');
}

char	*ft_lst_strjoin(t_list **lst)
{
	t_list	*iter;
	char	*res;
	char	*tmp;

	res = ft_strdup((char *)(*lst)->content);
	iter = (*lst)->next;
	tmp = 0;
	while (iter)
	{
		tmp = res;
		res = ft_strjoin(res, iter->content);
		free(tmp);
		iter = iter->next;
	}
	return (res);
}

void	expansion(t_list *node, char *content, int *idx, t_list **environ)
{
	t_list	*lst;
	char	*str;
	char	*env;
	int		key_size;
	int		q_flag;
	int		i;

	lst = NULL;
	i = 0;
	q_flag = 0;
	while (content[i])
	{
		key_size = 0;
		if (content[i + key_size] == '$' && q_flag != '\'')
		{
			i++;
			while (content[i + key_size] && !is_delim(content[i + key_size]))
			{
				if (q_flag == '\"' && (ft_is_blank(content[i + key_size]) \
					|| content[i + key_size] == '$'))
					break ;
				key_size++;
			}
			str = ft_substr(content, i, key_size);
			env = ft_getenv(environ, str);
			if (env == NULL && !ft_strncmp(str, "?", 1))
				env = ft_strjoin(ft_itoa(g_error_status), \
					ft_substr(str, 1, ft_strlen(str)));
			else if (env == NULL && key_size != 0)
				env = ft_strdup("");
			else if (env == NULL && key_size == 0)
				env = ft_strdup("$");
			free(str);
			ft_lstadd_back(&lst, ft_lstnew(env));
			i += key_size;
		}
		else if (content[i + key_size] == '\'')
		{
			if (q_flag == 0)
				q_flag = '\'';
			else if (q_flag == '\'')
				q_flag = 0;
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup("\'")));
			i++;
		}
		else if (content[i + key_size] == '\"')
		{
			if (q_flag == 0)
				q_flag = '\"';
			else if (q_flag == '\"')
				q_flag = 0;
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup("\"")));
			i++;
		}
		else
		{
			while (content[i + key_size]
				&& !ft_strchr("\'\"", content[i + key_size]))
			{
				if (q_flag != '\'' && content[i + key_size] == '$')
					break ;
				key_size++;
			}
			str = ft_substr(content, i, key_size);
			ft_lstadd_back(&lst, ft_lstnew(str));
			i += key_size;
		}
	}
	*idx = i;
	free(node->content);
	node->content = ft_lst_strjoin(&lst);
	ft_lstclear(&lst, free);
}

void	expand_env(t_list **token_list, t_list **environ)
{
	t_list	*iter;
	t_list	*iter_next;
	t_list	*tmp_list;
	char	*tmp;
	int		flag;
	int		i;
	int		exp_flag;

	exp_flag = 0;
	tmp_list = NULL;
	iter = *token_list;
	while (iter)
	{
		tmp = (char *)iter->content;
		if (iter->prev && !ft_strcmp("<<", iter->prev->content))
		{
			iter = iter->next;
			if (!iter)
				break ;
		}
		iter_next = iter->next;
		i = 0;
		if (ft_strchr(tmp, '$'))
		{
			while (tmp[i])
			{
				flag = 0;
				if (tmp[i] == '\'' || tmp[i] == '\"')
				{
					flag = tmp[i];
					while (flag == '\'' && tmp[i])
					{
						i++;
						if (tmp[i] == flag)
						{
							flag = 0;
							i++;
						}
					}
					while (tmp[i] && flag == '\"')
					{
						if (tmp[i] != '$')
							i++;
						if (tmp[i] == '$')
						{
							exp_flag = 1;
							expansion(iter, iter->content, &i, environ);
							if (!(iter->prev && (!ft_strcmp(iter->prev->content, "<") || \
								!ft_strcmp(iter->prev->content, ">") || !ft_strcmp(iter->prev->content, ">>"))))
								tokenizer(iter->content, &tmp_list);
							ft_lstadd_mid(iter, &tmp_list);
							ft_lstclear(&tmp_list, free);
							flag = 0;
							break ;
						}
					}
					if (exp_flag)
						break ;
				}
				else
				{
					if (tmp[i] == '$')
					{
						exp_flag = 1;
						expansion(iter, iter->content, &i, environ);
						if (!(iter->prev && (!ft_strcmp(iter->prev->content, "<") || \
							!ft_strcmp(iter->prev->content, ">") || !ft_strcmp(iter->prev->content, ">>"))))
							tokenizer(iter->content, &tmp_list);
						ft_lstadd_mid(iter, &tmp_list);
						ft_lstclear(&tmp_list, free);
						break ;
					}
					i++;
				}
			}
		}
		iter = iter->next;
		if (exp_flag && iter != NULL)
		{
			ft_lstdel_mid(token_list, iter->prev);
			iter = iter_next;
			exp_flag = 0;
		}
	}
}
