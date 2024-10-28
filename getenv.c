#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Pointer to environment array.
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments.
 * @var: the string env var property
 * Return: 1 on delete, 0 otherwise
 */
int _unsetenv(info_t *info, char *var)
{
	size_t i = 0;
	char *p;
	list_t *node = info->env;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			node = info->env;
			i = 0;
			continue;
		}
		i++;
		node = node->next;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment variable or modify an existing one
 * @info: Structure containing potential arguments.
 * @var: the string env var property
 * @value: the string env var value
 * Return: 0 on success, 1 on failure
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf;
	list_t *node;
	char *p;

	if (!var || !value)
		return (1);

	buf = malloc(_strlen(var) + _strlen(value) + 2);

	if (!buf)
		return (1);

	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);

	for (node = info->env; node; node = node->next)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
	}

	add_node_end(&(info->env), buf, 0);
	info->env_changed = 1;
	return (0);
}