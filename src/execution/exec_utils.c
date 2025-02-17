/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:59:23 by anggalle          #+#    #+#             */
/*   Updated: 2025/02/17 19:37:25 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	count_nodes(t_env *head)
{
	size_t	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	**list_to_array(t_env *head)
{
	size_t	count;
	size_t	i;
	size_t	j;
	char	**array;

	count = count_nodes(head);
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (head)
	{
		array[i] = strdup(head->value);
		if (!array[i++])
		{
			j = 0;
			while (j < i)
				free((char *)array[j++]);
			free(array);
			return (NULL);
		}
		head = head->next;
	}
	array[i] = NULL;
	return (array);
}

void	free_array(const char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free((char *)array[i]);
		i ++;
	}
	free(array);
}

void	analyse_status(t_data *data)
{
	int	status;

	status = data->wstatus;
	if (WIFEXITED(status))
	{
		data->wstatus = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		data->wstatus = 128 + WTERMSIG(status);
	}
	else
	{
		data->wstatus = status;
	}
}
