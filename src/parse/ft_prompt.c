/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:42:01 by paromero          #+#    #+#             */
/*   Updated: 2025/04/08 18:42:33 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_last_component(char *path)
{
	char	*last_slash;

	last_slash = ft_strrchr(path, '/');
	if (last_slash)
		return (last_slash + 1);
	else
		return (path);
}

void	update_prompt(t_data *data)
{
	char	*new_prompt;
	size_t	prompt_size;
	char	*last_component;

	last_component = get_last_component(data->cwd);
	prompt_size = ft_strlen("$Minishell()> ")
		+ ft_strlen(last_component)
		+ 40;
	new_prompt = malloc(prompt_size);
	if (!new_prompt)
		return ;
	new_prompt[0] = '\0';
	ft_strlcat(new_prompt, ANSI_COLOR_GREEN, prompt_size);
	ft_strlcat(new_prompt, "$Minishell", prompt_size);
	ft_strlcat(new_prompt, ANSI_COLOR_BLUE, prompt_size);
	ft_strlcat(new_prompt, "(", prompt_size);
	ft_strlcat(new_prompt, ANSI_COLOR_RED, prompt_size);
	ft_strlcat(new_prompt, last_component, prompt_size);
	ft_strlcat(new_prompt, ANSI_COLOR_BLUE, prompt_size);
	ft_strlcat(new_prompt, ")", prompt_size);
	ft_strlcat(new_prompt, ANSI_COLOR_GREEN, prompt_size);
	ft_strlcat(new_prompt, "> ", prompt_size);
	ft_strlcat(new_prompt, ANSI_COLOR_RESET, prompt_size);
	data->prompt = new_prompt;
}

int	update_pwd(t_data	*data)
{
	char	*new_pwd;
	char	cwd[PATH_MAX];
	t_env	*tmp;

	new_pwd = getcwd(cwd, sizeof(cwd));
	tmp = data->env;
	while (ft_strncmp(tmp->value, "PWD=", 4) != 0)
		tmp = tmp->next;
	if (tmp)
	{
		free (tmp->value);
		tmp->value = ft_strcat("PWD=", new_pwd);
	}
	free (data->cwd);
	data->cwd = ft_strdup(new_pwd);
	update_prompt(data);
	return (1);
}
