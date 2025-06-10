/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:39:37 by paromero          #+#    #+#             */
/*   Updated: 2025/05/30 19:43:17 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_env_match(t_env *env, const char *name)
{
	char	*equal_sign;
	int		key_len;

	while (env)
	{
		equal_sign = ft_strchr(env->value, '=');
		if (equal_sign)
		{
			key_len = equal_sign - env->value;
			if (key_len == (int)ft_strlen(name)
				&& !ft_strncmp(env->value, name, key_len))
				return (ft_strdup(equal_sign + 1));
		}
		env = env->next;
	}
	return (NULL);
}

char	*get_env_value(t_data *data, const char *name)
{
	char	*result;

	result = find_env_match(data->env, name);
	if (result)
		return (result);
	return (ft_strdup(""));
}

int	get_var_name_len(const char *str, int start)
{
	int	var_len;

	var_len = 0;
	while (str[start + var_len]
		&& (ft_isalnum(str[start + var_len])
			|| str[start + var_len] == '_'))
		var_len++;
	return (var_len);
}

char	*extract_var(const char *line, int *i)
{
	int		var_len;
	char	*var_token;

	var_len = get_var_name_len(line, *i + 1);
	var_token = ft_substr(line, *i, var_len + 1);
	*i += var_len + 1;
	return (var_token);
}

void	handle_exit_status(t_data *data, char *result, int *j)
{
	char	*wstatus;
	int		k;

	wstatus = ft_itoa(data->wstatus);
	if (!wstatus)
		return ;
	k = 0;
	while (wstatus[k])
	{
		result[(*j)] = wstatus[k];
		(*j)++;
		k++;
	}
	free(wstatus);
}
