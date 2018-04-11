/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:29:52 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/11 13:44:12 by azaliaus         ###   ########.fr       */
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
	rdr->line = NULL;
	rdr->next = NULL;
	return (rdr);
}

static t_reader	*load_reader(t_reader **reader, int fd)
{
	t_reader		*cpy;

	cpy = *reader;
	if (!cpy)
		return (*reader = init_reader(fd));
	else
	{
		while (cpy)
		{
			if (cpy->fd == fd)
				return (cpy);
			if (!cpy->next)
				break ;
			cpy = cpy->next;
		}
		cpy->next = init_reader(fd);
		return (cpy->next);
	}
	return (NULL);
}

static int		read_line(t_reader *reader, char **line)
{
	int		ret;
	char	buff[BUFF_SIZE + 1];
	char	*tmp;

	if (!reader->line || !ft_strchr(reader->line, '\n'))
	{
		while ((ret = read(reader->fd, &buff, BUFF_SIZE)) > 0)
		{
			buff[ret] = '\0';
			tmp = reader->line;
			reader->line = ft_strjoin(reader->line, buff);
			free(tmp);
			if (ft_strchr(reader->line, '\n'))
				break ;
		}
		if (ret < 0 || (ret == 0 && !reader->line))
			return (ret);
	}
	if (ft_strlen(tmp = reader->line) == 0)
		return (0);
	*line = ft_get_next_word(reader->line, '\n');
	ret = ft_strlen(*line) + (ft_strchr(reader->line, '\n') ? 1 : 0);
	reader->line = ft_strsub(reader->line, ret, -ret + ft_strlen(reader->line));
	free(tmp);
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
	return (read_line(curr, line));
}
