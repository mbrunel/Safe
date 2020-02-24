/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calls_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 18:22:17 by mbrunel           #+#    #+#             */
/*   Updated: 2020/02/24 19:04:28 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <psm.h>
#include <stdarg.h>

void	set_std(int in, int out)
{
		dup2(in, STDIN_FILENO);
		close(in);
		dup2(out, STDOUT_FILENO);
		close(out);
}

void	save_std(int *in, int *out)
{
		*in = dup(STDIN_FILENO);
		*out = dup(STDOUT_FILENO);
}

int		call(char *env[], char *exec , ...)
{
	char *av[100];
	va_list ap;
	int i = 0;
	int status;

	va_start(ap, exec);
	av[0] = exec;
	while ((av[++i] = va_arg(ap, char*)))
		;
	av[i] = NULL;
	if (!fork())
		execve(exec, av, env);
	wait(&status);
	va_end(ap);
	return (status);
}

