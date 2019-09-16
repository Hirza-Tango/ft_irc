/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 15:00:29 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 16:04:51 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

int				is_valid_nick(char *nick)
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
	if (i == 9 && nick[i])
		return (0);
	return (1);
}

int				is_valid_chan(char *chan)
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
	if (i == 200)
		return (0);
	return (1);
}

void			cmd_nick(t_env *e, size_t i, char *cmd)
{
	char	*nick;
	size_t	j;

	nick = get_arg(&cmd);
	if (!*nick)
		REPL_ERR(ERR_NONICKNAMEGIVEN, "", "No nickname given");
	if (!is_valid_nick(nick))
		REPL_ERR(ERR_ERRONEUSNICKNAME, nick, "Erroneus nickname");
	j = -1;
	while (++j < e->maxfd)
		if (e->fds[j].type == FD_CLIENT && !ft_strcmp(nick, e->fds[j].nick))
			REPL_ERR(ERR_NICKNAMEINUSE, nick, "Nickname is already in use");
	ft_strcpy(e->fds[i].nick, nick);
	reply(I_WRITE, RPL_NONE, NULL, NULL);
}

static size_t	add_name(t_env *e, size_t i, char **buff)
{
	char	new_buff[4096];
	size_t	start;

	if ((*buff)[0] != ':')
	{
		new_buff[0] = ':';
		ft_strcpy(new_buff + 1, e->fds[i].nick);
		ft_strcat(new_buff + 1, " ");
		ft_strlcat(new_buff, *buff, 4096);
		ft_strcpy(*buff, new_buff);
	}
	start = ft_strlen(e->fds[i].nick) + 1;
	while (ft_isspace((*buff)[start]))
		start++;
	return (start);
}

void			handle_cmd(t_env *e)
{
	char	buff[4096];
	size_t	cmd_start;
	char	*tmp;
	size_t	i;

	i = -1;
	while (++i < e->maxfd)
		if (e->fds[i].type == FD_CLIENT && cbuff_read(I_READ, buff) > 0)
		{
			ft_printf("Read: %s from %d\n", buff, i);
			tmp = buff;
			cmd_start = add_name(e, i, &tmp);
			if (!ft_strncmp(*&buff + cmd_start, "NICK", 4))
				cmd_nick(e, i, tmp + cmd_start + 4);
			else if (!ft_strncmp(tmp + cmd_start, "PRIVMSG", 7))
				cmd_privmsg(e, i, tmp);
			else if (!ft_strncmp(tmp + cmd_start, "JOIN", 4))
				cmd_join(e, i, tmp + cmd_start + 4);
			else if (!ft_strncmp(tmp + cmd_start, "PART", 4))
				cmd_part(e, i, tmp + cmd_start + 4);
			else if (!ft_strncmp(tmp + cmd_start, "NAMES", 5))
				cmd_names(e, i, tmp + cmd_start + 5);
			else
				reply(I_WRITE, ERR_UNKNOWNCOMMAND, "", "Unknown command");
		}
}
