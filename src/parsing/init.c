/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/26 12:01:44 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
#include "../../include/execution.h"

// DELETE TESTERS
void	print_tkns(t_token *tkn)
{
	t_token	*curr_tkn = tkn;

	while (curr_tkn)
	{
		printf("Token[%d]: '%s'\n", curr_tkn->index, curr_tkn->value);
		printf("word was expanded? 0 = no, 1 = yes: %d\n", curr_tkn->word_xpndd);
		printf("token->type: ");
		if (curr_tkn->type == WORD)
		{
			if (curr_tkn->word == CMD)
				printf("WORD, CMD\n");
			else if (curr_tkn->word == QMARK1)
				printf("WORD, QMARK1\n");
			else if (curr_tkn->word == QMARK2)
				printf("WORD, QMARK2\n");
			else
				printf("error!\n");
		}
		else if (curr_tkn->type == REDIR)
		{
			if (curr_tkn->redir == IN)
				printf("REDIR, IN\n");
			else if (curr_tkn->redir == OUT)
				printf("REDIR, OUT\n");
			else if (curr_tkn->redir == APPEND)
				printf("REDIR, APPEND\n");
			else if (curr_tkn->redir == HEREDOC)
				printf("REDIR, HEREDOC\n");
			else
				printf("error!\n");
		}
		else if (curr_tkn->type == CTRL_OP)
		{
			if (curr_tkn->ctrlop == PIPE)
				printf("CTRL_OP, PIPE\n");
			else
				printf("error!\n");
		}
		curr_tkn = curr_tkn->next;
	}
	printf("\n\n");
}

void	print_cmdline(t_cmd_line *cmdline)
{
	t_cmd_line	*curr_cmdline = cmdline;

	while (curr_cmdline)
	{
		int			i = -1;
		while (curr_cmdline->cmds[++i])
			printf("cmd_line->cmds[%d]: '%s'\n", i, curr_cmdline->cmds[i]);
		curr_cmdline = curr_cmdline->next;
	}
	printf("\n\n");
}

static char	*input_strs_join(char *input_str, char *extra_input)
{
	char	*tmp;
	char	*full_input;

	tmp = ft_strjoin(input_str, "\n");
	if (!tmp)
		return (NULL);
	full_input = ft_strjoin(tmp, extra_input);
	// add_ptr_shelly(full_input); // TODO: review if needed
	free(tmp);
	return (full_input);
}

static char	*validate_complete_input(char *input_str, t_shelly *shelly)
{
	char	*syntax_err;
	char	*extra_input;
	char	*full_input;

	syntax_err = syntax_check(input_str);
	while (syntax_err && (!(ft_strcmp(syntax_err, "incomplete"))
			|| !(ft_strcmp(syntax_err, "'")) || !(ft_strcmp(syntax_err, "\""))))
	{
		extra_input = put_extra_prompt(shelly, input_str);
		full_input = input_strs_join(input_str, extra_input);
		free(input_str);
		free(extra_input);
		if (!full_input)
			return (NULL);
		input_str = full_input;
		syntax_err = syntax_check(input_str);
	}
	if (syntax_err)
	{
		syntax_err_msg(syntax_err);
		add_history(input_str);
		free(input_str);
		return (NULL);
	}
	return (input_str);
}

static void	readevalprint_input(char *input_str, t_shelly *shelly)
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

static void	read_eval_print_loop(t_shelly *shelly)
{
	char	*input_str;

	sig_mode(INTERACTIVE, shelly);
	while (1)
	{
		input_str = put_prompt(shelly, "shelly");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS, shelly);
		if (*input_str)
		{
			input_str = validate_complete_input(input_str, shelly);
			if (input_str)
				readevalprint_input(input_str, shelly);
		}
		else
			free(input_str);
	}
}

static void	non_interactive_mode(t_shelly *shelly)
{
	char		*line;
	t_token		*tokens;
	t_cmd_line	*cmd_line;

	sig_mode(CHILD, shelly);
	while (1)
	{
		tokens = NULL;
		cmd_line = NULL;
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		tokens = tokenize_input(line);
		//shelly->cur_tok = tokens;
		if (!tokens)
		{
			free(line);
			continue ;
		}
		tokens = expand_params(tokens, shelly);
		shelly->cur_tok = tokens;
		cmd_line = parser(tokens);
		shelly->cur_cmd = cmd_line;
		exec_loop(cmd_line, shelly);
		free_tkn(tokens);
		shelly->cur_tok = NULL;
		free_cmd_line(cmd_line);
		shelly->cur_cmd = NULL;
		free(line);
	}
}

/* t_shelly	*init(char **envp)
{
    t_shelly	*shelly;

    shelly = set_shellyenvp(envp);
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
    {
        ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
        read_eval_print_loop(shelly);
    }
    else
        non_interactive_mode(shelly);
    return (shelly);
} */

t_shelly	*init(char **envp)
{
	t_shelly	*shelly;

	shelly = set_shellyenvp(envp);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		read_eval_print_loop(shelly);
	else
		non_interactive_mode(shelly);
	return (shelly);
}
