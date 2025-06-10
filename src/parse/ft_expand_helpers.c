/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paromero <paromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:15:00 by paromero          #+#    #+#             */
/*   Updated: 2025/06/10 18:09:25 by paromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_expand_ctx	init_expand_context(t_data *data, char *line, char *result)
{
	t_expand_ctx	ctx;
	static int		i;
	static int		j;
	static char		in_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	ctx.data = data;
	ctx.line = line;
	ctx.result = result;
	ctx.i = &i;
	ctx.j = &j;
	ctx.in_quote = &in_quote;
	return (ctx);
}

int	should_expand_variable(char *line, int i, char in_quote)
{
	return (line[i] == '$' && in_quote != '\''
		&& (ft_isalnum(line[i + 1]) || line[i + 1] == '?'));
}
