/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:39:17 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/02 09:17:42 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	client_read(t_env *e, int cs)
{
	int		r;
	char	buff[BUF_SIZE];

	ft_bzero(buff, BUF_SIZE);
	r = recv(cs, buff, BUF_SIZE, 0);
	if (r <= 0)
	{
		close(cs);
		clean_fd(&e->fds[cs]);
		cbuff_destroy(e->fds[cs].buf_read);
		cbuff_destroy(e->fds[cs].buf_write);
		ft_printf("client #%d gone away\n", cs);
	}
	else
	{
		cbuff_write(e->fds[cs].buf_read, buff);
	}
}
