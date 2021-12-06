/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrowang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 05:14:41 by tbrowang          #+#    #+#             */
/*   Updated: 2021/12/06 10:25:47 by tbrowang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

t_buffer	*free_lst(t_buffer *buffer_lst)
{
	t_buffer *tmp_buff;

	tmp_buff = buffer_lst->next;
	free(buffer_lst->buff);
	free(buffer_lst);
	buffer_lst = NULL;
	return (tmp_buff);
}

char	*build_string(int pos_eol, t_buffer **buffer_lst)
{
	int		index;
	char	*str;
	char	*ptr_str;

	if ((*buffer_lst)->length == 0)
		return (NULL);
	str = malloc(sizeof(char) * (pos_eol + 1));
	ptr_str = str;
	index = (*buffer_lst)->index;
	//	printf("before ptr    = %p\n", *buffer_lst);
	printf("buff       = >>>%s<<<\n", (*buffer_lst)->buff + pos_eol);
	while (pos_eol--)
	{
		*str++ = (*buffer_lst)->buff[index++];
		if (index == BUFF_SIZE/*(*buffer_lst)->length*/)
		{
			*buffer_lst = free_lst(*buffer_lst);
			//		printf("ptr    = %p\n", *buffer_lst);
			index = 0;
		}
	}
	//	printf("after ptr    = %p\n", *buffer_lst);
	(*buffer_lst)->index = index;
	*str = '\0';
	return (ptr_str);
}

t_buffer	*init_buffer(int fd)
{
	t_buffer	*new_buffer;

	new_buffer = malloc(sizeof(t_buffer));
	if (!new_buffer)
		return (NULL);
	new_buffer->buff = malloc(sizeof(char) * BUFF_SIZE);
	if (!(new_buffer->buff))
	{
		free(new_buffer);
		return (NULL);
	}
	new_buffer->length = read(fd, new_buffer->buff, BUFF_SIZE);
	if (new_buffer->length < 0)
	{
		free(new_buffer->buff);
		free(new_buffer);
		return (NULL);
	}
	new_buffer->index = 0;
	if (new_buffer->length == 0)
		new_buffer->eof = TRUE;
	else
		new_buffer->eof = FALSE;
	new_buffer->next = NULL;
	return (new_buffer);
}

int	search_eol(int fd, t_buffer *buffer_lst)
{
	int			i;
	int			pos_endl;
	t_buffer	*tmpbuff;

	i = buffer_lst->index;
	pos_endl = 0;
	tmpbuff = NULL;
	while (buffer_lst->buff[i] && i <= buffer_lst->length)
	{

		pos_endl++;
		if ((i == buffer_lst->length && buffer_lst->eof == TRUE)
				|| buffer_lst->buff[i] == '\n')
			break ;
		i++;
		if (i == buffer_lst->length)
		{
			tmpbuff = init_buffer(fd);
			tmpbuff->next = buffer_lst->next;
			buffer_lst->next = tmpbuff;
			buffer_lst = tmpbuff;
			i = 0;
			printf("length     = %d\n", buffer_lst->length);
			printf("index      = %d\n", buffer_lst->index);
			printf("buff       = %s\n", buffer_lst->buff);
			printf("eof        = %d\n", buffer_lst->eof);
		}
	}
//	printf("length     = %d\n", buffer_lst->length);
//	printf("index      = %d\n", buffer_lst->index);
//	printf("buff       = %s\n", buffer_lst->buff);
//	printf("eof        = %d\n", buffer_lst->eof);

	return (pos_endl);
}

char	*get_next_line(int fd)
{
	static t_buffer	*fd_open[MAX_OPEN];
	int				pos_endl;
	char			*str;

	if (!fd_open[fd])
		fd_open[fd] = init_buffer(fd);
	//	printf("length     = %d\n", fd_open[fd]->length);
	//	printf("index      = %d\n", fd_open[fd]->index);
	//	printf("buff       = %s\n", fd_open[fd]->buff);
	//	printf("eof        = %d\n", fd_open[fd]->eof);
	pos_endl = search_eol(fd, fd_open[fd]);
	//	printf("pos_eof    = %d\n", pos_endl);
//	printf("buff       = >>>%s<<<\n", fd_open[fd]->buff + pos_endl);
	str = build_string(pos_endl, &fd_open[fd]);
	return (str);	
}
