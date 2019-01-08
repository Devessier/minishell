/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 16:10:47 by bdevessi          #+#    #+#             */
/*   Updated: 2019/01/08 21:57:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "libft.h"

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
		envp_map->entries[i - 1] = envp[i - 1];
	return (envp_map);
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

t_map	*append_env_var(t_map *map, char *key, char *value)
{
	t_map	*tmp;
	char	*new_entry;
	int		i;

	if (!(tmp = malloc(sizeof(t_map) + sizeof(char *) * (map->len + 2))))
		return (NULL);
	tmp->len = map->len + 1;
	i = 0;
	tmp->entries[tmp->len] = NULL;
	while (map->entries[i++])
		tmp->entries[i - 1] = map->entries[i - 1];
	if (!(new_entry = ft_strnew(ft_strlen(key) + 1 + ft_strlen(value))))
	{
		free(tmp);
		return (NULL);
	}
	i = 0;
	while (*key)
		new_entry[i++] = *key++;
	new_entry[i++] = '=';
	while (*value)
		new_entry[i++] = *value++;
	tmp->entries[tmp->len - 1] = new_entry;
	return (tmp);
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

void	replace_env_var(char *entry, char *value, size_t value_len)
{
	while (*entry && *entry != '=')
		entry++;
	entry++;
	ft_strlcat(entry, value, value_len);
}

t_map	*set_env_var(t_map *env_map, const char *name, const char *value)
{
	const size_t	value_len = ft_strlen(value);
	size_t			i;
	char			*tmp;
	char			*found;
	char			*equal;

	i = 0;
	if (!(name && value))
		return (env_map);
	while ((tmp = env_map->entries[i++]))
		if ((found = ft_strstr(tmp, name)) == tmp)
		{
			equal = ft_strstr(tmp, "=");
			if (ft_strlen(tmp + (equal - tmp + 1)) < value_len)
				env_map->entries[i - 1] = generate_env_var(tmp, (char *)value, value_len);
			else
				replace_env_var(tmp, (char *)value, value_len);
			return (env_map);
		}
	return (append_env_var(env_map, (char *)name, (char *)value));
}
