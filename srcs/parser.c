/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:45:46 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 18:32:44 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <unistd.h>

bool	can_execute_path(char *path)
{
	int			can_exec;

	if (path == NULL)
		return (false);
	can_exec = access(path, X_OK) == 0;
	free(path);
	return (can_exec);
}

char	*lookup_path(char *cmd, char *usr_path, bool is_builtin)
{
	char	*colon;
	char	*path;

	if (!cmd)
		return (NULL);
	if (is_builtin || usr_path == NULL || *usr_path == '\0')
		return (cmd);
	while (*usr_path)
	{
		colon = ft_strchr(usr_path, ':');
		if (colon != NULL)
			*colon = '\0';
		path = ft_strjoin(usr_path, cmd);
		if (path == NULL)
			continue ;
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		usr_path = colon + 1;
	}
	free(cmd);
	return (NULL);
}

void		init_cmd(t_command *command, char **env)
{
	command->env = env;
	command->path = NULL;
	command->is_builtin = false;
	command->args = NULL;
}

t_command	*parser(char *input, char **env)
{
	t_command	*command;
	size_t		i;
	char		*cmd;

	if (!(command = malloc(sizeof(t_command))))
		return (NULL);
	init_cmd(command, env);
	i = 0;
	while (*input && (*input)++ != ' ')
		i++;
	if (i == 0)
	{
		free(command);
		return (NULL);
	}
	if (!(cmd = ft_strndup(input, i)))
	{
		free(command);
		return (NULL);
	}
	command->is_builtin = is_builtin(cmd);
	if (!(command->path = lookup_path(cmd, "/Users/bdevessi/.nvm/versions/node/v11.2.0/bin:/Users/bdevessi/.brew/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/Applications/VMware Fusion.app/Contents/Public:/Library/Frameworks/Mono.framework/Versions/Current/Commands:/usr/local/munki:/Applications/Wireshark.app/Contents/MacOS",command->is_builtin )))
	{
		free(command);
		return (NULL);
	}
	if (!(command->args = ft_strdup(input + ++i)))
	{
		free(command);
		free(cmd);
		free(command->path);
		return (NULL);
	}
	return (command);
}
