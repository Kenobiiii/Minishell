t_data	*init_data(char **env)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = env;
	data->path = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->ast = NULL;
	data->wstatus = 0;
	data->file_error = 0;
	data->only_redirections = 0;
	data->has_pipe = 0;
	return (data);
} 