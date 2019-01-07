/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:13:00 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 15:25:48 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"

static int	complete_partially(t_buffer *b, char *nl, char **string)
{
	char	*str;
	char	*tmp;

	tmp = *string;
	if (!((str = ft_strndup(b->payload, nl - b->payload))
		&& (*string = tmp ? ft_strjoin(*string, str) : str)))
	{
		free(str);
		free(*string);
		return (-1);
	}
	ft_memcpy(b->payload, nl + 1, b->length);
	if (tmp)
	{
		free(tmp);
		free(str);
	}
	b->length -= nl - b->payload + 1;
	return (0);
}

static int	read_buffer(t_buffer *b, char **string)
{
	char	*nl;
	char	*tmp;

	if (b->length == 0)
		return (1);
	if ((nl = ft_memchr(b->payload, '\n', b->length)))
		return (complete_partially(b, nl, string));
	tmp = *string;
	b->payload[b->length] = '\0';
	if (!(*string = tmp ? ft_strjoin(tmp, b->payload) : ft_strdup(b->payload)))
		return (-1);
	ft_bzero(b->payload, 4096);
	if (tmp)
		free(tmp);
	return (1);
}

int			reader(char **input)
{
	static t_buffer	buffer;
	int				result;
	ssize_t			read_bytes;
	char			*string;

	string = NULL;
	while ((result = read_buffer(&buffer, &string)))
	{
		if (result == -1
				|| (read_bytes = read(0, buffer.payload, 4096)) < 0)
		{
			if (string)
				free(string);
			return (-1);
		}
		if (buffer.length && !read_bytes)
		{
			buffer.length = read_bytes;
			break ;
		}
		else if (!(buffer.length || read_bytes))
			return ((int)(*input = string) & 0);
		buffer.length = read_bytes;
	}
	return ((int)(*input = string) || 1);
}
