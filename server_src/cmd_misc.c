/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_misc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 16:01:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 16:54:28 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	print_members_chan(t_env *e, size_t i, t_chan *chan)
{
	t_list	*current;

	current = chan->users;
	while (current)
	{
		if (e->fds[*(size_t *)current->content].type != FD_CLIENT)
			del_from_chan(e, *(size_t *)current->content, chan);
		current = current->next;
	}
	if (!chan->users)
		return ;
	cbuff_write(I_WRITE, RPL_NAMREPLY);
	cbuff_write(I_WRITE, " ");
	cbuff_write(I_WRITE, chan->name);
	cbuff_write(I_WRITE, " :");
	current = chan->users;
	while (current)
	{
		cbuff_write(I_WRITE, e->fds[*(size_t *)current->content].nick);
		if ((current = current->next))
			cbuff_write(I_WRITE, " ");
		else
			cbuff_write(I_WRITE, "\n");
	}
}

void		cmd_names(t_env *e, size_t i, char *cmd)
{
	t_list	*current;
	char	*name;

	current = e->channels;
	if (!*(name = get_arg(&cmd)))
	{
		while (current)
		{
			print_members_chan(e, i, current->content);
			current = current->next;
		}
		REPL_ERR(RPL_ENDOFNAMES, NULL, "End of /NAMES list");
	}
	if (!is_valid_chan(name))
		REPL_ERR(RPL_ENDOFNAMES, name, "End of /NAMES list");
	while (current)
	{
		if (!ft_strcmp(((t_chan *)(current->content))->name, name))
			break ;
		current = current->next;
	}
	if (!current)
		REPL_ERR(RPL_ENDOFNAMES, name, "End of /NAMES list");
	print_members_chan(e, i, current->content);
	REPL_ERR(RPL_ENDOFNAMES, name, "End of /NAMES list");
}

void		reply(t_cbuff write_buffer, char *code,
	char *target, char *value)
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

char		*get_arg(char **cmd)
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
