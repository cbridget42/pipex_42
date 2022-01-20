/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:14:44 by cbridget          #+#    #+#             */
/*   Updated: 2022/01/20 14:27:06 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "libft.h"

int	check_commands(char **argv, char **envp, char ***cmar, int mod)
{
	int		i;
	int		err;
	int		argc;
	char	**spath;

	i = 0;
	argc = save_argc(argv);
	err = save_arco(argc, argv, cmar);
	if (err)
		return (err);
	spath = ft_split(search_path(envp), ':');
	if (!spath)
		return (1);
	if (mod == 1)
		err = check_rights(argc - 4, spath, cmar);
	else if (mod == 2)
		err = check_rights(0, spath, cmar);
	else
	{
		while (i < (argc - 3))
		{
			err = check_rights(i, spath, cmar);
			i++;
		}
	}
	clean_s(spath);
	if (err)
		return (err);
	return (0);
}

int	check_rights(int num, char **spath, char ***cmar)
{
	char	*tmp;

	if (!(access(*cmar[num], F_OK | X_OK)))
		return (0);
	while (*spath)
	{
		tmp = cr_path(num, spath, cmar);
		if (!tmp)
			return (1);
		if (!(access(tmp, F_OK | X_OK)))
		{
			free(*cmar[num]);
			*cmar[num] = tmp;
			return (0);
		}
		free(tmp);
		spath++;
	}
	put_error(*cmar[num], 0);
	return (1);
}

char	*cr_path(int num, char **spath, char ***cmar)
{
	char	*tmp;
	int		length;

	length = ft_strlen(*cmar[num]);
	length += ft_strlen(*spath);
	tmp = (char *)malloc(sizeof(char) * (length + 2));
	if (!tmp)
		return ((void *)0);
	ft_strlcpy(tmp, *spath, ft_strlen(*spath) + 1);
	ft_strlcat(tmp, "/", ft_strlen(tmp) + 2);
	ft_strlcat(tmp, *cmar[num], length + 2);
	return (tmp);
}

int	save_arco(int argc, char **argv, char ***cmar)
{
	int	i;
	int	j;

	i = 0;
	j = 2;
	while (i < (argc - 3))
	{
		cmar[i] = ft_split(argv[j], ' ');
		if (!cmar[i])
			return (1);
		i++;
		j++;
	}
	return (0);
}

char	*search_path(char **env)
{
	int		i;
	char	*src;

	i = 0;
	src = "PATH";
	while (env)
	{
		while ((*env)[i] == src[i])
		{
			if (i == 3)
				return (&(*env)[i + 2]);
			i++;
		}
		i = 0;
		env++;
	}
	return ((void *)0);
//	return "/Users/cbridget/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/share/dotnet:/usr/local/munki:/opt/X11/bin:~/.dotnet/tools";
}

int	save_argc(char **argv)
{
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	return (argc);
}
