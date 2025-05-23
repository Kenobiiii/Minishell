/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:06:52 by paromero          #+#    #+#             */
/*   Updated: 2025/04/08 18:39:53 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_envvar(t_data *data, const char *dollarline)
{
	int		var_len;
	char	*name;
	char	*value;

	var_len = 0;
	while (dollarline[1 + var_len]
		&& (ft_isalnum(dollarline[1 + var_len])
			|| dollarline[1 + var_len] == '_'))
		var_len++;
	if (var_len == 0)
		return (ft_strdup(""));
	name = ft_substr(dollarline, 1, var_len);
	if (!name)
		return (NULL);
	value = get_env_value(data, name);
	free(name);
	return (value);
}

static char	*extract_var(const char *line, int *i)
{
	int		var_len;
	char	*var_token;

	var_len = 0;
	while (line[*i + 1 + var_len]
		&& (ft_isalnum(line[*i + 1 + var_len])
			|| line[*i + 1 + var_len] == '_'))
		var_len++;
	var_token = ft_substr(line, *i, var_len + 1);
	*i += var_len + 1;
	return (var_token);
}

static int	copy_env_value(t_data *data,
	char *var_token, char *result, int *j)
{
	char	*env_value;
	int		k;

	k = 0;
	env_value = find_envvar(data, var_token);
	if (env_value)
	{
		while (env_value[k])
			result[(*j)++] = env_value[k++];
		free (env_value);
	}
	free(var_token);
	return (1);
}

static	void	handle_exit_status(t_data *data, char *result, int *j)
{
	char	*wstatus;
	int		k;

	wstatus = ft_itoa(data->wstatus);
	if (!wstatus)
		return ;
	k = 0;
	while (wstatus[k])
		result[(*j)++] = wstatus[k++];
	free(wstatus);
}

char	*ft_handledollar(t_data *data, char *line)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = ft_calloc(ft_strlen(line) * 10 + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			handle_exit_status(data, result, &j);
			i += 2;
		}
		else if (line[i] == '$' && ft_isalnum(line[i + 1]))
		{
			copy_env_value(data, extract_var(line, &i), result, &j);
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}
