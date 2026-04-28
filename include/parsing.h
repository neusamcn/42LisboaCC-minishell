/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 22:00:19 by ncruz-ne          #+#    #+#             */
/*   Updated: 2026/04/26 22:37:14 by ncruz-ne         ###   ########.fr       */
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

// TODO: use here or in minishell.h?
// typedef struct s_minishell
// {
// 	char	**minienvp;
// 	int		*open_fd;
// 	void	*malloc_ptrs;
// }	t_minishell;

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

// TODO: keep here or in execution.h ?
/*
IN, // < redir input to a cmd, taking input from a file
OUT, // > redir output to a file, and overwrites the file if it already exists
APPEND, // >> redir output top a file, append the output to the end of the file
HEREDOC, // <<
*/
typedef enum e_redir_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC
}	t_redir_type;

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
typedef struct s_redir
{
	t_redir_type	type; // IN (<), OUT (>), APPEND (>>), HEREDOC (<<)
	char			*filename; // Target file for <, >, >>. Usually NULL for heredoc.
	int				io; // fd being redirected (destination/source side), e.g.: default: 0 for input redirs, 1 for output redirs
	int				fd; // Runtime opened FD (result of open() or heredoc pipe read-end). Best practice: initialize to -1, set when opened, close during cleanup after dup2() / on error paths
	char			*delimiter; // Used only for heredoc (<<), e.g. EOF in cat << EOF. Usually NULL for non-heredoc.
	struct s_redir	*next; // Linked list of redirections in lexical order.
}	t_redir;

// TODO: use here or in execution.h ? Milena check struct_notes.md
// one command in a pipeline. This represents a single simple command, with args + its redirections.

typedef struct s_cmd
{
	char			**argv; // Command + arguments array for execution: argv[0] command name (ls, echo, etc.); argv[n] args; NULL-terminated for execve()
	t_redir			*redirs; // Head of that command’s redirection list (t_redir nodes).
	int				in_fd; // Final input FD chosen for this command at execution time. Typical defaults: STDIN_FILENO unless changed by pipe/redirection
	int				out_fd; // Final output FD chosen for this command at execution time. Typical defaults: STDOUT_FILENO unless changed by pipe/redirection
	struct s_cmd	*next; // Next command in pipeline (|). So a pipeline is a linked list of t_cmd.
}	t_cmd;

// TODO: use here or in execution.h ?
// typedef struct s_pipe
// {
// 	char			**cmds;
// 	t_redir			redir;
// 	struct s_pipe	*next;
// }	t_pipe;


// TODO: review functions that should be general =>> minishell.h & =/= files
/* Wrapper functions - maybe Protected std functions? */
char		*getcwd_protec(char *buf, size_t size, t_minishell *minishell);
void		*malloc_protec(size_t size, t_minishell *minishell);
void		*ft_calloc_protec(size_t nmemb, size_t size);

/* Parsing functions */
t_minishell	*init(char **envp);
char		*put_prompt(t_minishell *minishell, char *prompt);
t_minishell	*set_minienvp(char **envp);
// void	read_eval_print_loop(char **minienvp); // currently static
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