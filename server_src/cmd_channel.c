/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_channel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 11:28:04 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/10 13:45:22 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	add_to_chan(t_env *e, size_t i, t_chan *chan)
{
	ft_lstadd(&chan->users, ft_lstnew(ft_strdup(e->fds[i].nick), 10));
	reply(I_WRITE, RPL_NOTOPIC, "Topics are not currently supported");
	cmd_names(e, i, chan->name);
}

void		cmd_join(t_env *e, size_t i, char *cmd)
{
	t_list	*current;
	char	name[201];
	size_t	j;

	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	if (!*cmd)
		return (reply(I_WRITE, ERR_NEEDMOREPARAMS, "No channel name given"));
	if (!is_valid_chan(cmd))
		return (reply(I_WRITE, ERR_NOSUCHCHANNEL, "Bad channel name"));
	j = 0;
	while (*cmd && !ft_isspace(*cmd) && i < 200)
		name[j++] = *(cmd++);
	name[j] = 0;
	current = e->channels;
	while (current)
	{
		if (!ft_strcmp(((t_chan *)(current->content))->name, name))
			return (add_to_chan(e, i, current->content));
		current = current->next;
	}
	current = ft_lstnew(malloc(sizeof(t_chan)), sizeof(t_chan));
	ft_strcpy(((t_chan *)(current->content))->name, name);
	((t_chan *)(current->content))->users = NULL;
	ft_lstadd(&e->channels, current);
	return (add_to_chan(e, i, ((t_chan *)(current->content))));
}

void		cmd_leave(t_env *e, size_t i, char *cmd)
{
	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	(void)(e && i && cmd);
}

void		cmd_names(t_env *e, size_t i, char *cmd)
{
	while (ft_isspace(*cmd) && *cmd)
		cmd++;
	(void)(e && i && cmd);
}
