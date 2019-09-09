/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 06:32:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/09/09 13:03:30 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ring_buffer.h"

t_cbuff	cbuff_create(size_t buffer_size)
{
	t_cbuff	cbuff;

	cbuff = malloc(sizeof(struct s_cbuff));
	cbuff->buffer = malloc(buffer_size);
	ft_bzero(cbuff->buffer, buffer_size);
	cbuff->head = 0;
	cbuff->tail = 0;
	cbuff->capacity = buffer_size;
	cbuff->is_full = 0;
	return (cbuff);
}

void	cbuff_destroy(t_cbuff cbuff)
{
	free(cbuff->buffer);
	free(cbuff);
}

int		cbuff_write(t_cbuff cbuff, char *data)
{
	const size_t len = ft_strlen(data);

	if (len > cbuff->capacity - cbuff_size(cbuff))
		return (-1);
	if (cbuff->head + len > cbuff->capacity)
	{
		ft_strncpy(cbuff->buffer + cbuff->head, data,
			cbuff->capacity - cbuff->head);
		ft_strcpy(cbuff->buffer, data + cbuff->capacity - cbuff->head);
	}
	else
		ft_strcpy(cbuff->buffer + cbuff->head, data);
	cbuff->head = (cbuff->head + len) % cbuff->capacity;
	cbuff->is_full = (cbuff->head == cbuff->tail);
	return (0);
}

void	cbuff_overwrite(t_cbuff cbuff, char *data)
{
	const size_t len = ft_strlen(data);

	if (len > cbuff->capacity)
		return ;
	if (len > cbuff->capacity - cbuff_size(cbuff))
	{
		cbuff->tail = (cbuff->tail + len - cbuff->capacity + cbuff_size(cbuff))
			% cbuff->capacity;
	}
	if (cbuff->head + len > cbuff->capacity)
	{
		ft_strncpy(cbuff->buffer + cbuff->head, data,
			cbuff->capacity - cbuff->head);
		ft_strcpy(cbuff->buffer, data + cbuff->capacity - cbuff->head);
	}
	else
		ft_strcpy(cbuff->buffer + cbuff->head, data);
	cbuff->head = (cbuff->head + len) % cbuff->capacity;
	cbuff->is_full = (cbuff->head == cbuff->tail);
}

size_t	cbuff_cmd_size(t_cbuff cbuff)
{
	char *newl;

	if (cbuff_isempty(cbuff))
		return (0);
	if (cbuff->head <= cbuff->tail) //wraparound
	{
		if ((newl = ft_memchr(cbuff->buffer + cbuff->tail, '\n',
			cbuff->capacity - cbuff->tail)))
			return (newl - cbuff->buffer + 1);
		else if ((newl = ft_memchr(cbuff->buffer, '\n', cbuff->head)))
			return (cbuff->capacity - cbuff->tail + (newl - cbuff->buffer));
		else
			return (0);
	}
	else if ((newl = ft_memchr(cbuff->buffer + cbuff->tail, '\n',
			cbuff->head - cbuff->tail + 1)))
		return (newl - cbuff->buffer - cbuff->tail + 1);
	else
		return (0);
}

int		cbuff_read(t_cbuff cbuff, char *string_buff)
{
	int		size;
	//FIXME: only read between tail and head
	if (!(size = cbuff_cmd_size(cbuff)))
		return (0);
	if (cbuff->tail + size > cbuff->capacity)
	{
		ft_memcpy(string_buff, cbuff->buffer + cbuff->tail,
			cbuff->capacity - cbuff->tail);
		ft_memcpy(string_buff + cbuff->capacity - cbuff->tail, cbuff->buffer,
			size - cbuff->capacity + cbuff->tail);
	}
	else
	{
		ft_memcpy(string_buff, cbuff->buffer + cbuff->tail, size);
	}
	cbuff->tail = (cbuff->tail + size) % cbuff->capacity;
	return (size);
}
