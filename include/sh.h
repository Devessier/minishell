/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:15:49 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 12:18:04 by bdevessi         ###   ########.fr       */
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

typedef	struct	s_command
{
	char	**env;
	char	path[PATH_MAX];
	bool	found;
	bool	is_builtin;
	char	args[ARG_MAX];
}				t_command;

int				reader(char **input);
void			sh_signals(void);
int				sh_loop(int argc, char **argv, char **env);
t_command		*parse(char **input);
bool			is_builtin(char *cmd);
t_command		parser(char *input, char **env);
void			exec_process(t_command *cmd);
void			exec_builtin(t_command *cmd);
void			builtin_runner(char *cmd);
extern char		*builtins[];
extern pid_t	child_pid;

#endif
