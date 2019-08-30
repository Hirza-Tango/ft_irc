/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 06:32:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 13:36:16 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ring_buffer.h"

t_cbuff	cbuff_create(size_t buffer_size)
{
	t_cbuff	cbuff;

	cbuff = malloc(sizeof(struct s_cbuff));
	cbuff->buffer = malloc(buffer_size);
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
	const size_t len = strlen(data);

	if (len > cbuff->capacity - cbuff_size(cbuff))
		return (-1);
	if (cbuff->head + len > cbuff->capacity)
	{
		strncpy(cbuff->buffer + cbuff->head, data,
			cbuff->capacity - cbuff->head);
		strcpy(cbuff->buffer, data + cbuff->capacity - cbuff->head);
	}
	else
		strcpy(cbuff->buffer + cbuff->head, data);
	cbuff->head = (cbuff->head + len) % cbuff->capacity;
	cbuff->is_full = (cbuff->head == cbuff->tail);
	return (0);
}

void	cbuff_overwrite(t_cbuff cbuff, char *data)
{
	const size_t len = strlen(data);

	if (len > cbuff->capacity)
		return ;
	if (len > cbuff->capacity - cbuff_size(cbuff))
	{
		cbuff->tail = (cbuff->tail + len - cbuff->capacity + cbuff_size(cbuff))
			% cbuff->capacity;
	}
	if (cbuff->head + len > cbuff->capacity)
	{
		strncpy(cbuff->buffer + cbuff->head, data,
			cbuff->capacity - cbuff->head);
		strcpy(cbuff->buffer, data + cbuff->capacity - cbuff->head);
	}
	else
		strcpy(cbuff->buffer + cbuff->head, data);
	cbuff->head = (cbuff->head + len) % cbuff->capacity;
	cbuff->is_full = (cbuff->head == cbuff->tail);
}

int		cbuff_read(t_cbuff cbuff, char *string_buff)
{
	char	*newl;

	newl = memchr(cbuff->buffer + cbuff->tail, '\n',
		cbuff->capacity - cbuff->tail);
	if (!newl)
		newl = memchr(cbuff->buffer, '\n', cbuff->tail);
	else
	{
		strncpy(string_buff, cbuff->buffer + cbuff->tail, newl - cbuff->tail - cbuff->buffer);
		cbuff->tail = (newl + 1 - cbuff->buffer) % cbuff->capacity;
		return (1);
	}
	if (!newl)
		return (0);
	else
	{
		strncpy(string_buff, cbuff->buffer + cbuff->tail,
			cbuff->capacity - cbuff->tail);
		strncpy(string_buff + cbuff->capacity - cbuff->tail, cbuff->buffer,
			newl - cbuff->buffer);
		cbuff->tail = (newl + 1 - cbuff->buffer) % cbuff->capacity;
		return (1);
	}
}
