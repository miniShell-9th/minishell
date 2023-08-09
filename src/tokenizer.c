#include "minishell.h"

int	tokenizer(char *input, t_list **token_list)
{
	int		token_size;
	int		flag;

	while (*input)
	{
		token_size = 0;
		flag = 0;
		while (ft_is_blank(*input))
			input++;
		while (*input && !ft_is_blank(*input))
		{
			if (help_tokenizer(&input, &token_size, &flag, token_list))
				return (1);
		}
		if (token_size)
			make_token(input - token_size, token_list, token_size);
	}
	return (0);
}
