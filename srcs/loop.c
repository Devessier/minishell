/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:37:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 18:27:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <signal.h>
#include "libft.h"
#include <stdio.h>

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putf("yo !");
}

void	sighup_handler(int sig)
{
	(void)sig;
	ft_putf("lel !");
}

void	sh_signals(void)
{
	signal(SIGHUP, sighup_handler);
	signal(SIGINT, sigint_handler);
}

int		sh_loop(int argc, char **argv, char **env)
{
	char	*input;

	input = NULL;
	(void)argc, (void)argv, (void)env;
	ft_putf("😂  > ");
	while (1)
	{
		if (reader(&input) > 0)
		{
			printf("evaluate string : %s\n", input);
			t_command *parsed_str = parser(input, env);
			printf("path = %s\n", parsed_str->path);
			ft_putf("😂  > ");
		}
	}
}
