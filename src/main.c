/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:47:13 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/18 18:40:57 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../include/minishell.h"
#include "execution.h"
#include "../include/parsing.h"
#include "minishell.h"

static t_cmd_line *new_cmd(void)
{
	t_cmd_line *cmd = ft_calloc_protec(1, sizeof(t_cmd_line));
	cmd->cmds = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return cmd;
}

static void	add_arg(t_cmd_line *cmd, char *word)
{
	int i = 0;
	int len = 0;
	char **new;

	if (!cmd->cmds)
	{
		cmd->cmds = malloc(sizeof(char *) * 2);
		cmd->cmds[0] = ft_strdup(word);
		cmd->cmds[1] = NULL;
		return;
	}

	while (cmd->cmds[len])
		len++;

	new = malloc(sizeof(char *) * (len + 2));
	while (i < len)
	{
		new[i] = cmd->cmds[i];
		i++;
	}
	new[i++] = ft_strdup(word);
	new[i] = NULL;

	free(cmd->cmds);
	cmd->cmds = new;
}

static void	add_redir(t_cmd_line *cmd, t_token *t, t_token **tokens)
{
	t_redirects *r = ft_calloc_protec(1, sizeof(t_redirects));

	r->type = t->redir;

	*tokens = (*tokens)->next;

	if (*tokens && (*tokens)->type == WORD)
	{
		if (r->type == HEREDOC)
			r->delimiter = ft_strdup((*tokens)->value);
		else
			r->filename = ft_strdup((*tokens)->value);
	}

	r->next = cmd->redir;
	cmd->redir = r;
}

t_cmd_line *parse_tokens(t_token *tokens)
{
	t_cmd_line *head = new_cmd();
	t_cmd_line *cur = head;

	while (tokens)
	{
		if (tokens->type == WORD)
		{
			add_arg(cur, tokens->value);
		}
		else if (tokens->type == REDIR)
		{
			add_redir(cur, tokens, &tokens);
		}
		else if (tokens->type == CTRL_OP && tokens->ctrlop == PIPE)
		{
			cur->next = new_cmd();
			cur = cur->next;
		}

		tokens = tokens->next;
	}

	return head;
}

void	exit_cleanup(int exit_status, t_shelly *shelly)
{
	int	i;

	ft_putendl_fd("exit", STDOUT_FILENO);
	rl_clear_history();
	i = 0;
	while (shelly->envp[i])
		free(shelly->envp[i++]);
	free(shelly->envp);
	free(shelly);
	exit(exit_status);
}

int	main(int ac, char **av, char **envp)
{
	t_shelly	*shelly;

	if (ac != 1 || av[1])
	{
		// TODO: print_err_args_msg() ?
		ft_putendl_fd("Invalid arguments to run Minishell. Try "
			ORCHID"./minishell"COLOR_RESET" at "
			ORCHID"dir /minishell"COLOR_RESET" root.", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	shelly = init(envp);
	// TODO: add **av/*av/av[0] as arg?
	exit_cleanup(EXIT_SUCCESS, shelly);
}
