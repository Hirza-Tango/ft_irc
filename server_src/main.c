/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:44:50 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/13 11:57:07 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	init_fd(t_env *e)
{
	size_t	i;

	i = 0;
	e->max = 0;
	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);
	while (i < e->maxfd)
	{
		if (e->fds[i].type != FD_FREE)
		{
			FD_SET(i, &e->fd_read);
			if (e->fds[i].type == FD_CLIENT &&
				cbuff_size(e->fds[i].buf_write) > 0)
			{
				FD_SET(i, &e->fd_write);
			}
			e->max = MAX(e->max, i);
		}
		i++;
	}
}

static void	check_fd(t_env *e)
{
	size_t	i;

	i = 0;
	while ((i < e->maxfd) && (e->r > 0))
	{
		if (e->fds[i].type == FD_FREE)
		{
			i++;
			continue ;
		}
		if (FD_ISSET(i, &e->fd_read))
			e->fds[i].fct_read(e, i);
		if (FD_ISSET(i, &e->fd_write))
			e->fds[i].fct_write(e, i);
		if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
			e->r--;
		i++;
	}
}

static void	init_env(t_env *e)
{
	size_t			i;
	struct rlimit	rlp;

	x_int(-1, getrlimit(RLIMIT_NOFILE, &rlp), "getrlimit");
	e->maxfd = rlp.rlim_cur;
	e->fds = (t_fd*)x_void(NULL, malloc(sizeof(*e->fds) * e->maxfd), "malloc");
	i = 0;
	while (i < e->maxfd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
	e->channels = NULL;
}

int			main(int ac, char **av)
{
	t_env			e;
	struct timeval	timeout;

	init_env(&e);
	if (ac != 2)
	{
		ft_printf_fd(2, USAGE, av[0]);
		exit(1);
	}
	e.port = ft_atoi(av[1]);
	srv_create(&e, e.port);
	while (1)
	{
		init_fd(&e);
		timeout.tv_sec = 1;
		e.r = select(e.max + 1, &e.fd_read, &e.fd_write, NULL, &timeout);
		check_fd(&e);
		handle_cmd(&e);
	}
	return (0);
}
