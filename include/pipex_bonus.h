/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:30:20 by sidrissi          #+#    #+#             */
/*   Updated: 2025/02/20 20:37:35 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define ERROR "zsh: command not found:"

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>


typedef struct s_data {
    int     **fds;
    int     n_commands;
    int     n_pipes;
    int     infile;
    int     outfile;
    char    **path;
    char    **cmd;
    char    *cmd_path;
    pid_t   pid;
} t_data;



char			**ft_split(char const *s, char sep);
char			**ft_free(char **arr, size_t n);

void			ft_close(t_data *data);
void			ft_clean(t_data *data);
void			child_process1(char **av, char **env, t_data *data);
void			child_process2(char **av, char **env, t_data *data);
void			ft_check(int ac, char **av, char **env, t_data *data);

int				ft_strlen_p(char **s);
int				ft_strncmp(char *s1, char *s2, int n);
int				ft_find(char *s, char *sep);
int				ft_strlen(char *s);
char			*ft_strcpy(char *dst, char *s);
char			*ft_strcat(char *dst, char *s);
char			*ft_strdup(char *s);
char			*ft_strjoin(char *s1, char *s2);
void			ft_putstr(char *cmd_path, char *error);

char			**split_path(char **env);
char			*get_path(t_data *data);
int				ft_ok(char **cmd);

/*
*/


void close_all_pipes(t_data *data);
void create_pipes(t_data *data);
void execute_command(int i, char **av, char **env, t_data *data);
void read_writ_fd(int i, char **av, char **env, t_data *data);
void execute_logic(int ac, char **av, char **env, t_data *data);

#endif