/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:12:50 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 13:00:57 by bdevessi         ###   ########.fr       */
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

void	builtin_runner(char *cmd, char **argv)
{
	static char	path[PATH_MAX];

	if (ft_strcmp(cmd, "exit") == 0)
	{
		ft_putf("exit\n");
		exit(0);
	}
	else if (ft_strcmp(cmd, "cd") == 0)
	{
		if (getcwd(path, sizeof(path)) == NULL)
			ft_putf("error\n");
		printf("path = %s, sizeof = %zu\n", path, sizeof(path));
		printf("second = %p\n", argv[1]);
		if (argv[1] == NULL)
		{
			if (chdir(path) == -1)
				ft_putf("error\n");
		}
		else
			chdir(argv[1]);
	}
}

void	exec_builtin(t_command *cmd)
{
	builtin_runner(cmd->path, cmd->argv);
}

void	exec_process(t_command *cmd)
{
	int			status;

	int i = 0;
	while (cmd->argv[i++])
		ft_putf("argv[%d] = %s\n", i - 1, cmd->argv[i - 1]);
	child_pid = fork();
	if (child_pid == -1)
		ft_putf("Error during forking\n");	
	else if (child_pid == 0)
	{
		ft_putf("first printing\n");
		print_env(cmd->env);
		ft_putf("second printing\n");
		print_env(cmd->env);
		if (execve(cmd->path, cmd->argv, cmd->env->entries) == -1)
			ft_putf("Error during child execution\n");
	}
	else
		waitpid(child_pid, &status, 0);
}
