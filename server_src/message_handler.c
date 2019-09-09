/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 15:00:29 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/09 16:38:47 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	reply(t_cbuff write_buffer, int code, char *value)
{
	const char	*code_str = ft_itoa(code);

	cbuff_write(write_buffer, (char *)code_str);
	cbuff_write(write_buffer, " ");
	cbuff_write(write_buffer, value);
	cbuff_write(write_buffer, "\n");
	free((char *)code_str);
}

void	cmd_privmsg(t_env *e, size_t i, char *cmd)
{
	size_t		j;
	char		nick[10];

	if (!e->fds[i].nick[0])
		return (reply(e->fds[i].buf_write, ERR_NOTREGISTERED,
			"Please register with a nick"));
	j = 0;
	//TODO: proper check for valid nick
	while (j < 9 && *cmd && !ft_isspace(*cmd))
	{
		nick[j] = *(cmd++);
		j++;
	}
	while (ft_isspace(*(++cmd)))
		PASS;
	nick[j] = 0;
	if (nick[0] == '&')
	{
		//handle channels
	}
	else
	{
		j = -1;
		while (++j < e->maxfd)
			if (e->fds[j].type == FD_CLIENT && !ft_strcmp(e->fds[j].nick, nick))
			{
				cbuff_write(e->fds[j].buf_write, cmd);
				return (reply(e->fds[i].buf_write, RPL_NONE, NULL));
			}
		return (reply(e->fds[i].buf_write, ERR_NOSUCHNICK, "No such nick"));
	}
}

void	cmd_nick(t_env *e, size_t i, char *cmd)
{
	size_t	j;

	j = 0;
	//TODO: proper check for valid nick
	while (j < 9 && *cmd && !ft_isspace(*cmd))
	{
		e->fds[i].nick[j] = *(cmd++);
		j++;
	}
	e->fds[i].nick[j] = 0;
	reply(e->fds[i].buf_write, RPL_NONE, NULL);
}

void	message_handler(t_env *e)
{
	char	buff[BUF_SIZE];
	size_t	i;

	i = -1;
	while (++i < e->maxfd)
	{
		if (e->fds[i].type == FD_CLIENT &&
			cbuff_read(e->fds[i].buf_read, buff) > 0)
		{
			ft_printf("Read: %s from %d\n", buff, i);
			if (!ft_strncmp(buff, "NICK ", 5))
				cmd_nick(e, i, buff + 5);
			else if (!ft_strncmp(buff, "PRIVMSG ", 8))
				cmd_privmsg(e, i, buff + 8);
			else
				return (reply(e->fds[i].buf_write, ERR_UNKNOWNCOMMAND,
					"Unknown command"));
		}
	}
}
