/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 10:46:55 by dslogrov          #+#    #+#             */
/*   Updated: 2019/08/30 08:59:06 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

int		x_int(int err, int res, char *str)
{
	if (res == err)
	{
		fprintf(stderr, "%s error (%s, %d): %s\n",
			str, __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	return (res);
}

void	*x_void(void *err, void *res, char *str)
{
	if (res == err)
	{
		fprintf(stderr, "%s error (%s, %d): %s\n",
			str, __FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	return (res);
}
