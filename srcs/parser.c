/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 15:45:46 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 13:06:17 by bdevessi         ###   ########.fr       */
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
			return (ft_strcpy(cmd->path, path));
		if (colon == NULL)
			break ;
		usr_path = colon + 1;
	}
	cmd->found = false;
}

size_t	count_args(char *str)
{
	size_t	count;

	count = 0;
	while (*str)
		if (*str++ == ' ')
			count++;
	return (++count);
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

uint8_t		fill_argv(t_command *cmd, size_t args)
{
	char	*start;
	int		i;
	int		j;

	if (!(cmd->argv = malloc(sizeof(char *) * (args + 1))))
		return (1);
	cmd->argv[args] = NULL;
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
	printf("argv[0] = |%s|\n", cmd->argv[0]);
}

t_map		*configure_env(t_command *cmd, const t_map *envp_map)
{
	bool	exec_env;
	t_map	*tmp_env_map;
	int		i;

	if (!(tmp_env_map = copy_env_map(envp_map)))
		return ((t_map *)envp_map);
	while ((exec_env = ft_strcmp(cmd->argv[0], "env") == 0)
			&& cmd->argv[1] != NULL)
	{
		i = 0;
		while (cmd->argv[++i]
				&& ft_strchr(cmd->argv[i], '=') != NULL)
		{
			printf("[%i] = |%s|\n", i, cmd->argv[i]);
			set_env(&tmp_env_map, cmd->argv[i]);
		}
		if (cmd->argv[i] != NULL)
		{
			ft_memmove(cmd->argv, cmd->argv + i, (cmd->argc - i) * sizeof(char *));
			cmd->argv[cmd->argc - i] = NULL;
			ft_strcpy(cmd->path, *cmd->argv);
		}
		else
			break ;
	}
	return (tmp_env_map);
}

t_command	parser(char *input, const t_map *envp_map)
{
	t_command	command;
	char		*colon;

	command = (t_command) { .found = true, .is_builtin = false };
	ft_bzero(command.path, PATH_MAX);
	clean_input(input);
	ft_strcpy(command.args, input);
	command.argc = count_args(input);
	if ((colon = ft_strchr(input, ' ')) == NULL)
		ft_strcpy(command.path, input);
	else
		ft_memmove(command.path, input, colon - input);
	if (fill_argv(&command, command.argc) != 0)
		return (command);
	expansions(&command, envp_map);
	command.env = ft_strcmp(command.argv[0], "env") == 0
	   ? configure_env(&command, envp_map) : (t_map *)envp_map;
	command.is_builtin = is_builtin(command.path);
	lookup_path(&command, get_env(envp_map, "PATH"));
	return (command);
}
