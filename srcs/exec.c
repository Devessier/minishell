/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:12:50 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 17:18:15 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>

char	*builtins[] = {
	"echo",
	"cd",
	"setenv",
	"unsetenv",
	"env",
	"exit",
	NULL
};

bool	is_builtin(char *cmd)
{
	uint8_t	i;

	i = 0;
	while (builtins[i])
		if (ft_strcmp(builtins[i++], cmd) == 0)
			return (true);
	return (false);
}
