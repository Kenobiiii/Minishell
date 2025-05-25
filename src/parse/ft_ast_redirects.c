/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:30:00 by paromero          #+#    #+#             */
/*   Updated: 2025/05/25 17:13:40 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


//? Verifica el acceso a archivos para operaciones de redirección
int	handle_file_access(t_data *data, char *filename, int type)
{
	int	fd;
	int	flags;

	if (type == REDIRECT_IN)
	{
		if (access(filename, R_OK) == -1)
		{
			data->wstatus = 1;
			if (data->only_redirections == 1)
				return (0);
		}
	}
	else if (type == REDIRECT_OUT || type == REDOUT2)
	{
		if (type == REDOUT2)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(filename, flags, 0644);
		if (fd == -1)
		{
			data->wstatus = 1;
			if (data->only_redirections == 1)
				return (0);
		}
		close(fd);
	}
	return (1);
}

//? Verifica la validez de los tokens de redirección y sus argumentos
int	handle_redirect_checks(t_data *data, t_tokens *tokens)
{
	char	*filename;

	if (tokens->type != REDIRECT_IN && tokens->type != REDIRECT_OUT
		&& tokens->type != REDIN2 && tokens->type != REDOUT2)
		return (1);
	filename = tokens->next->value;
	if (!filename || !*filename)
	{
		data->wstatus = 1;
		if (data->only_redirections == 1)
			return (0);
	}
	return (handle_file_access(data, filename, tokens->type));
}

//? Actualiza un nodo de redirección existente con un nuevo valor
int	handle_redirect_update(t_ast *current, t_tokens *tokens)
{
	if (current->right)
	{
		free(current->right->value);
		current->right->value = ft_strdup(tokens->value);
		if (!current->right->value)
			return (0);
	}
	else
	{
		current->right = ft_create_ast_node(CMD, tokens->value);
		if (!current->right || !current->right->value)
			return (0);
	}
	return (1);
}

//? Busca nodos de redirección del mismo tipo y los actualiza o crea nuevos
int	handle_redirect_node(t_ast **root,
	t_ast **last_op, t_tokens *tokens)
{
	t_ast	*current;
	int		found_same_redir;

	if (tokens->type == REDIRECT_OUT || tokens->type == REDOUT2)
		return (0);
	current = *root;
	found_same_redir = 0;
	while (current)
	{
		if (current->type == tokens->type)
		{
			found_same_redir = 1;
			if (!handle_redirect_update(current, tokens))
				return (0);
			*last_op = current;
			break ;
		}
		if (current->right && (current->type == REDIRECT_IN
				|| current->type == REDIRECT_OUT
				|| current->type == REDIN2
				|| current->type == REDOUT2))
			current = current->right;
		else
			break ;
	}
	return (found_same_redir);
}
