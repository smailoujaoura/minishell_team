/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 07:58:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    builtin_pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        return ;
    }
    printf("%s\n", path);
    free(path);
}
