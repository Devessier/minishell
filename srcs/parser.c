/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:45:46 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 11:52:25 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

void	lookup_path(t_command *cmd, char *usr_path)
{
	char	path[PATH_MAX];
	char	*colon;

	if (!cmd)
		return ;
	if (cmd->is_builtin || usr_path == NULL || *usr_path == '\0')
		return ;
	ft_strcpy(path, cmd->path);
	while (*usr_path)
	{
		ft_bzero(path, PATH_MAX);
		colon = ft_strchr(usr_path, ':');
		if (colon == NULL)
			ft_strcpy(path, usr_path);
		else
			ft_memcpy(path, usr_path, colon - usr_path);
		ft_strcat(path, "/");
		ft_strcat(path, cmd->path);
		if (access(path, X_OK) == 0)
			return ((void)ft_strcpy(cmd->path, path));
		if (colon == NULL)
			break ;
		usr_path = colon + 1;
	}
	cmd->found = false;
}

t_command	parser(char *input, char **env)
{
	t_command	command;
	char		*colon;

	command = (t_command) { .env = env, .found = true, .is_builtin = false };
	ft_bzero(command.path, PATH_MAX);
	if ((colon = ft_strchr(input, ' ')) == NULL)
		ft_strcpy(command.path, input);
	else
		ft_memcpy(command.path, input, colon - input);
	command.is_builtin = is_builtin(command.path);
	char		*usr_path = "/Users/bdevessi/.nvm/versions/node/v11.2.0/bin:/Users/bdevessi/.brew/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/Applications/VMware Fusion.app/Contents/Public:/Library/Frameworks/Mono.framework/Versions/Current/Commands:/usr/local/munki:/Applications/Wireshark.app/Contents/MacOS";
	lookup_path(&command, usr_path);
	return (command);
}
