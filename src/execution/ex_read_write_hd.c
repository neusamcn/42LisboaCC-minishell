/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_read_write_hd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:05:40 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 16:26:38 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/parsing.h"

void	writehd(char *msg, int fd, t_shelly *shelly, bool quoted)
{
	char	*line;

	line = msg;
	if (!quoted)
		line = word_param_expansion(msg, shelly);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	if (!quoted)
		free(line);
	free(msg);
}

char	*readhd(void)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}