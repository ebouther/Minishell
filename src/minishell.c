/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 17:37:30 by ebouther          #+#    #+#             */
/*   Updated: 2016/03/29 19:02:25 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_get_in_env(char *search, char **env)
{
	int	i;
	char *ret;

	i = 0;
	ret = NULL;
	while (env[i])
		if ((ret = ft_strstr((const char *)env[i++], search)) != NULL)
			return (i - 1);
	return (-1);
}


// Should be done in father process
static void	ft_change_directory(char **arg)
{
	int	i;
	char directory[1024];

	i = -1;
	while (arg[++i])
		;

	getcwd(directory, sizeof(directory));
	ft_printf("CURRENT DIR : '%s'\n", directory);
	if (i > 2)
	{
		ft_printf("TOO BIG\n");
	}
	else if (i > 1)
	{
		if (access(arg[1], R_OK) == 0)
		{
			ft_printf("HERE WE GO\n");
			if (chdir((const char *)arg[1]) == -1)
				ft_printf("CHDIR Error\n");
			getcwd(directory, sizeof(directory));
			ft_printf("CURRENT DIR : '%s'\n", directory);
		}
	}
	else
	{
	 //(test if  ~ / - / no arg)
	}
}

static char	**ft_get_user_input(pid_t pid)
{
	char	**arg;
	char	*str;

	arg = NULL;
	if (get_next_line(1, &str))
	{
		if (ft_strlen((char *)(arg = ft_strsplit(str, ' '))) > 0)
		{
			if (ft_strcmp(arg[0], "exit") == 0)
			{
				kill(pid, SIGQUIT);
			}
			else if (ft_strcmp(arg[0], "cd") == 0)
			{
				ft_change_directory(arg);
			}
			else if (ft_strcmp(arg[0], "clear") == 0)
				ft_printf("\033[2J\033[1;1H");
			else
				return (arg);
		}
	}
	return (NULL);
}

static void	ft_find_and_exec_bin(char **input, char **env)
{
	char	*path;
	char	**split;
	int		i;
	int		n;

	if ((i = ft_get_in_env("PATH=", env)) != -1)
	{
		//Should check before if the path doesn't already exists
		split = ft_strsplit(env[i], ':');
		while (*split != '\0')
		{
			if (access(path = ft_strjoin(*split, ft_strjoin("/", input[0])), X_OK) == 0)
			{
				if (execve(path, input, NULL) == -1)
					ft_printf("EXECVE ERROR.\n");
				return ;
			}
			split++;
		}
		n = 0;
		ft_printf("minishell: command not found:");
		while (input[n])
			ft_printf(" %s", input[n++]);
		ft_putchar('\n');
		exit(-1);
	}
}

int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	char	**input;

	(void)ac;
	(void)av;

	ft_printf("Goes there\n");
	while (42)
	{
		pid = fork();
		if (pid == 0) //father process
		{
			ft_printf("$> ");
			if ((input = ft_get_user_input(pid)) != NULL)
				ft_find_and_exec_bin(input, env);
			else
				exit(0);
		}
		else if (pid > 0) //child process
			wait(NULL);
		else
		{
			ft_printf("Fork error.\n");
			exit(-1);
		}
	}
	return (0);
}
