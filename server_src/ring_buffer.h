/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 06:20:50 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 09:57:36 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RING_BUFFER_H
# define RING_BUFFER_H

# include <string.h>
# include <stdlib.h>

struct	s_cbuff
{
	char	**buffer;
	size_t	head;
	size_t	tail;
	size_t	capacity;
	char	is_full;
};

typedef struct s_cbuff* t_cbuff;

t_cbuff	cbuff_create(size_t string_size, size_t nstrings);
void	cbuff_destroy(t_cbuff cbuff);
void	cbuff_reset(t_cbuff cbuff);
int		cbuff_write(t_cbuff cbuff, char *data);
void	cbuff_overwrite(t_cbuff cbuff, char *data);
int		cbuff_read(t_cbuff cbuff, char **string_buff);
char	cbuff_isempty(t_cbuff cbuff);
char	cbuff_isfull(t_cbuff cbuff);
size_t	cbuff_cap(t_cbuff cbuff);
size_t	cbuff_size(t_cbuff cbuff);

#endif
