/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trus <trus@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:00:00 by hrrokaj           #+#    #+#             */
/*   Updated: 2026/04/27 14:29:48 by trus             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	errmsg(char *prefix, char *name, char *suffix)
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(suffix, 2);
}

int	execve_errno(char *path)
{
	struct stat	st;
	int			err;

	err = errno;
	if (err == EACCES && stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		err = EISDIR;
	return (err);
}
