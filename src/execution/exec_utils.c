/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:59:23 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/31 11:37:49 by paromero         ###   ########.fr       */
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
		array[i] = ft_strdup(head->value);
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
	int	termsig;

	status = data->wstatus;
	if (WIFEXITED(status))
	{
		data->wstatus = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		termsig = WTERMSIG(status);
		if (termsig == SIGQUIT)
			write(STDOUT_FILENO, "Quit\n", 5);
		data->wstatus = 128 + termsig;
	}
	else
	{
		data->wstatus = status;
	}
}

/**
 *? Handles process errors and displays descriptive messages
 */
int	handle_process_error(int result, char *error_msg)
{
	if (result < 0)
	{
		perror(error_msg);
		return (-1);
	}
	return (result);
}
