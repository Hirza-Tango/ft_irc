/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:32:33 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/11 15:53:36 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_H
# define IRC_SERVER_H

# include "ring_buffer.h"
# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"

# include <sys/select.h>
# include <sys/resource.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

# define RPL_NONE				"300"
# define RPL_TOPIC				"332"
# define RPL_NOTOPIC			"331"
# define RPL_NAMREPLY			"353"
# define RPL_ENDOFNAMES			"366"
# define ERR_NOSUCHNICK			"401"
# define ERR_NOSUCHCHANNEL		"403"
# define ERR_NORECIPIENT		"411"
# define ERR_NOTEXTTOSEND		"412"
# define ERR_UNKNOWNCOMMAND		"421"
# define ERR_NONICKNAMEGIVEN	"431"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_NICKNAMEINUSE		"433"
# define ERR_NOTONCHANNEL		"442"
# define ERR_NOTREGISTERED		"451"
# define ERR_NEEDMOREPARAMS		"461"

# define BUF_SIZE	4096

# define USAGE	"Usage: %s port\n"

# define I_READ		e->fds[i].buf_read
# define I_WRITE	e->fds[i].buf_write
# define REPL_ERR(x, y, z) { reply(I_WRITE, x, y, z); return ; }

typedef struct	s_fd
{
	void	(*fct_read)();
	void	(*fct_write)();
	t_cbuff	buf_read;
	t_cbuff	buf_write;
	int		type;
	char	nick[10];
}				t_fd;

typedef struct	s_chan
{
	char	name[201];
	t_list	*users;
}				t_chan;

typedef struct	s_env
{
	t_fd	*fds;
	size_t	max;
	rlim_t	maxfd;
	int		port;
	int		r;
	fd_set	fd_read;
	fd_set	fd_write;
	t_list	*channels;
}				t_env;

void			srv_create(t_env *e, int port);
void			srv_accept(t_env *e, int s);
void			client_read(t_env *e, int cs);
void			client_write(t_env *e, int cs);
void			clean_fd(t_fd *fd);
int				x_int(int err, int res, char *str);
void			*x_void(void *err, void *res, char *str);

void			handle_cmd(t_env *e);
void			reply(t_cbuff write_buffer, char *code, char *target,
	char *value);
char			*get_arg(char **cmd);
int				is_valid_nick(char *nick);
int				is_valid_chan(char *chan);

void			cmd_privmsg(t_env *e, size_t i, char *cmd);
void			cmd_join(t_env *e, size_t i, char *cmd);
void			cmd_part(t_env *e, size_t i, char *cmd);
void			cmd_names(t_env *e, size_t i, char *cmd);

#endif
