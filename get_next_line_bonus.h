/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrowang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 05:29:30 by tbrowang          #+#    #+#             */
/*   Updated: 2021/12/15 22:57:54 by tbrowang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <stddef.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
# define MAX_OPEN 65535

typedef enum s_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_buffer
{
	int				length;
	int				index;
	char			*buff;
	t_bool			eof;
	struct s_buffer	*next;
}		t_buffer;

char	*get_next_line(int fd);
#endif
