#include "shell.h"

/**
 * exit_shell - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */
int exit_shell(infooo *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = err_convert_str_to_int(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_e_message(info, "Illegal number: ");
			print_message_error(info->argv[1]);
			print_char('\n');
			return (1);
		}
		info->err_number = err_convert_str_to_int(info->argv[1]);
		return (-2);
	}
	info->err_number = -1;
	return (-2);
}

/**
 * change_dirr - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int change_dirr(infooo *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		print_string("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = get_enviroment(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = get_enviroment(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (compare_strings(info->argv[1], "-") == 0)
	{
		if (!get_enviroment(info, "OLDPWD="))
		{
			print_string(s);
			print_char('\n');
			return (1);
		}
		print_string(get_enviroment(info, "OLDPWD=")), print_char('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = get_enviroment(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_e_message(info, "can't cd to ");
		print_message_error(info->argv[1]), print_char('\n');
	}
	else
	{
		make_env(info, "OLDPWD", get_enviroment(info, "PWD="));
		make_env(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * helper - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int helper(infooo *info)
{
	char **arg_array;

	arg_array = info->argv;
	print_string("help call works. Function not yet implemented \n");
	if (0)
		print_string(*arg_array); /* temp att_unused workaround */
	return (0);
}
