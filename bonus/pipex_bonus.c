/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:26:51 by sidrissi          #+#    #+#             */
/*   Updated: 2025/02/20 21:56:24 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"


void close_all_pipes(t_data *data)
{
    int i = 0;
    if (data->n_pipes > 0) {
        while (i < data->n_pipes) {
            close(data->fds[i][0]);
            close(data->fds[i][1]);
            free(data->fds[i]);
            i++;
        }
        free(data->fds);
        data->fds = NULL;
    }
}


void execute_command(int i, char **av, char **env, t_data *data)
{
    data->cmd = ft_split(av[i + 2], ' ');
    if (!data->cmd || !data->cmd[0]) {
        ft_putstr(ERROR, "Invalid command\n");
        exit(1);
    }
    
    if (ft_find(data->cmd[0], "/")) {
        if (access(data->cmd[0], X_OK) == 0)
            execve(data->cmd[0], data->cmd, env);
        ft_putstr(data->cmd[0], ": Permission denied\n");
    } else {
        data->cmd_path = get_path(data);
        if (data->cmd_path) {
            execve(data->cmd_path, data->cmd, env);
            free(data->cmd_path);
        }
        ft_putstr(data->cmd[0], ": Command not found\n");
    }
    ft_free(data->cmd, ft_strlen_p(data->cmd));
    exit(1);
}

void read_writ_fd(int i, char **av, char **env, t_data *data)
{
    if (i == 0)
        dup2(data->infile, STDIN_FILENO);
    else
        dup2(data->fds[i-1][0], STDIN_FILENO);

    if (i == data->n_commands-1)
        dup2(data->outfile, STDOUT_FILENO);
    else
        dup2(data->fds[i][1], STDOUT_FILENO);

    close_all_pipes(data);
    close(data->infile);
    close(data->outfile);
    execute_command(i, av, env, data);
}

void execute_logic(int ac, char **av, char **env, t_data *data)
{
    (void)ac;
    int i = 0;
    int status;
    
    while (i < data->n_commands) {
        data->pid = fork();
        if (data->pid == 0)
            read_writ_fd(i, av, env, data);
        else if (data->pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        i++;
    }
    close_all_pipes(data);
    close(data->infile);
    close(data->outfile);
    while (wait(&status) > 0);
}
void create_pipes(t_data *data)
{
    int i;

    i = 0;
    data->n_pipes = data->n_commands - 1;
    if (data->n_pipes > 0)
    {
        data->fds = malloc(data->n_pipes * sizeof(int *));
        if (!data->fds) {
            perror("Malloc failed");
            exit(1);
        }
        while (i < data->n_pipes) {
            data->fds[i] = malloc(2 * sizeof(int));
            if (!data->fds[i] || pipe(data->fds[i]) < 0) {
                close_all_pipes(data);
                perror("Pipe failed");
                exit(1);
            }
            i++;
        }
    }
}

int main(int ac, char **av, char **env)
{
    t_data data = {0};
    
    if (ac < 4) {
        ft_putstr(ERROR, "Usage: ./pipex infile cmd1 cmd2... outfile\n");
        return 1;
    }
    
    data.infile = open(av[1], O_RDONLY);
    data.outfile = open(av[ac-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (data.infile < 0 || data.outfile < 0) {
        perror("Error");
        return 1;
    }
    
    // data.path = ft_split(ft_getenv("PATH"), ':');
    data.path = split_path(env);
    if (!data.path) {
        ft_putstr(ERROR, "PATH not found\n");
        return 1;
    }
    
    data.n_commands = ac - 3;
    create_pipes(&data);
    execute_logic(ac, av, env, &data);
    
    // close_all_pipes(&data); 
    ft_free(data.path, ft_strlen_p(data.path));
    return 0;
}
