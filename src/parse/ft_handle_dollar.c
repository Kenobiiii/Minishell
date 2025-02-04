/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:06:52 by paromero          #+#    #+#             */
/*   Updated: 2025/02/04 12:12:10 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_envvar(t_data *data, const char *dollarline)
{
	int		i;
	int		var_len;
	char	*name;
	t_env	*env;
	char	*equal_sign;
	int		key_len;

	i = 1;
	var_len = 0;
	while (dollarline[i + var_len] && \
		(ft_isalnum(dollarline[i + var_len]) || dollarline[i + var_len] == '_'))
		var_len++;
	if (var_len == 0)
		return (ft_strdup(""));
	name = ft_substr(dollarline, 1, var_len);
	if (!name)
		return (NULL);
	env = data->env;
	while (env)
	{
		equal_sign = ft_strchr(env->value, '=');
		if (equal_sign)
		{
			key_len = equal_sign - env->value;
			if (key_len == (int)ft_strlen(name) && \
				!ft_strncmp(env->value, name, key_len))
			{
				free(name);
				return (ft_strdup(equal_sign + 1));
			}
		}
		env = env->next;
	}
	free(name);
	return (ft_strdup(""));
}


char	*ft_handledollar(t_data *data, const char *line)
{
	char	*result;
	int		i;
	int		j;
	int		k;
	int		var_len;
	char	*var_token;
	char	*env_value;

	i = 0;
	j = 0;
	result = malloc(sizeof(char) * (ft_strlen(line) * 10 + 1));
	if (!result)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$')
		{
			var_len = 0;
			while (line[i + 1 + var_len] && \
				(ft_isalnum(line[i + 1 + var_len]) || \
				line[i + 1 + var_len] == '_'))
				var_len++;
			if (var_len)
			{
				var_token = ft_substr(line, i, var_len + 1);
				env_value = find_envvar(data, var_token);
				free(var_token);
				if (env_value)
				{
					free (result);
					result = malloc(sizeof(char) * (ft_strlen(env_value) + 1));
					if (!result)
						return (NULL);
					k = 0;
					while (env_value[k])
						result[j++] = env_value[k++];
					free(env_value);
				}
				i += var_len + 1;
				continue ;
			}
			else
				result[j++] = '$';
		}
		else
			result[j++] = line[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
