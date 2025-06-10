/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:42:01 by paromero          #+#    #+#             */
/*   Updated: 2025/06/03 18:34:32 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_last_component(char *path)
{
	char	*last_slash;

	if (!path)
		return ("unknown");
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

	if (!data || !data->cwd)
		return ;
	last_component = get_last_component(data->cwd);
	prompt_size = ft_strlen("$Minishell()> ")
		+ ft_strlen(last_component) + 52;
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
	if (data->prompt)
		free(data->prompt);
	data->prompt = new_prompt;
}

int	update_pwd(t_data	*data)
{
	char	*new_pwd;
	char	cwd[PATH_MAX];
	t_env	*tmp;
	char	*pwd_value;

	new_pwd = getcwd(cwd, sizeof(cwd));
	if (!new_pwd)
		new_pwd = "Deleted cd";
	tmp = data->env;
	while (tmp && ft_strncmp(tmp->value, "PWD=", 4) != 0)
		tmp = tmp->next;
	if (tmp)
	{
		free (tmp->value);
		pwd_value = ft_strcat("PWD=", new_pwd);
		tmp->value = pwd_value;
	}
	free (data->cwd);
	data->cwd = ft_strdup(new_pwd);
	update_prompt(data);
	return (1);
}
