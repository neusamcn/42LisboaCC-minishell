/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:24:29 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/19 15:10:51 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
#include "execution.h"

static t_cmd_line *new_cmd(void)
{
	t_cmd_line *cmd;

	cmd = ft_calloc_protec(1, sizeof(t_cmd_line));
	cmd->cmds = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return cmd;
}

static void add_arg(t_cmd_line *cmd, char *word)
{
	int i;
	int len;
	char **new;

	if (!cmd || !word)
		return;

	len = 0;
	if (cmd->cmds)
		while (cmd->cmds[len])
			len++;

	new = malloc(sizeof(char *) * (len + 2));
	if (!new)
		return;

	i = 0;
	while (i < len)
	{
		new[i] = cmd->cmds[i];
		i++;
	}

	new[i] = ft_strdup(word);
	new[i + 1] = NULL;

	free(cmd->cmds);
	cmd->cmds = new;
}

static void	add_redir(t_cmd_line *cmd, t_token **tokens)
{
	t_redirects	*r;

	if (!cmd || !*tokens || (*tokens)->type != REDIR)
		return ;

	r = ft_calloc_protec(1, sizeof(t_redirects));
	r->type = (*tokens)->redir;

	*tokens = (*tokens)->next;          // move to filename

	if (*tokens && (*tokens)->type == WORD)
	{
		if (r->type == HEREDOC)
			r->delimiter = ft_strdup((*tokens)->value);
		else
			r->filename = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;      // consume the filename
	}

	r->next = cmd->redir;
	cmd->redir = r;
}

t_cmd_line	*parse_tokens(t_token *tokens)
{
	t_cmd_line	*head;
	t_cmd_line	*cur;

	head = new_cmd();
	cur = head;

	while (tokens)
	{
		if (tokens->type == WORD)
		{
			add_arg(cur, tokens->value);
			tokens = tokens->next;
		}
		else if (tokens->type == REDIR)
		{
			add_redir(cur, &tokens);
			continue;                   // tokens already moved inside add_redir
		}
		else if (tokens->type == CTRL_OP && tokens->ctrlop == PIPE)
		{
			cur->next = new_cmd();
			cur = cur->next;
			tokens = tokens->next;
		}
		else
		{
			tokens = tokens->next;
		}
	}
	return (head);
}

static char	*input_strs_join(char *input_str, char *extra_input)
{
	char	*tmp;
	char	*full_input;

	tmp = ft_strjoin(input_str, " ");
	if (!tmp)
		return (NULL);
	full_input = ft_strjoin(tmp, extra_input);
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

static void	read_eval_print_loop(t_shelly *shelly)
{
	char	*input_str;
	t_token	*tokens;
	t_cmd_line *cmds;
	// char	**tokens; // ft_split(mini_av, ' ' or ft_isspace())

	sig_mode(INTERACTIVE);
	while (1)
	{
		input_str = put_prompt(shelly, "shelly");
		if (!input_str)
			exit_cleanup(EXIT_SUCCESS, shelly);
		if (*input_str)
		{
			input_str = validate_complete_input(input_str, shelly);
			if (input_str)
			{
				add_history(input_str);
				tokens = tokenize_input(input_str);
				cmds = parse_tokens(tokens);
				exec_loop(cmds, shelly);
			}
		}
		free(input_str);
	}
}

static void	non_interactive_mode(t_shelly *shelly)
{
	char	*line;
	t_token	*tokens;
	t_cmd_line	*cmds;
	
	// set_signals_noninteractive();
	sig_mode(CHILD);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		tokens = tokenize_input(line);
		cmds = parse_tokens(tokens);
		exec_loop(cmds, shelly);
		// TODO: tokenize + expand + execute here
		free(line);
	}
}

t_shelly	*init(char **envp)
{
	t_shelly	*shelly;

	shelly = set_shellyenvp(envp);
	// TODO: initialise other elements of *shelly
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		read_eval_print_loop(shelly);
	else
		non_interactive_mode(shelly);
		// TODO: does this mode not need shelly_envp?
		// Read line from stdin
		// Parse + execute
		// Repeat until EOF
		// Exit with proper status
	return (shelly);
}
