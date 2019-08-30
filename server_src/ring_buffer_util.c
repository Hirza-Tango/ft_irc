/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ring_buffer_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:48:19 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 09:58:37 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ring_buffer.h"

void	cbuff_reset(t_cbuff cbuff)
{
	cbuff->is_full = 0;
	cbuff->head = 0;
	cbuff->tail = 0;
}

char	cbuff_isempty(t_cbuff cbuff)
{
	return (cbuff->head == cbuff->tail && !cbuff->is_full);
}

char	cbuff_isfull(t_cbuff cbuff)
{
	return (cbuff->is_full);
}

size_t	cbuff_cap(t_cbuff cbuff)
{
	return (cbuff->capacity);
}

size_t	cbuff_size(t_cbuff cbuff)
{
	if (cbuff->is_full)
		return (cbuff->capacity);
	else if (cbuff->head >= cbuff->tail)
		return (cbuff->head - cbuff->tail);
	else
		return (cbuff->capacity - cbuff->head + cbuff->tail);
}
