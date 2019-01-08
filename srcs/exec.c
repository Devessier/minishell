/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:12:50 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 12:18:52 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdbool.h>
#include <unistd.h>
#include "stdlib.h"
#include <stdio.h>

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

void	builtin_runner(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
	{
		ft_putf("exit\n");
		exit(0);
	}
}

void	exec_builtin(t_command *cmd)
{
	builtin_runner(cmd->path);
}

void	exec_process(t_command *cmd)
{
	int			status;

	child_pid = fork();
	if (child_pid == -1)
	{
		ft_putf("Error during forking\n");	
		perror("sh: ");
	}
	else if (child_pid == 0)
	{
		char	*array[] = { cmd->path, NULL };
		if (execve(cmd->path, array, cmd->env) == -1)
			ft_putf("Error during child execution\n");
	}
	else
	{
		waitpid(child_pid, &status, 0);
	}
}
