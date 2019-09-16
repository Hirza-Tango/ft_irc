/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 09:46:00 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 15:31:10 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	client_connect(t_env *e)
{
	struct addrinfo	*tmp;
	struct addrinfo	*info;

	e->hint.ai_family = AF_UNSPEC;
	e->hint.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(e->host, e->port, &e->hint, &info))
		return ((void)ft_printf_fd(2, "Could not find host\n"));
	tmp = info;
	while (tmp)
	{
		if ((e->socket_fd =
			socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol)) != -1)
		{
			if (connect(e->socket_fd, tmp->ai_addr, tmp->ai_addrlen) != -1)
				break ;
			else
				close(e->socket_fd);
		}
		tmp = tmp->ai_next;
	}
	freeaddrinfo(info);
	if (!tmp)
		return ((void)ft_printf_fd(2, "Could not connect to host\n"));
	FD_SET(e->socket_fd, &e->sock_set);
}

void	init_env(t_env *e, fd_set *set)
{
	ft_bzero(e, sizeof(t_env));
	e->socket_fd = -1;
	FD_ZERO(&e->sock_set);
	FD_ZERO(&set);
	FD_SET(0, &e->sock_set);
}

void	handle_cmd(t_env *e, char *cmd_buff)
{
	char *arg;

	if (!cmd_buff || *cmd_buff != '/')
		return ;
	arg = get_arg(&cmd_buff);
	if (e->socket_fd == -1 && ft_strcmp(arg, "/connect"))
		return ((void)ft_putendl("Please connect to a server first"));
	else if (!ft_strcmp(arg, "/connect"))
		return (cmd_connect(e, cmd_buff));
	else if (!ft_strcmp(arg, "/join"))
		irc_write(e, "JOIN ");
	else if (!ft_strcmp(arg, "/nick"))
		irc_write(e, "NICK ");
	else if (!ft_strcmp(arg, "/message"))
		irc_write(e, "PRIVMSG ");
	else if (!ft_strcmp(arg, "/who"))
		irc_write(e, "NAMES ");
	else
		return (ft_putendl("Invalid command"));
	if (cmd_buff && *cmd_buff)
		irc_write(e, cmd_buff);
	irc_write(e, "\n");
}

int		main(int argc, char *argv[])
{
	t_env			e;
	char			*cmd_buff;
	fd_set			set;
	struct timeval	timeout;

	init_env(&e, &set);
	if (argc == 3)
	{
		e.host = ft_strdup(argv[1]);
		e.port = ft_strdup(argv[2]);
		client_connect(&e);
	}
	while (1)
	{
		timeout.tv_sec = 1;
		FD_COPY(&e.sock_set, &set);
		select(e.socket_fd == -1 ? 1 : e.socket_fd + 1,
			&set, NULL, NULL, &timeout);
		if (FD_ISSET(STDIN_FILENO, &set))
			if (get_next_line(STDIN_FILENO, &cmd_buff) > 0)
				handle_cmd(&e, cmd_buff);
		if (e.socket_fd > 0 && FD_ISSET(e.socket_fd, &set))
			irc_read(&e);
	}
	return (0);
}
