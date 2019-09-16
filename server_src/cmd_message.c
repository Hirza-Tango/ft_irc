/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 11:27:33 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 16:24:02 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	msg_nick(t_env *e, size_t i, char *cmd, char *nick)
{
	size_t	j;

	if (!is_valid_nick(nick))
		REPL_ERR(ERR_NOSUCHNICK, nick, "No such nick");
	j = -1;
	while (++j < e->maxfd)
		if (e->fds[j].type == FD_CLIENT && !ft_strcmp(e->fds[j].nick, nick))
		{
			cbuff_write(e->fds[j].buf_write, cmd);
			return ;
		}
	REPL_ERR(ERR_NOSUCHNICK, nick, "No such nick");
}

static void	msg_chan(t_env *e, size_t i, char *cmd, char *chan_name)
{
	t_list	*current;

	if (!is_valid_chan(chan_name))
		REPL_ERR(ERR_NOSUCHCHANNEL, chan_name, "No such channel");
	current = e->channels;
	while (current)
	{
		if (!ft_strcmp(((t_chan *)(current->content))->name, chan_name))
			break ;
		current = current->next;
	}
	if (!current)
		REPL_ERR(ERR_NOSUCHCHANNEL, chan_name, "No such channel");
	current = ((t_chan *)(current->content))->users;
	while (current)
	{
		cbuff_write(e->fds[*(size_t *)(current->content)].buf_write, cmd);
		current = current->next;
	}
	REPL_ERR(RPL_NONE, NULL, NULL);
}

void		cmd_privmsg(t_env *e, size_t i, char *cmd)
{
	char		*tmp;
	char		*nick;

	tmp = cmd;
	nick = get_arg(&tmp);
	if (*nick == ':')
		nick = get_arg(&tmp);
	nick = get_arg(&tmp);
	if (!e->fds[i].nick[0])
		REPL_ERR(ERR_NOTREGISTERED, "", "Not registered");
	if (!*nick)
		REPL_ERR(ERR_NORECIPIENT, NULL, "No recipient given");
	if (!*tmp)
		REPL_ERR(ERR_NOTEXTTOSEND, NULL, "No text to send");
	if (*nick == '&')
		msg_chan(e, i, cmd, nick);
	else
	{
		msg_nick(e, i, cmd, nick);
		REPL_ERR(RPL_NONE, NULL, NULL);
	}
}
