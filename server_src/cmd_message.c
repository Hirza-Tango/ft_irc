/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 11:27:33 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/10 13:02:15 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	msg_nick(t_env *e, size_t i, char *cmd, char *nick)
{
	size_t	j;

	if (!is_valid_nick(nick))
		return (reply(I_WRITE, ERR_NOSUCHNICK, "No such nick"));
	j = -1;
	while (++j < e->maxfd)
		if (e->fds[j].type == FD_CLIENT && !ft_strcmp(e->fds[j].nick, nick))
		{
			cbuff_write(e->fds[j].buf_write, cmd);
			return (reply(I_WRITE, RPL_NONE, NULL));
		}
	return (reply(I_WRITE, ERR_NOSUCHNICK, "No such nick"));
}

static void	msg_chan(t_env *e, size_t i, char *cmd, char *nick)
{
	(void)(e && i && cmd && nick);
}

void		cmd_privmsg(t_env *e, size_t i, char *cmd)
{
	size_t		j;
	char		nick[10];

	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	if (!e->fds[i].nick[0])
		return (reply(I_WRITE, ERR_NOTREGISTERED, "Please register"));
	j = 0;
	while (j < 9 && *cmd && !ft_isspace(*cmd))
	{
		nick[j++] = *(cmd++);
	}
	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	nick[j] = 0;
	if (!j)
		return (reply(I_WRITE, ERR_NORECIPIENT, "No recipient specified"));
	if (!*cmd)
		return (reply(I_WRITE, ERR_NOTEXTTOSEND, "No text to send"));
	if (nick[0] == '&')
		msg_chan(e, i, cmd, nick);
	else
		msg_nick(e, i, cmd, nick);
}
