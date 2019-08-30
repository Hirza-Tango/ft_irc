/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 06:32:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 09:58:09 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ring_buffer.h"

t_cbuff	cbuff_create(size_t string_size, size_t nstrings)
{
	size_t	i;
	t_cbuff	cbuff;

	cbuff = malloc(sizeof(struct s_cbuff));
	cbuff->buffer = malloc(sizeof(char *) * nstrings);
	i = 0;
	while (i < nstrings)
		cbuff->buffer[i++] = malloc(string_size);
	cbuff->head = 0;
	cbuff->tail = 0;
	cbuff->capacity = nstrings;
	cbuff->is_full = 0;
	return (cbuff);
}

void	cbuff_destroy(t_cbuff cbuff)
{
	size_t	i;

	i = 0;
	while (i < cbuff->capacity)
		free(cbuff->buffer[i++]);
	free(cbuff->buffer);
	free(cbuff);
}

int		cbuff_write(t_cbuff cbuff, char *data)
{
	if (cbuff->is_full)
		return (-1);
	strcpy(cbuff->buffer[cbuff->head], data);
	cbuff->head = (cbuff->head + 1) % cbuff->capacity;
	cbuff->is_full = cbuff->head == cbuff->tail;
	return (0);
}

void	cbuff_overwrite(t_cbuff cbuff, char *data)
{
	if (cbuff->is_full)
		cbuff->tail = (cbuff->tail + 1) % cbuff->capacity;
	strcpy(cbuff->buffer[cbuff->head], data);
	cbuff->head = (cbuff->head + 1) % cbuff->capacity;
	cbuff->is_full = cbuff->head == cbuff->tail;
}

int		cbuff_read(t_cbuff cbuff, char **string_buff)
{
	if (cbuff->head == cbuff->tail && !cbuff->is_full)
		return (-1);
	strcpy(*string_buff, cbuff->buffer[cbuff->tail]);
	cbuff->tail = (cbuff->tail + 1) % cbuff->capacity;
	cbuff->is_full = 0;
	return (0);
}
