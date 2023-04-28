#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(infooo *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		delete_info(info);
		if (is_interactive(info))
			print_string("$ ");
		print_char(BUF_FLUSH);
		r = input_getter(info);
		if (r != -1)
		{
			put_info(info, av);
			builtin_ret = get_builtin(info);
			if (builtin_ret == -1)
				find_cmmd(info);
		}
		else if (is_interactive(info))
			_putchar('\n');
		delete_info_all(info, 0);
	}
	print_hiss(info);
	delete_info_all(info, 1);
	if (!is_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_number == -1)
			exit(info->status);
		exit(info->err_number);
	}
	return (builtin_ret);
}

/**
 * get_builtin - finds a builtins command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtins not found,
 * 	0 if builtins executed successfully,
 * 	1 if builtins found but not successful,
 * 	2 if builtins signals exit()
 */
int get_builtin(infooo *info)
{
	int i, built_in_ret = -1;
	builtin_vars builtintbl[] = {
		{"exit", exit_shell},
		{"env", current_env},
		{"help", helper},
		{"history", get_historyyyy},
		{"setenv", init_env},
		{"unsetenv", remove_env},
		{"cd", change_dirr},
		{"alias", get_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (compare_strings(info->argv[0], builtintbl[i].type) == 0)
		{
			info->count_line++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmmd(infooo *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->conutline_flag == 1)
	{
		info->count_line++;
		info->conutline_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delimeter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = get_path(info, get_enviroment(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmmd(info);
	}
	else
	{
		if ((is_interactive(info) || get_enviroment(info, "PATH=")
					|| info->argv[0][0] == '/') && is_a_cmd(info, info->argv[0]))
			fork_cmmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_e_message(info, "not found\n");
		}
	}
}

/**
 * fork_cmmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmmd(infooo *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, copy_env_string(info)) == -1)
		{
			delete_info_all(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_e_message(info, "Permission denied\n");
		}
	}
}
