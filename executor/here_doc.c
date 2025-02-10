/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/10 22:15:44 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    here_doc(const char *limiter)
{
    char    *line;
    int     fd;
    char    buff[10000];

    line = NULL;
    fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    while (1337)
    {
        line = readline("> ");
        if (ft_strncmp(line, limiter, ft_strlen(line)) == 0)
            break ;
        write(fd, line, ft_strlen(line));
        free(line);
    }
    free(line);
    fd = open(".here_doc", O_RDONLY, 0644);
    read(fd, buff, 1337);
    printf("%s\n", buff);
    if (access(".here_doc", F_OK) == 0)
        unlink(".here_doc");
}
