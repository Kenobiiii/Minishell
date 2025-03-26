/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anggalle <anggalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:01 by anggalle          #+#    #+#             */
/*   Updated: 2025/03/26 14:42:25 by anggalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_putstatus_fd(char *status, int fd)
{
	int	i;

	i = 0;
	if (status != NULL)
	{
		while (status[i])
			write (fd, &status[i++], 1);
	}
	free (status);
}

char	*remove_mask(char *str)
{
    char	*cleaned;
    int		i;
    int		j;

    cleaned = malloc(ft_strlen(str) + 1);
    if (!cleaned)
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] != '\x01') // Ignorar el marcador
            cleaned[j++] = str[i];
        i++;
    }
    cleaned[j] = '\0';
    return (cleaned);
}

int	echo_builtin(t_data *data)
{
	int	i;
	int	j;
	int	print_new_line;

	print_new_line = 1;
	i = 1;

	if (data->ast->args[1])
	{
		while (data->ast->args[i])
		{
			if (ft_strncmp(data->ast->args[i], "-n", 2) == 0)
			{
				if (ft_strncmp(data->ast->args[i], "-n",
						ft_strlen(data->ast->args[i])) == 0)
					print_new_line = 0;
				else
				{
					j = 2;
					while (data->ast->args[i][j] == 'n')
						j ++;
					if (data->ast->args[i][j])
					{
						ft_putstr_fd(data->ast->args[i], STDOUT_FILENO);
						if (data->ast->args[i + 1])
							ft_putstr_fd(" ", STDOUT_FILENO);
						print_new_line = 1;
					}
					else
						print_new_line = 0;
				}
			}
			else if (ft_strncmp(data->ast->args[i], "$?", 2) == 0)
				ft_putstatus_fd(ft_itoa(data->wstatus), STDOUT_FILENO);
			else
			{
				// Eliminar el marcador '\x01' antes de imprimir
				char *cleaned_arg = remove_mask(data->ast->args[i]);
				if (cleaned_arg && cleaned_arg[0] != '\0') // Verifica que no esté vacío
				{
					ft_putstr_fd(cleaned_arg, STDOUT_FILENO);
					if (data->ast->args[i + 1])
						ft_putstr_fd(" ", STDOUT_FILENO);
				}
				free(cleaned_arg);
			}
			i ++;
		}
		if (print_new_line)
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (1);
}
