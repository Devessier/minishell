/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:45:46 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/09 17:39:36 by bdevessi         ###   ########.fr       */
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

	if (!cmd || cmd->is_builtin || usr_path == NULL
			|| *usr_path == '\0' || ft_strchr(cmd->path, '/') != NULL)
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

void		clean_input(char *str)
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
}

uint8_t		fill_argv(t_command *cmd, size_t blanks)
{
	char	*start;
	int		i;
	int		j;

	if (!(cmd->argv = malloc(sizeof(char *) * (blanks + 2))))
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

void		expansions(t_command *cmd, const t_map *envp_map)
{
	const char	*home = get_env(envp_map, "HOME");
	size_t		i;

	i = 0;
	while (cmd->argv[++i])
	{
		if (*cmd->argv[i] == '~')
			cmd->argv[i] = (char *)home;
		else if (*cmd->argv[i] == '$' && cmd->argv[i][1] != '\0')
			cmd->argv[i] = get_env(envp_map, cmd->argv[i] + 1);
	}
}

t_map		*handle_env(t_command *cmd, const t_map *envp_map)
{
	t_map	*tmp_env_map;
	size_t	i;

	if (ft_strcmp(cmd->path, "env") != 0
			|| !(tmp_env_map = copy_env_map(envp_map)))
		return ((t_map *)envp_map);
	i = 0;
	while (cmd->argv[++i]
			&& ft_strchr(cmd->argv[i], '=') != NULL)
		tmp_env_map = set_env(tmp_env_map, cmd->argv[i]);
	if (cmd->argv[i] != NULL)
	{
		cmd->argv = cmd->argv + i;
		ft_strcpy(cmd->path, *cmd->argv);
	}
	return (tmp_env_map);
}

t_command	parser(char *input, const t_map *envp_map)
{
	t_command	command;
	size_t		blanks;
	char		*colon;

	command = (t_command) { .found = true, .is_builtin = false };
	ft_bzero(command.path, PATH_MAX);
	clean_input(input);
	ft_strcpy(command.args, input);
	blanks = count_blank(input);
	if ((colon = ft_strchr(input, ' ')) == NULL)
		ft_strcpy(command.path, input);
	else
		ft_memcpy(command.path, input, colon - input);
	if (fill_argv(&command, blanks) != 0)
		return (command);
	expansions(&command, envp_map);
	command.env = handle_env(&command, envp_map);
	command.is_builtin = is_builtin(command.path);
	lookup_path(&command, get_env(envp_map, "PATH"));
	return (command);
}
