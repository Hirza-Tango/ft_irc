/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 13:29:16 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 14:33:13 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	irc_read(t_env *e)
{
	ssize_t			readlen;
	char			user[256];
	char			dest[256];
	char			*buff;

	if (e->socket_fd == -1)
		return ;
	if ((readlen = recv(e->socket_fd, e->buff, READ_BUFF_SIZE, 0)) <= 0)
	{
		close(e->socket_fd);
		FD_CLR(e->socket_fd, &e->sock_set);
		e->socket_fd = -1;
		return ((void)ft_printf_fd(2, "Disconnected from server\n"));
	}
	else if (e->buff[0] == ':')
	{
		buff = e->buff + 1;
		ft_strcpy(user, get_arg(&buff));
		get_arg(&buff);
		ft_strcpy(dest, get_arg(&buff));
		return ((void)ft_printf("%s -> %s: %s", user, dest, buff));
	}
	ft_putstr("Response from server: ");
	write(1, e->buff, readlen);
}

void	irc_write(t_env *e, char *cmd)
{
	if (e->socket_fd == -1)
		return ((void)ft_printf_fd(2, "Please connect to the server\n"));
	if ((send(e->socket_fd, cmd, ft_strlen(cmd), 0)) <= 0)
	{
		close(e->socket_fd);
		FD_CLR(e->socket_fd, &e->sock_set);
		ft_printf_fd(2, "Disconnected from server\n");
		e->socket_fd = -1;
	}
}

char	*get_arg(char **cmd)
{
	static char arg[256];
	size_t		i;

	if (!cmd || !*cmd)
		return ("");
	while (ft_isspace(**cmd))
		(*cmd)++;
	i = 0;
	while (i < 256 && **cmd && !ft_isspace(**cmd))
		arg[i++] = *((*cmd)++);
	arg[i] = 0;
	while (ft_isspace(**cmd))
		(*cmd)++;
	return (arg);
}

void	cmd_connect(t_env *e, char *cmd)
{
	if (e->host)
		free(e->host);
	if (e->port)
		free(e->port);
	e->host = ft_strdup(get_arg(&cmd));
	e->port = ft_strdup(get_arg(&cmd));
	client_connect(e);
}
