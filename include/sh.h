/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:15:49 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 12:21:02 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdbool.h>
# include <limits.h>
#include <unistd.h>
# define JOY "\xF0\x9F\x98\x82"

typedef struct	s_buffer
{
	char	payload[4096];
	ssize_t	length;
}				t_buffer;

typedef struct	s_map
{
	size_t	len;
	char	*entries[];
}				t_map;

typedef	struct	s_command
{
	bool		found;
	bool		is_builtin;
	char		path[PATH_MAX];
	char		args[ARG_MAX];
	char		**argv;
	size_t		argc;
	t_map		*env;
}				t_command;

int				reader(char **input);
void			sh_signals(void);
int				sh_loop(int argc, char **argv, char **env);
t_command		*parse(char **input);
bool			is_builtin(char *cmd);
t_command		parser(char *input, const t_map *envp_map);
void			exec_process(t_command *cmd);
void			exec_builtin(t_command *cmd);
void			builtin_runner(char *cmd, char **argv);
t_map			*init_env_map(char **envp);
t_map			*copy_env_map(const t_map *old_env_map);
char			*get_env(const t_map *envp_map, const char *key);
int				set_env(t_map **env_map, const char *arg);
void			print_env(const t_map *env_map);
extern char		*builtins[];
extern pid_t	child_pid;

#endif
