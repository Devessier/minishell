/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:45:46 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 15:44:51 by bdevessi         ###   ########.fr       */
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

size_t	count_blank(char *str)
{
	size_t	count;

	count = 0;
	while (*str)
		if (*str++ == ' ')
			count++;
	return (count);
}

char	*clean_input(char *str)
{
	const size_t    len = ft_strlen(str);
	char            last_char;
	int             i;

	i = len - 1;
	while (i >= 0 && str[i] == ' ')
		str[i--] = '\0';
	while (*str && *str == ' ')
		str++;
	i = 0;
	last_char = -1;
	while (str[i])
	{
		if (last_char == ' ' && str[i] == ' ')
		{
			while (str[i] && str[i] == ' ')
				ft_strcpy(str + i - 1, str + i);
			last_char = ' ';
		}
		else
			last_char = str[i++];
	}
	return (str);
}

uint8_t		fill_argv(t_command *cmd, size_t blanks)
{
	char	*start;
	int		i;
	int		j;

	if (!(cmd->argv = malloc(sizeof(char **) * (blanks + 2))))
		return (1);
	cmd->argv[blanks + 1] = NULL;
	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		start = cmd->args + i;
		while (cmd->args[i] && cmd->args[i] != ' ')
			i++;
		cmd->args[i++] = '\0';
		cmd->argv[j++] = start;
	}
	return (0);
}

t_command	parser(char *input, char **env)
{
	t_command	command;
	size_t		blanks;
	char		*colon;

	command = (t_command) { .env = env, .found = true, .is_builtin = false };
	ft_bzero(command.path, PATH_MAX);
	input = clean_input(input);
	ft_strcpy(command.args, input);
	blanks = count_blank(input);
	if ((colon = ft_strchr(input, ' ')) == NULL)
		ft_strcpy(command.path, input);
	else
		ft_memcpy(command.path, input, colon - input);
	if (fill_argv(&command, blanks) != 0)
		return (command);
	command.is_builtin = is_builtin(command.path);
	char		*usr_path = "/Users/bdevessi/.nvm/versions/node/v11.2.0/bin:/Users/bdevessi/.brew/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/Applications/VMware Fusion.app/Contents/Public:/Library/Frameworks/Mono.framework/Versions/Current/Commands:/usr/local/munki:/Applications/Wireshark.app/Contents/MacOS";
	lookup_path(&command, usr_path);
	return (command);
}
