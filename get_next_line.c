/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:29:52 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/10 15:15:54 by azaliaus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


#include <stdio.h>

static t_reader	*init_reader(int fd)
{
	t_reader		*rdr;
	char c[10];

	rdr = (t_reader *)malloc(sizeof(t_reader));
	if (!rdr)
		return (0);
	rdr->fd = fd;
	rdr->error = 0;
	rdr->error = read(fd, &c, 0);
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
			{
				return (cpy);
			}
			if (cpy->next)
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
	char	*tmp;

	read_ret = -1;
	if (!reader->line || !strchr(reader->line, '\0')) /* loading more if needed */
	{
		while (((read_ret = read(reader->fd, &buff, BUFF_SIZE)) > 0)
				|| !strchr(reader->line, '\n'))
		{
			if (read_ret == 0)
				break ;
			buff[read_ret] = '\0';
			if (!reader->line)
				reader->line = ft_strdup(buff);
			else
				reader->line = ft_strjoin(reader->line, buff);
		}
		if (read_ret < 0)
			return (-1);
		if (read_ret == 0 && !reader->line)
			return (0);
	}
	/* after loading more */
	if (ft_strlen(reader->line) == 0)
		return (0);
	tmp = ft_get_next_word(reader->line, '\n');
	*line = tmp;
	cut_len = ft_strlen(tmp) + (ft_strchr(reader->line, '\n')? 1 : 0);
	//printf("Counter: %d\n",(int)ft_strlen(reader->line) - cut_len);
	if (ft_strlen(reader->line) == ft_strlen(*line))
		reader->line = ft_strdup("");
	else
		reader->line = ft_strsub(reader->line, cut_len,
			(int)ft_strlen(reader->line) - cut_len);
	
	//printf("-------------------------\n");
	return (ft_strlen(*line));
	//return (0);
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
