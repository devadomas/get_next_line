/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:29:52 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/09 16:12:26 by azaliaus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_reader	*init_reader(int fd)
{
	t_reader		*rdr;

	rdr = (t_reader *)malloc(sizeof(t_reader));
	if (!rdr)
		return (0);
	rdr->fd = fd;
	rdr->error = 0;
	return (rdr);
}

static int		load_reader(t_reader **reader, int fd)
{
	t_reader		*cpy;

	cpy = *reader;
	if (!cpy)
	{
		*reader = init_reader(fd);
		return (*reader);
	}
	else
	{
		while (cpy->next)
		{
			if(cpy->fd == fd)
				return (cpy);
			cpy = cpy->next;
		}
		cpy->next = init_reader(fd);
		return (cpy->next);
	}
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	int					ret;
	static t_reader		*reader;
	t_reader			*curr;

	if (fd < 0 || !line)
		return (-1);
	if (!load_reader(&reader, fd))
		return (-1);

	return (ret);
}
