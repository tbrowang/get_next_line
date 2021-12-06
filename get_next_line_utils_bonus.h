/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrowang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 16:19:52 by tbrowang          #+#    #+#             */
/*   Updated: 2021/12/06 18:12:49 by tbrowang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

void	*ft_memset(void *s, register int c, register size_t n)
{
	register unsigned char	*ptr;

	ptr = (unsigned char *) s;
	while (n-- > 0)
		*ptr++ = c;
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*to_alloc;

	to_alloc = malloc(nmemb * size);
	if (to_alloc == NULL)
		return (NULL);
	ft_memset(to_alloc, 0, nmemb * size);
	return (to_alloc);
}
