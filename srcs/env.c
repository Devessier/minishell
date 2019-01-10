/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 16:10:47 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/10 13:25:05 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"
#include <stdio.h>

size_t	env_size(char **envp)
{
	size_t	size;

	size = 0;
	while (envp[size] != NULL)
		size++;
	return (size);
}

t_map	*init_env_map(char **envp)
{
	const size_t	envp_size = env_size(envp);
	t_map			*envp_map;
	size_t			i;

	if (!(envp_map = malloc(sizeof(t_map) + sizeof(char *) * (envp_size + 1))))
		return (NULL);
	envp_map->len = envp_size;
	envp_map->entries[envp_size] = NULL;
	i = 0;
	while (envp[i++])
		envp_map->entries[i - 1] = ft_strdup(envp[i - 1]);
	return (envp_map);
}

t_map	*copy_env_map(const t_map *old_env_map)
{
	t_map	*new_env_map;
	size_t	i;

	if (!(new_env_map = malloc(sizeof(t_map) + sizeof(char *) + (old_env_map->len + 1))))
		return (NULL);
	new_env_map->len = old_env_map->len;
	new_env_map->entries[new_env_map->len] = NULL;
	i = 0;
	while (old_env_map->entries[i++])
		new_env_map->entries[i - 1] = old_env_map->entries[i - 1];
	return (new_env_map);
}

char	*new_map_entry(char *key, char *value)
{
	const size_t	key_len = ft_strlen(key);
	const size_t	value_len = ft_strlen(value);
	char			*tmp;

	if (!(tmp = ft_strnew(key_len + 1 + value_len + 1)))
		return (NULL);
	ft_strcat(tmp, key);
	ft_strcat(tmp + key_len, "=");
	ft_strcat(tmp + key_len + 1, value);
	return (tmp);
}

int		append_env_var(t_map **map, char *key, char *value)
{
	t_map	*tmp;
	char	*new_entry;
	size_t	i;

	if (!(tmp = malloc(sizeof(t_map) + sizeof(char *) * ((*map)->len + 2))))
		return (-1);
	tmp->len = (*map)->len + 1;
	i = 0;
	tmp->entries[tmp->len] = NULL;
	while (i < (*map)->len && (*map)->entries[i++])
		tmp->entries[i - 1] = (*map)->entries[i - 1];
	if (!(new_entry = new_map_entry(key, value)))
    {
        free(tmp);
        return (-1);
    }
	free(*map);
	tmp->entries[tmp->len - 1] = new_entry;
	*map = tmp;
	return (0);
}

char	*generate_env_var(char *entry, char *value, size_t new_val_len)
{
	size_t	key_len;
	char	*tmp;

	key_len = 0;
	while (entry[key_len] && entry[key_len] != '=')
		key_len++;
	if (!(tmp = malloc(sizeof(char) * (key_len + 2 + new_val_len))))
		return (NULL);
	ft_memcpy(tmp, entry, key_len + 1);
	ft_strcat(tmp + key_len + 1, value);
	return (tmp);
}

void	replace_env_var(char *entry, char *value)
{
	while (*entry && *entry != '=')
		entry++;
	entry++;
	ft_strcpy(entry, value);
}

int		set_env_var(t_map **env_map, const char *name, const char *value)
{
	const size_t	value_len = ft_strlen(value);
	size_t			i;
	char			*tmp;
	char			*found;
    bool            was_found;
	char			*equal;

	i = 0;
    was_found = false;
	if (!(name && value))
		return (0);
	while (i < (*env_map)->len && (tmp = (*env_map)->entries[i++]))
		if ((found = ft_strstr(tmp, name)) == tmp)
		{
            was_found = true;
			equal = ft_strchr(tmp, '=');
			printf("equal = %p, i = %zu\n", equal, i);
			if (equal && ft_strlen(tmp + (equal - tmp + 1)) < value_len)
			{
				free((*env_map)->entries[i - 1]);
				(*env_map)->entries[i - 1] = generate_env_var(tmp, (char *)value, value_len);
			}
			else
				replace_env_var(tmp, (char *)value);
		}
	if (!was_found)
		append_env_var(env_map, (char *)name, (char *)value);
	return (0);
}

int		set_env(t_map **env_map, const char *arg)
{
	char		buffer[PATH_MAX];
	const char	*equal = ft_strchr(arg, '=');

	ft_bzero(buffer, sizeof(buffer));
	if (!ft_isalpha(*arg))
		ft_putf_fd(2, "setenv: Variable name must begin with a letter.\n");
	else if (equal == NULL || equal + 1 == '\0')
		return (set_env_var(env_map, arg, ""));
	else
	{
		ft_strncat(buffer, arg, equal - arg);
		return (set_env_var(env_map, buffer, equal + 1));
	}
	return (0);
}

char	*get_env(const t_map *envp_map, const char *key)
{
	const size_t	key_len = ft_strlen(key);
	ssize_t			i;
	char			*equal;

	i = -1;
	while (envp_map->entries[++i])
		if (ft_strstr(envp_map->entries[i], key) == envp_map->entries[i])
			if (*(equal = envp_map->entries[i] + key_len) == '\0'
					|| *equal == '=')
				return (*equal == '=' ? equal + 1 : NULL);
	return (NULL);
}

void	print_env(const t_map *envp_map)
{
	size_t	i;

	i = 0;
	while (i < envp_map->len && envp_map->entries[i++])
		printf("[%lu] %s\n", i - 1, envp_map->entries[i - 1]);
	printf("end\n");
}
