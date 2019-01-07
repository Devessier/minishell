/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:15:49 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 18:27:42 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include <sys/types.h>
# include <stdbool.h>

typedef struct	s_buffer
{
	char	payload[4096];
	ssize_t	length;
}				t_buffer;

typedef	struct	s_command
{
	char	**env;
	char	*path;
	bool	is_builtin;
	char	*args;
}				t_command;

int			reader(char **input);
void		sh_signals(void);
int			sh_loop(int argc, char **argv, char **env);
t_command	*parse(char **input);
bool		is_builtin(char *cmd);
t_command	*parser(char *input, char **env);
extern char	*builtins[];

#endif
