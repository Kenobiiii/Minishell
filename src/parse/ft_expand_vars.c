/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:00:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 18:16:31 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_variable_expansion(t_expand_ctx *ctx)
{
	char	*expanded_var;
	char	*var_value;
	int		value_len;

	if (ctx->line[*ctx->i + 1] == '?')
	{
		handle_exit_status(ctx->data, ctx->result, ctx->j);
		*ctx->i += 2;
	}
	else
	{
		expanded_var = extract_var(ctx->line, ctx->i);
		if (expanded_var)
		{
			var_value = find_envvar(ctx->data, expanded_var);
			if (var_value && ft_strlen(var_value) > 0)
			{
				value_len = ft_strlen(var_value);
				ft_strlcpy(ctx->result + *ctx->j, var_value, value_len + 1);
				*ctx->j += value_len;
				free(var_value);
			}
			free(expanded_var);
		}
	}
}

static int	handle_quote_char(t_expand_ctx *ctx)
{
	if ((ctx->line[*ctx->i] == '\'' || ctx->line[*ctx->i] == '"')
		&& !*ctx->in_quote)
	{
		*ctx->in_quote = ctx->line[*ctx->i];
		ctx->result[(*ctx->j)++] = ctx->line[(*ctx->i)++];
		return (1);
	}
	else if (ctx->line[*ctx->i] == *ctx->in_quote && *ctx->in_quote)
	{
		*ctx->in_quote = 0;
		ctx->result[(*ctx->j)++] = ctx->line[(*ctx->i)++];
		return (1);
	}
	return (0);
}

static void	process_character(t_expand_ctx *ctx)
{
	if (handle_quote_char(ctx))
		return ;
	else if (should_expand_variable(ctx->line, *ctx->i, *ctx->in_quote))
		handle_variable_expansion(ctx);
	else
		ctx->result[(*ctx->j)++] = ctx->line[(*ctx->i)++];
}

char	*expand_variables_outside_quotes(t_data *data, char *line)
{
	int					len;
	char				*result;
	t_expand_ctx		ctx;

	if (!line || !data)
		return (ft_strdup(line));
	len = calculate_expanded_size(data, line) + ft_strlen(line);
	result = malloc(len);
	if (!result)
		return (ft_strdup(line));
	ctx = init_expand_context(data, line, result);
	while (line[*ctx.i])
		process_character(&ctx);
	result[*ctx.j] = '\0';
	return (result);
}
