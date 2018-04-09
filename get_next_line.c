/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:29:52 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/09 19:51:21 by azaliaus         ###   ########.fr       */
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
		while (cpy->next)
		{
			if(cpy->fd == fd)
				return (cpy);
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
	char	buff[BUF_SIZE];
	char	*tmp;

	if (!reader->line)
	{
		read_ret = read(reader->fd, &buff, BUF_SIZE);
		if (read_ret < 0)
			return (-1);
		reader->line = ft_strdup(buff);
	}
	if (!ft_strchr(buff, '\n')) /* load more */
	{
		printf("needs to load more\n");
		read_ret = read(reader->fd, &buff, BUF_SIZE);
		if (read_ret < 0)
			return (-1);
		reader->line = ft_strjoin(reader->line, buff);
		return (read_line(reader, line));
	}
	else
	{
			tmp = ft_get_next_word(reader->line, '\n');
			*line = tmp;
			cut_len = ft_strlen(tmp) + 1;
			tmp = ft_strdup(reader->line);
			if (reader->line)
				free(reader->line);
			reader->line = ft_strsub(tmp, cut_len,
					ft_strlen(buff) - cut_len - 1);
			if (tmp)
				free(tmp);
			printf("Left in str: %s\n", reader->line);
			return (ft_strlen(*line));
	}
	return (0);
}

/*static int		read_line(t_reader *reader, char **line)
{
	int		cut_len;
	int		read_ret;
	char	buff[BUF_SIZE];
	char	*tmp;

	while ((read_ret = read(reader->fd, &buff, BUF_SIZE)) > 0)
	{
		if (!ft_strstr(buff, "\n"))
			reader->line = ft_strjoin(reader->line, buff);
		else
		{
			tmp = ft_get_next_word(buff, '\n');
			*line = ft_strjoin(reader->line, tmp);
			cut_len = ft_strlen(tmp) + 1;
			if (reader->line)
				free(reader->line);
			reader->line = ft_strsub(buff, cut_len,
					ft_strlen(buff) - cut_len - 1);
			if (tmp)
				free(tmp);
			printf("Left in str: %s\n", reader->line);
			return (ft_strlen(*line));
		}
	}
	reader->error = 1;
	return (-1);
}*/

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
