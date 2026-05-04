/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 00:04:41 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/01 14:48:21 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static char	*build_pretty_prompt(char *user, char *prompt, char *cwd)
{
	char	*tmp_prompt[8];
	char	*full_prompt;
	size_t	len;
	int		i;

	tmp_prompt[0] = "\001"LIGHT_PINK"\002";
	tmp_prompt[1] = user;
	tmp_prompt[2] = "\001"COLOR_RESET"\002@\001"ORCHID"\002";
	tmp_prompt[3] = prompt;
	tmp_prompt[4] = "\001"COLOR_RESET"\002:\001"DEEP_PINK"\002";
	tmp_prompt[5] = cwd;
	tmp_prompt[6] = "\001"COLOR_RESET"\002$ ";
	tmp_prompt[7] = NULL;
	len = 0;
	i = 0;
	while (tmp_prompt[i])
		len += ft_strlen(tmp_prompt[i++]);
	full_prompt = ft_calloc_protec(len + 1, sizeof(char));
	i = 0;
	while (tmp_prompt[i])
		ft_strlcat(full_prompt, tmp_prompt[i++], len + 1);
	return (full_prompt);
}

char	*put_prompt(t_shelly *shelly, char *prompt)
{
	char	cwd[PATH_MAX];
	char	*full_prompt;
	char	*input;
	char	*user;

	getcwd_protec(cwd, PATH_MAX, shelly);
	user = find_var_shellyenvp(shelly, "USER");
	full_prompt = build_pretty_prompt(user, prompt, cwd);
	input = readline(full_prompt);
	free(full_prompt);
	return (input);
}
