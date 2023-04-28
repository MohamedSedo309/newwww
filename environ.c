#include "shell.h"

/**
 * current_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int current_env(infooo *info)
{
	print_str_part(info->env);
	return (0);
}

/**
 * get_enviroment - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *get_enviroment(infooo *info, const char *name)
{
	list_s *node = info->env;
	char *p;

	while (node)
	{
		p = begain_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * init_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int init_env(infooo *info)
{
	if (info->argc != 3)
	{
		print_message_error("Incorrect number of arguements\n");
		return (1);
	}
	if (make_env(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * remove_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int remove_env(infooo *info)
{
	int i;

	if (info->argc == 1)
	{
		print_message_error("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		remove_env_var(info, info->argv[i]);

	return (0);
}

/**
 * make_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int make_env_list(infooo *info)
{
	list_s *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		create_end_node(&node, environ[i], 0);
	info->env = node;
	return (0);
}
