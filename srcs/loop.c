/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:37:45 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 21:55:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <signal.h>
#include "libft.h"
#include <stdio.h>

pid_t	child_pid = 0;

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (child_pid != 0)
			kill(child_pid, SIGKILL);
		ft_putchar('\n');
	}
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
	const t_map	*envp_map = init_env_map(env);
	char		*input;
	int			status;

	int i = 0;
	while (envp_map->entries[i])
		puts(envp_map->entries[i++]);
	input = NULL;
	(void)argc, (void)argv;
	while (1)
	{
		ft_putf(JOY "  > ");
		if ((status = reader(&input)) > 0)
		{
			t_command cmd = parser(input, env);
			if (!cmd.found)
				ft_putf("joysh \\o/ : %s: command not found\n", cmd.path);
			else if (!cmd.is_builtin)
				exec_process(&cmd);
			else
				exec_builtin(&cmd);
		}
		else if (status == 0)
			builtin_runner("exit", NULL);
		free(input);
	}
}
