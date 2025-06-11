/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:06:52 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:32 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_envvar(t_data *data, const char *dollarline)
{
	int		var_len;
	char	*name;
	char	*value;

	if (!data || !dollarline)
		return (NULL);
	var_len = get_var_name_len(dollarline, 1);
	if (var_len == 0)
		return (ft_strdup(""));
	name = ft_substr(dollarline, 1, var_len);
	if (!name)
		return (NULL);
	value = get_env_value(data, name);
	free(name);
	return (value);
}

static int	copy_env_value(t_data *data,
	char *var_token, char *result, int *j)
{
	char	*env_value;
	int		k;

	if (!data || !var_token || !result || !j)
		return (0);
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

static void	process_dollar_expansion(t_data *data, t_expansion *exp)
{
	if (exp->line[*exp->i] == '$' && exp->line[*exp->i + 1] == '?')
	{
		handle_exit_status(data, exp->result, exp->j);
		*exp->i += 2;
	}
	else if (exp->line[*exp->i] == '$' && exp->line[*exp->i + 1] == '_')
	{
		handle_last_token(data, exp->result, exp->j);
		*exp->i += 2;
	}
	else if (exp->line[*exp->i] == '$' && ft_isalnum(exp->line[*exp->i + 1]))
	{
		copy_env_value(data, extract_var(exp->line, exp->i),
			exp->result, exp->j);
	}
	else
		exp->result[(*exp->j)++] = exp->line[(*exp->i)++];
}

char	*expand_dollar_variables(t_data *data, char *line)
{
	char		*result;
	int			i;
	int			j;
	int			buffer_size;
	t_expansion	exp;

	i = 0;
	j = 0;
	buffer_size = calculate_expanded_size(data, line);
	result = ft_calloc(buffer_size, sizeof(char));
	if (!result)
		return (NULL);
	exp.line = line;
	exp.result = result;
	exp.i = &i;
	exp.j = &j;
	while (line[i])
		process_dollar_expansion(data, &exp);
	result[j] = '\0';
	return (result);
}
