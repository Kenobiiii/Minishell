/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:51:47 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/11 17:52:56 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_var_key(char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

static void	remove_variable(t_env **env, char *var)
{
	t_env	*prev;
	t_env	*current;
	char	*key;

	prev = NULL;
	current = *env;
	while (current)
	{
		key = get_var_key(current->value);
		if (ft_strncmp(key, var, ft_strlen(var)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->value);
			free(current);
			free(key);
			break ;
		}
		free(key);
		prev = current;
		current = current->next;
	}
}

static int	is_valid_identifier(char *var)
{
	int	i;

	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	i = 1;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	unset_builtin(t_data *data)
{
	int	i;
	int	ret;

	ret = 1;
	if (!data->ast->args[1])
		return (1);
	i = 0;
	while (data->ast->args[i])
	{
		if (!is_valid_identifier(data->ast->args[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(data->ast->args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
			remove_variable(&data->env, data->ast->args[i]);
		i++;
	}
	data->wstatus = 0;
	return (ret);
}
