/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 15:00:29 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/10 13:44:21 by dslogrov         ###   ########.fr       */
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

int		is_valid_nick(char *nick)
{
	size_t	i;

	i = 0;
	if (!nick || !ft_isalpha(nick[0]))
		return (0);
	while (i < 9 && nick[i] && !ft_isspace(nick[i]))
	{
		if (!ft_isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' &&
			nick[i] != ']' && nick[i] != '\\' && nick[i] != '`' &&
			nick[i] != '^' && nick[i] != '{' && nick[i] != '}')
			return (0);
		i++;
	}
	return (1);
}

int		is_valid_chan(char *chan)
{
	size_t	i;

	if (!chan || *chan != '&')
		return (0);
	i = 0;
	while (i < 200 && chan[i] && !ft_isspace(chan[i]))
	{
		if (chan[i] == 7 || chan[i] == ',')
			return (0);
		i++;
	}
	return (1);
}

void	cmd_nick(t_env *e, size_t i, char *cmd)
{
	size_t	j;
	char	nick[10];

	j = 0;
	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	if (!*cmd)
		return (reply(I_WRITE, ERR_NONICKNAMEGIVEN, "No nick given"));
	if (!is_valid_nick(cmd))
		return (reply(I_WRITE, ERR_ERRONEUSNICKNAME, "Bad nick"));
	while (j < 9 && *cmd && !ft_isspace(*cmd))
	{
		nick[j] = *(cmd++);
		j++;
	}
	nick[j] = 0;
	j = -1;
	while (++j < e->maxfd)
		if (e->fds[j].type == FD_CLIENT && !ft_strcmp(nick, e->fds[j].nick))
			return (reply(I_WRITE, ERR_NICKNAMEINUSE, "Nick in use"));
	ft_strcpy(e->fds[i].nick, nick);
	reply(I_WRITE, RPL_NONE, NULL);
}

void	handle_cmd(t_env *e)
{
	char	buff[BUF_SIZE];
	size_t	i;

	i = -1;
	while (++i < e->maxfd)
		if (e->fds[i].type == FD_CLIENT &&
			cbuff_read(I_READ, buff) > 0)
		{
			ft_printf("Read: %s from %d\n", buff, i);
			if (!ft_strncmp(buff, "NICK", 4))
				cmd_nick(e, i, buff + 4);
			else if (!ft_strncmp(buff, "PRIVMSG", 7))
				cmd_privmsg(e, i, buff + 7);
			else if (!ft_strncmp(buff, "JOIN", 4))
				cmd_join(e, i, buff + 4);
			else if (!ft_strncmp(buff, "LEAVE", 5))
				cmd_leave(e, i, buff + 5);
			else if (!ft_strncmp(buff, "WHO", 3))
				cmd_names(e, i, buff + 8);
			else
				return (reply(I_WRITE, ERR_UNKNOWNCOMMAND, "Unknown command"));
		}
}