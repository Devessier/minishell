/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 13:15:38 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/07 13:57:09 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <stdio.h>

int	main(int argc, char **argv, char **env)
{
	sh_signals();
	return sh_loop(argc, argv, env);
}