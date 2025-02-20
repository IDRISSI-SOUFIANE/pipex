/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:58:53 by sidrissi          #+#    #+#             */
/*   Updated: 2025/02/19 11:44:40 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_close(t_data *data)
{
	close(data->fds[0]);
	close(data->fds[1]);
}

void	ft_clean(t_data *data)
{
	if (data->path)
		ft_free(data->path, ft_strlen_p(data->path));
	close(data->fds[0]);
	close(data->fds[1]);
	exit(0);
}

void	child_process1(char **av, char **env, t_data *data)
{
	data->infile = open(av[1], O_RDONLY, 0777);
	if (data->infile < 0)
		(close(data->infile), ft_clean(data), perror("Error"));
	if (dup2(data->infile, STDIN_FILENO) < 0
		|| dup2(data->fds[1], STDOUT_FILENO) < 0)
		(ft_clean(data), perror("Error"));
	(close(data->infile), ft_close(data));
	data->cmd = ft_split(av[2], ' ');
	if (NULL == data->cmd)
		(perror("Error"), ft_clean(data));
	if (ft_find(data->cmd[0], "/."))
	{
		if (ft_ok(data->cmd) == 0)
			data->cmd_path = data->cmd[0];
		else
			(perror("Error"), ft_clean(data));
	}
	else
		data->cmd_path = get_path(data);
	if (execve(data->cmd_path, data->cmd, env) < 0)
	{
		(ft_putstr(data->cmd[0], ERROR), ft_clean(data),
			ft_free(data->cmd, ft_strlen_p(data->cmd)),
			free(data->cmd_path)), exit(1);
	}
}

void	child_process2(char **av, char **env, t_data *data)
{
	data->outfile = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->outfile < 0)
		(close(data->outfile), ft_clean(data), perror("Error"));
	if (dup2(data->outfile, STDOUT_FILENO) < 0
		|| dup2(data->fds[0], STDIN_FILENO) < 0)
		(ft_clean(data), perror("Error"));
	(close(data->outfile), ft_close(data));
	data->cmd = ft_split(av[3], ' ');
	if (NULL == data->cmd)
		(perror("Error"), ft_clean(data));
	if (ft_find(data->cmd[0], "/."))
	{
		if (ft_ok(data->cmd) == 0)
			data->cmd_path = data->cmd[0];
		else
			(perror("Error"), ft_clean(data));
	}
	else
		data->cmd_path = get_path(data);
	if (execve(data->cmd_path, data->cmd, env) < 0)
	{
		(ft_putstr(data->cmd[0], ERROR), ft_clean(data),
			ft_free(data->cmd, ft_strlen_p(data->cmd)),
			free(data->cmd_path)), exit(1);
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	ft_check(ac, av, env, &data);
	data.path = split_path(env);
	if (NULL == data.path)
		ft_free(data.path, ft_strlen_p(data.path));
	data.pid1 = fork();
	if (data.pid1 < 0)
		ft_clean(&data);
	if (data.pid1 == 0)
		child_process1(av, env, &data);
	data.pid2 = fork();
	if (data.pid2 < 0)
		ft_clean(&data);
	if (data.pid2 == 0)
		child_process2(av, env, &data);
	close(data.fds[0]);
	close(data.fds[1]);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	ft_clean(&data);
	return (0);
}
