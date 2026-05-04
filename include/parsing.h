/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/05/04 18:23:58 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* Our libs */
# include "minishell.h"

// TODO: review std libs that should be general =>> minishell.h
/* Standard libs */
# include <sys/wait.h>
# include <signal.h>
// TODO: Review why I (might) need this & if PATH_MAX must be replaced (Norme?):
# include <linux/limits.h>

/* Structs */
// TODO: create separate header?

// TODO: use here or in execution.h?
typedef enum e_token_type
{
	WORD,
	REDIR,
	CTRL_OP
	// ASSIGN, // VAR=value before command
	// TK_EOF, TODO: include?
}	t_token_type;

/*
PIPE // |
OR, // ||, bonus
AND, // &&, bonus
LFT_PAREN, // (, bonus
RGT_PAREN, // ), bonus
*/
typedef enum e_ctrlop_type
{
	PIPE
}	t_ctrlop_type;

typedef enum e_word_type
{
	CMD,
	QMARK1,
	QMARK2
}	t_word_type;

typedef struct s_token
{
	t_token_type	type;
	t_ctrlop_type	ctrlop; // Valid only if type == CTRL_OP
	t_redir_type	redir; // Valid only if type == REDIR
	int				index;
	char			*value; // For TK_WORD, filename, delimiter, etc.
	int				quoted; // 1 if token had quotes TODO: optional but useful?
	struct s_token	*previous; // TODO: review if necessary?	
	struct s_token	*next;
}	t_token;

// TODO: use here or in execution.h ? Milena check struct_notes.md
// one redirection node. This represents one redirection attached to a command (like < infile, >> out, << EOF).

// TODO: use here or in execution.h ? Milena check struct_notes.md pls
// one command in a pipeline. This represents a single simple command, with args + its redirections.
// TODO: use here or in execution.h ?
// typedef struct s_pipe
// {
// 	char			**cmds;
// 	t_redir			redir;
// 	struct s_pipe	*next;
// }	t_pipe;

// TODO: review functions that should be general =>> minishell.h & =/= files
/* Wrapper functions - maybe Protected std functions? */
char		*getcwd_protec(char *buf, size_t size, t_shelly *shelly);
void		*malloc_protec(size_t size, t_shelly *shelly);
void		*ft_calloc_protec(size_t nmemb, size_t size);

/* Parsing functions */
t_shelly	*init(char **envp);
char		*put_prompt(t_shelly *shelly, char *prompt);
t_shelly	*set_shellyenvp(char **envp);
char		*find_var_shellyenvp(t_shelly *shelly, char *envp_var_key);
// void	read_eval_print_loop(char **envp); // currently static
// void	non_interactive_mode(void); // currently static
// void	set_sigaction(int signo, void (*handler)(int), int flags); // currently static
// void	sigint_prompt_handler(int signal); // currently static
void		set_signals_interactive_parent(void);
void		set_signals_noninteractive(void);

/* Tokenizing functions */
t_token		*tokenize_input(char *input_str);

/* Tokenizing utils */
int			ft_isspace(int c);
char		quote_check(char *str);
char		*syntax_err_check(char *trimmed_input);


#endif