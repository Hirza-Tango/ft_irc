/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:40:02 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/13 11:40:26 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	client_write(t_env *e, int cs)
{
	int		w;
	char	buff[BUF_SIZE];

	ft_bzero(buff, BUF_SIZE);
	if ((w = cbuff_read(e->fds[cs].buf_write, buff)) <= 0)
		return ;
	if (send(cs, buff, w, 0) <= 0)
	{
		close(cs);
		FD_CLR(cs, &e->fd_write);
		clean_fd(&e->fds[cs]);
		cbuff_destroy(e->fds[cs].buf_read);
		cbuff_destroy(e->fds[cs].buf_write);
		ft_printf("client #%d gone away\n", cs);
	}
}
