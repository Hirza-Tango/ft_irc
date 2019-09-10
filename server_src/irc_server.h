/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:32:33 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/10 13:35:02 by dslogrov         ###   ########.fr       */
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

typedef enum	e_reply_codes{
	RPL_NONE = 300,
	RPL_TOPIC = 332,
	RPL_NOTOPIC = 331,
	RPL_NAMREPLY = 353,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461
}				t_reply_codes;

# define BUF_SIZE	4096

# define USAGE	"Usage: %s port\n"

# define I_READ		e->fds[i].buf_read
# define I_WRITE	e->fds[i].buf_write

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
void			reply(t_cbuff write_buffer, int code, char *value);
int				is_valid_nick(char *nick);
int				is_valid_chan(char *chan);

void			cmd_privmsg(t_env *e, size_t i, char *cmd);
void			cmd_join(t_env *e, size_t i, char *cmd);
void			cmd_leave(t_env *e, size_t i, char *cmd);
void			cmd_names(t_env *e, size_t i, char *cmd);

#endif
