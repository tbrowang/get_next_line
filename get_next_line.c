/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrowang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 05:14:41 by tbrowang          #+#    #+#             */
/*   Updated: 2021/12/15 22:17:48 by tbrowang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

static t_buffer	*free_lst(t_buffer *buffer_lst)
{
	t_buffer	*tmp_buff;

	tmp_buff = buffer_lst->next;
	free(buffer_lst->buff);
	free(buffer_lst);
	buffer_lst = NULL;
	return (tmp_buff);
}

static t_buffer	*init_buffer(int fd)
{
	t_buffer	*new_buffer;

	new_buffer = malloc(sizeof(t_buffer));
	if (!new_buffer)
		return (NULL);
	new_buffer->buff = malloc(sizeof(char) * BUFFER_SIZE);
	*(new_buffer->buff) = '\0';
	if (!(new_buffer->buff))
		return (free(new_buffer), NULL);
	new_buffer->length = read(fd, new_buffer->buff, BUFFER_SIZE);
	if (new_buffer->length < 0)
	{
		free(new_buffer->buff);
		free(new_buffer);
		return (NULL);
	}
	new_buffer->index = 0;
	if (!new_buffer->length)
		new_buffer->eof = TRUE;
	else
		new_buffer->eof = FALSE;
	new_buffer->next = NULL;
	return (new_buffer);
}

static char	*build_string(register int pos_eol, register t_buffer **buffer_lst)
{
	register int		index;
	register char		*str;
	char				*ptr_str;

	if (!((*buffer_lst)->length))
		return (NULL);
	str = malloc(sizeof(char) * (pos_eol + 1));
	ptr_str = str;
	index = (*buffer_lst)->index;
	if (pos_eol == -1)
		return (free(str), NULL);
	while (pos_eol--)
	{
		*str++ = (*buffer_lst)->buff[index++];
		if (index == (*buffer_lst)->length)
		{
			*buffer_lst = free_lst(*buffer_lst);
			index = 0;
		}
	}
	if (*buffer_lst)
		(*buffer_lst)->index = index;
	*str = '\0';
	return (ptr_str);
}

static int	search_eol(int fd, register t_buffer *buffer_lst)
{
	register int			i;
	register int			pos_endl;
	t_buffer				*tmpbuff;

	i = buffer_lst->index;
	pos_endl = 0;
	while (buffer_lst->buff[i] && i <= buffer_lst->length)
	{
		++pos_endl;
		if ((i == buffer_lst->length && buffer_lst->eof)
			|| buffer_lst->buff[i] == '\n')
			break ;
		++i;
		if (i == buffer_lst->length)
		{
			tmpbuff = init_buffer(fd);
			if (!tmpbuff)
			{
				return (-1);
			}
			tmpbuff->next = buffer_lst->next;
			buffer_lst->next = tmpbuff;
			buffer_lst = tmpbuff;
			i = 0;
		}
	}
	return (pos_endl);
}

char	*get_next_line(int fd)
{
	static t_buffer	*fd_open[MAX_OPEN];
	int				pos_endl;
	char			*str;

	if (fd < 0)
		return (NULL);
	if (!fd_open[fd])
		fd_open[fd] = init_buffer(fd);
	if (!fd_open[fd])
		return (NULL);
	pos_endl = search_eol(fd, fd_open[fd]);
	if (fd_open[fd] != NULL && fd_open[fd]->eof == TRUE)
	{
		free(fd_open[fd]->buff);
		free(fd_open[fd]);
		fd_open[fd] = NULL;
		return (NULL);
	}
	str = build_string(pos_endl, &fd_open[fd]);
	return (str);
}
