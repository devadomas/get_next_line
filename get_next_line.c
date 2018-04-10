/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:29:52 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/10 18:39:08 by azaliaus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


#include <stdio.h>

static t_reader	*init_reader(int fd)
{
	t_reader		*rdr;

	rdr = (t_reader *)malloc(sizeof(t_reader));
	if (!rdr)
		return (0);
	rdr->fd = fd;
	rdr->error = 0;
	rdr->error = 0;
	rdr->line = NULL;
	rdr->next = NULL;
	return (rdr);
}

static t_reader	*load_reader(t_reader **reader, int fd)
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
		while (cpy)
		{
			if(cpy->fd == fd)
				return (cpy);
			if (!cpy->next)
				break ;
			cpy = cpy->next;
		}
		cpy->next = init_reader(fd);
		return (cpy->next);
	}
	return (0);
}

static int		read_line(t_reader *reader, char **line)
{
	int		cut_len;
	int		read_ret;
	char	buff[BUFF_SIZE + 1];

	if (!reader->line || !ft_strchr(reader->line, '\n'))
	{
		while ((read_ret = read(reader->fd, &buff, BUFF_SIZE)) > 0)
		{
			buff[read_ret] = '\0';
			reader->line = ft_strjoin(reader->line, buff);
			if (ft_strchr(reader->line, '\n'))
				break ;
		}
		if (read_ret < 0 || (read_ret == 0 && !reader->line))
			return (read_ret);
	}
	if (!(reader->line))
		return (-1);
	if (ft_strlen(reader->line) == 0)
		return (0);
	*line = ft_get_next_word(reader->line, '\n');
	cut_len = ft_strlen(*line) + (ft_strchr(reader->line, '\n')? 1 : 0);
	reader->line = ft_strsub(reader->line, cut_len,
			(int)ft_strlen(reader->line) - cut_len);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static t_reader		*reader;
	t_reader			*curr;

	if (fd < 0 || !line)
		return (-1);
	if (!(curr = load_reader(&reader, fd)))
		return (-1);
	if (!curr->error)
		return (read_line(curr, line));
	return (-1);
}
