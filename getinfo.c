#include "shell.h"

/**
 * delete_info - initializes infooo struct
 * @info: struct address
 */
void delete_info(infooo *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * put_info - initializes infooo struct
 * @info: struct address
 * @av: argument vector
 */
void put_info(infooo *info, char **av)
{
	int i = 0;

	info->filename = av[0];
	if (info->arg)
	{
		info->argv = tokinize_string(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = duplicate_string(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		aliasreplace(info);
		var_replace(info);
	}
}

/**
 * delete_info_all - frees infooo struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void delete_info_all(infooo *info, int all)
{
	ffree_memory(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buffer)
			free(info->arg);
		if (info->env)
			freemem_list(&(info->env));
		if (info->history)
			freemem_list(&(info->history));
		if (info->alias)
			freemem_list(&(info->alias));
		ffree_memory(info->environ);
			info->environ = NULL;
		free_buff((void **)info->cmd_buffer);
		if (info->readfile > 2)
			close(info->readfile);
		print_char(BUF_FLUSH);
	}
}
