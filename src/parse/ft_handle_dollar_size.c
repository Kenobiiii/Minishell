/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_dollar_size.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 20:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/27 20:00:00 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	calculate_var_size(t_data *data, char *line, int *i)
{
	char	*var_token;
	char	*env_value;
	int		temp_i;
	int		size;

	temp_i = *i;
	var_token = extract_var(line, &temp_i);
	env_value = find_envvar(data, var_token);
	size = 0;
	if (env_value)
	{
		size = ft_strlen(env_value);
		free(env_value);
	}
	free(var_token);
	*i = temp_i;
	return (size);
}

static int	process_dollar_size(t_data *data, char *line, int *i)
{
	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		*i += 2;
		return (10);
	}
	else if (line[*i] == '$' && line[*i + 1] == '_')
	{
		*i += 2;
		if (data->last_token)
			return (ft_strlen(data->last_token));
		return (0);
	}
	else if (line[*i] == '$' && ft_isalnum(line[*i + 1]))
		return (calculate_var_size(data, line, i));
	else
	{
		(*i)++;
		return (1);
	}
}

int	calculate_expanded_size(t_data *data, char *line)
{
	int		i;
	int		total_size;

	i = 0;
	total_size = 0;
	while (line[i])
		total_size += process_dollar_size(data, line, &i);
	return (total_size + 100);
}
