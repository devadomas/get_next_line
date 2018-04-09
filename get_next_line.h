/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaliaus <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 14:28:49 by azaliaus          #+#    #+#             */
/*   Updated: 2018/04/09 16:11:28 by azaliaus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
/* # include "libft.h" */
# include <stdlib.h>
# define BUF_SIZE 32

typedef struct		s_reader
{
	int				fd;
	int				error;
	char			*line;
	struct s_reader	*next;
}					t_reader;

int					get_next_line(const int fd, char **line);

#endif
