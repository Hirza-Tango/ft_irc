/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 11:39:24 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 14:26:31 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	cmd_join(t_env *e, char *cmd)
{
	irc_write(e, "JOIN ");
	irc_write(e, cmd);
	irc_write(e, "\n");
}

void	cmd_nick(t_env *e, char *cmd)
{
	irc_write(e, "NICK ");
	irc_write(e, cmd);
	irc_write(e, "\n");
}

void	cmd_msg(t_env *e, char *cmd)
{
	irc_write(e, "PRIVMSG ");
	irc_write(e, cmd);
	irc_write(e, "\n");
}

void	cmd_who(t_env *e, char *cmd)
{
	irc_write(e, "NAMES ");
	if (cmd && *cmd)
		irc_write(e, cmd);
	irc_write(e, "\n");
}

void	cmd_leave(t_env *e, char *cmd)
{
	irc_write(e, "PART ");
	irc_write(e, cmd);
	irc_write(e, "\n");
}
