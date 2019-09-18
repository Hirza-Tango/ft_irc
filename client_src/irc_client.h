/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 09:47:40 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/18 12:26:50 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_H
# define IRC_CLIENT_H

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <netdb.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <pwd.h>
# include <errno.h>

# define READ_BUFF_SIZE	512

typedef struct	s_env
{
	char			*port;
	int				socket_fd;
	char			*host;
	char			buff[READ_BUFF_SIZE];
	fd_set			sock_set;
	struct addrinfo	hint;
}				t_env;

void			client_connect(t_env *e);
void			irc_write(t_env *e, char *cmd);
void			irc_read(t_env *e);
char			*get_arg(char **cmd);
void			init_connect(t_env *e, char **argv);
void			cmd_connect(t_env *e, char *cmd);

#endif
