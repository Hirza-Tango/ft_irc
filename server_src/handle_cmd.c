/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 15:00:29 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/11 15:51:17 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	reply(t_cbuff write_buffer, char *code, char *target, char *value)
{
	cbuff_write(write_buffer, code);
	if (target && *target)
	{
		cbuff_write(write_buffer, " ");
		cbuff_write(write_buffer, target);
	}
	if (value && *value)
	{
		cbuff_write(write_buffer, " :");
		cbuff_write(write_buffer, value);
	}
	cbuff_write(write_buffer, "\n");
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
	if (i == 9 && nick[i])
		return (0);
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
	if (i == 200)
		return (0);
	return (1);
}

void	cmd_nick(t_env *e, size_t i, char *cmd)
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

void	add_name(t_env *e, size_t i, char **buff)
{
	char	new_buff[4096];

	if ((*buff)[0] == ':')
		return ;
	new_buff[0] = ':';
	ft_strcpy(new_buff + 1, e->fds[i].nick);
	ft_strcat(new_buff + 1, " ");
	ft_strlcat(new_buff, *buff, 4096);
	ft_strcpy(*buff, new_buff);
}

void	handle_cmd(t_env *e)
{
	char	buff[4096];
	char	*tmp;
	char	*cmd;
	size_t	i;

	i = -1;
	while (++i < e->maxfd)
		if (e->fds[i].type == FD_CLIENT && cbuff_read(I_READ, buff) > 0)
		{
			ft_printf("Read: %s from %d\n", buff, i);
			tmp = buff;
			add_name(e, i, &tmp);
			cmd = get_arg(&tmp);
			if (cmd && *cmd == ':')
				cmd = get_arg(&tmp);
			if (!ft_strcmp(cmd, "NICK"))
				cmd_nick(e, i, tmp);
			else if (!ft_strcmp(cmd, "PRIVMSG"))
				cmd_privmsg(e, i, buff);
			else if (!ft_strcmp(cmd, "JOIN"))
				cmd_join(e, i, tmp);
			else if (!ft_strcmp(cmd, "PART"))
				cmd_part(e, i, tmp);
			else if (!ft_strcmp(cmd, "NAMES"))
				cmd_names(e, i, tmp);
			else
				reply(I_WRITE, ERR_UNKNOWNCOMMAND, cmd, "Unknown command");
		}
}
