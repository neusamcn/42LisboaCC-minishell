/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:54:32 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/26 15:58:31 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
#include "../../include/execution.h"


void	readevalprint_input(char *input_str, t_shelly *shelly)
{
	t_token		*tokens;
	t_cmd_line	*cmd_line;

	add_history(input_str);
	tokens = tokenize_input(input_str);
	// DELETE TESTER
	// print_tkns(tokens);
	//shelly->cur_tok = tokens;
	tokens = expand_params(tokens, shelly);
	// DELETE TESTER
	// print_tkns(tokens);
	shelly->cur_tok = tokens;
	cmd_line = parser(tokens);
	// DELETE TESTER
	// print_cmdline(cmd_line);
	shelly->cur_cmd = cmd_line;
	exec_loop(cmd_line, shelly);
	free_tkn(tokens);
	shelly->cur_tok = NULL;
	free_cmd_line(cmd_line);
	shelly->cur_cmd = NULL;
	free(input_str);
}

void	noninteractive_readevalprint_input(char *line, t_shelly *shelly)
{
	t_token		*tkn;
	t_cmd_line	*cmd_l;

	tkn = tokenize_input(line);
	if (!tkn)
		return ;
	tkn = expand_params(tkn, shelly);
	shelly->cur_tok = tkn;
	cmd_l = parser(tkn);
	shelly->cur_cmd = cmd_l;
	exec_loop(cmd_l, shelly);
	free_tkn(tkn);
	shelly->cur_tok = NULL;
	free_cmd_line(cmd_l);
	shelly->cur_cmd = NULL;
}
