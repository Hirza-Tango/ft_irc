/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:39:17 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 13:45:05 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	client_read(t_env *e, int cs)
{
	int	r;
	int	i;
	char	buff[BUF_SIZE];

	r = recv(cs, buff, BUF_SIZE, 0);
	if (r <= 0)
	{
		close(cs);
		clean_fd(&e->fds[cs]);
		cbuff_destroy(e->fds[cs].buf_read);
		cbuff_destroy(e->fds[cs].buf_write);
		printf("client #%d gone away\n", cs);
	}
	else
	{
		i = 0;
		cbuff_write(e->fds[cs].buf_read, buff);
		while (i < e->maxfd)
		{
			//When a "CRLF" is found:
				//Add command to a ring buffer
			if ((e->fds[i].type == FD_CLIENT) &&
					(i != cs))
			{
				//cbuff_write(e->fds[i].buf_read, buff);
				send(i, buff, r, 0);
			}
			i++;
		}
	}
}
