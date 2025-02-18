/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/18 09:30:45 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    prompt_here_doc(const char *limiter, int fd)
{
    char    *line;
    
    line = NULL;
    while (1337)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, limiter, ft_strlen(line)) == 0)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    free(line);
}

void    here_doc(t_chain *data)
{
    int fd;
    char buff[1000000];

    fd = open(".here_doc", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    prompt_here_doc(data->argv->content, fd);
    fd = open(".here_doc", O_RDONLY, 0644);
    read(fd, buff, 1337);
    printf("%s", buff);
    if (access(".here_doc", F_OK) == 0)
        unlink(".here_doc");
}
