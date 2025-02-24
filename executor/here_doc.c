/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/24 11:09:49 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    prompt_here_doc(const char *limiter, int *fd)
{
    char    *line;
    
    line = NULL;
    while (1337)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, limiter, ft_strlen(line)) == 0)
            break ;
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    free(line);
}

void    here_doc(t_chain *data)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("minishell");
        return ;
    }
    prompt_here_doc(data->delim, pipe_fd);
}
