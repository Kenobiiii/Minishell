/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:27:38 by anggalle          #+#    #+#             */
/*   Updated: 2025/05/27 17:18:02 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_shell_state;

int	write_heredoc_line(int pipefd, char *line)
{
	if (write(pipefd, line, ft_strlen(line)) == -1
		|| write(pipefd, "\n", 1) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (1);
}

t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*current;

	if (!node)
		return (NULL);
	current = node;
	while (current && (current->type == REDIRECT_OUT
			|| current->type == REDOUT2))
		current = current->left;
	return (current);
}

// Función auxiliar para contar redirecciones de salida en cadena
int	count_output_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
		return (1 + count_output_redirections(node->left));
	return (0);
}

// Función auxiliar para recolectar todas las redirecciones en orden inverso
void	collect_redirections(t_ast *node, t_ast **redirections, int *index)
{
	if (!node)
		return ;
	if (node->type == REDIRECT_OUT || node->type == REDOUT2)
	{
		collect_redirections(node->left, redirections, index);
		redirections[(*index)++] = node;
	}
}

// Función para procesar múltiples redirecciones de salida en orden correcto
void	process_multiple_output_redirections(t_data *data, t_ast *node)
{
	int		count;
	t_ast	**redirections;
	int		i;
	int		index;
	int		fd;

	count = count_output_redirections(node);
	if (count <= 1)
	{
		// Si solo hay una redirección, usar el comportamiento normal
		if (node->type == REDIRECT_OUT)
			exec_redirect_out(data, node);
		else if (node->type == REDOUT2)
			exec_redirect_append(data, node);
		return ;
	}

	// Recolectar todas las redirecciones
	redirections = malloc(sizeof(t_ast *) * count);
	if (!redirections)
		return ;
	
	index = 0;
	collect_redirections(node, redirections, &index);
	
	// Procesar todas las redirecciones en orden (primera a última)
	i = 0;
	while (i < count)
	{
		if (redirections[i]->right)
		{
			// Abrir el archivo
			if (redirections[i]->type == REDIRECT_OUT)
				fd = open(redirections[i]->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(redirections[i]->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			
			if (fd == -1)
			{
				perror("minishell");
				data->wstatus = 1;
				free(redirections);
				return ;
			}
			
			// Si no es la última redirección, cerrar inmediatamente
			if (i < count - 1)
			{
				close(fd);
			}
			else
			{
				// Es la última redirección, guardar para stdout
				if (data->output_redir_fd != -1)
					close(data->output_redir_fd);
				data->output_redir_fd = fd;
			}
		}
		i++;
	}
	
	free(redirections);
	
	// Encontrar y ejecutar el comando
	t_ast *cmd_node = node;
	while (cmd_node && (cmd_node->type == REDIRECT_OUT || cmd_node->type == REDOUT2))
		cmd_node = cmd_node->left;
	
	if (cmd_node)
		exec_ast(data, cmd_node);
}

void	exec_redirect_out(t_data *data, t_ast *node)
{
	char	*filename;
	int		fd;

	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return ;
	}
	
	// Verificar si hay múltiples redirecciones de salida
	if (count_output_redirections(node) > 1)
	{
		process_multiple_output_redirections(data, node);
		return ;
	}
	
	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	
	// Si había un descriptor anterior, cerrarlo inmediatamente
	// (bash trunca/crea todos los archivos pero solo el último recibe output)
	if (data->output_redir_fd != -1)
	{
		close(data->output_redir_fd);
	}
	
	// Establecer el nuevo descriptor como actual
	data->output_redir_fd = fd;
	
	// Ejecutar recursivamente el comando de la izquierda
	if (node->left)
		exec_ast(data, node->left);
}

void	exec_redirect_append(t_data *data, t_ast *node)
{
	char	*filename;
	int		fd;

	if (!node || !node->right)
	{
		ft_printf("No hay archivo de salida\n");
		data->wstatus = 2;
		return ;
	}
	
	// Verificar si hay múltiples redirecciones de salida
	if (count_output_redirections(node) > 1)
	{
		process_multiple_output_redirections(data, node);
		return ;
	}
	
	filename = node->right->value;
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	
	// Si había un descriptor anterior, cerrarlo inmediatamente
	// (bash trunca/crea todos los archivos pero solo el último recibe output)
	if (data->output_redir_fd != -1)
		close(data->output_redir_fd);
	
	// Establecer el nuevo descriptor como actual
	data->output_redir_fd = fd;
	
	// Ejecutar recursivamente el comando de la izquierda
	if (node->left)
		exec_ast(data, node->left);
}

void	exec_redirect_in(t_data *data, t_ast *node)
{
	int		fd;
	char	*filename;

	if (!node->right)
	{
		ft_printf("No hay archivo de entrada\n");
		data->wstatus = 2;
		return ;
	}
	
	filename = node->right->value;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		data->wstatus = 1;
		return ;
	}
	
	// Cerrar el descriptor anterior si existía
	if (data->input_redir_fd != -1)
		close(data->input_redir_fd);
	
	// Guardar el nuevo descriptor para el proceso hijo
	data->input_redir_fd = fd;
	
	// Ejecutar recursivamente el comando de la izquierda
	if (node->left)
		exec_ast(data, node->left);
}
