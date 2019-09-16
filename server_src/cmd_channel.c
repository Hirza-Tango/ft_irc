/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_channel.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 11:28:04 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/16 14:38:20 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	add_to_chan(t_env *e, size_t i, t_chan *chan)
{
	ft_lstadd(&chan->users, ft_lstnew(ft_strdup(e->fds[i].nick), 10));
	reply(I_WRITE, RPL_NOTOPIC, chan->name, "No topic is set");
	cmd_names(e, i, chan->name);
}

static void	del_from_chan(t_env *e, size_t i, t_chan *chan)
{
	t_list *current;
	t_list *prev;

	current = chan->users;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(current->content, e->fds[i].nick))
		{
			if (!prev)
				chan->users = current->next;
			else
				prev->next = current->next;
			free(current->content);
			free(current);
			REPL_ERR(RPL_NONE, NULL, NULL);
		}
		prev = current;
		current = current->next;
	}
	if (!current)
		REPL_ERR(ERR_NOTONCHANNEL, chan->name, "You're not on that channel");
}

void		cmd_join(t_env *e, size_t i, char *cmd)
{
	t_list	*current;
	char	*name;

	if (!e->fds[i].nick[0])
		REPL_ERR(ERR_NOTREGISTERED, NULL, "You have not registered");
	name = get_arg(&cmd);
	if (!*name)
		REPL_ERR(ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
	if (!is_valid_chan(name))
		REPL_ERR(ERR_NOSUCHCHANNEL, name, "No such channel");
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

void		cmd_part(t_env *e, size_t i, char *cmd)
{
	t_list	*current;
	char	*name;

	if (!e->fds[i].nick[0])
		REPL_ERR(ERR_NOTREGISTERED, NULL, "You have not registered");
	name = get_arg(&cmd);
	if (!*name)
		REPL_ERR(ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
	if (!is_valid_chan(name))
		REPL_ERR(ERR_NOSUCHCHANNEL, name, "No such channel");
	current = e->channels;
	while (current)
	{
		if (!ft_strcmp(((t_chan *)(current->content))->name, name))
			break ;
		current = current->next;
	}
	if (!current)
		REPL_ERR(ERR_NOSUCHCHANNEL, name, "No such channel");
	del_from_chan(e, i, current->content);
}

static void	print_members_chan(t_env *e, size_t i, t_chan *chan)
{
	t_list	*current;

	if (!chan->users)
		return ;
	cbuff_write(I_WRITE, RPL_NAMREPLY);
	cbuff_write(I_WRITE, " ");
	cbuff_write(I_WRITE, chan->name);
	cbuff_write(I_WRITE, " :");
	current = chan->users;
	while (current)
	{
		cbuff_write(I_WRITE, current->content);
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
