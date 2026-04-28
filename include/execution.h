/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncruz-ne <ncruz-ne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/04/19 20:52:58 by ncruz-ne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

# include "minishell.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>

# define p(...) printf(__VA_ARGS__)
# define CD "cd"
# define ECHO "echo"
# define EXIT "exit"
# define PWD "pwd"
# define ENV "env"
# define EXPORT "export"
# define UNSET "unset"
# define DELIMETER "delimeter"
# define TRUE 0
# define FALSE 1
# define HD "minishell: warning: here-document delimited by end-of-file (wanted '"

// TODO: needed? or use default macros?
typedef enum e_fds
{
	STDIN,
	STDOUT
}   t_fds;

typedef enum e_builts
{
	BUILTINS,
	NON_BUILTINS
}	t_builtins_check;

// TODO: use here or in parsing.h ?
typedef enum e_types_of_redirections
{
	IN,  // < redir input to a cmd, taking input from a file
	OUT, // > redir output to a file, and overwrites the file if it already exists
	APPEND, // >> redir output top a file, append the output to the end of the file
	HEREDOC, // <<
	FD // TODO: should be somewhere else since it's not a redir type
}	t_redir_type;

// TODO: necessary? or use default macros?
typedef enum e_signal_types
{
	SIG_INT, // ctrl+c
	SIG_QUIT // ctrl \+
}	e_signal_types;

// TODO: use here or in parsing.h ? Milena check struct_notes.md
typedef struct s_redirections
{
	t_redir_type			type;
	char					*filename;
    char    				*delimiter;
	struct s_redirections	*next;
}   t_redirects;

// TODO: use here or in parsing.h ?
typedef struct s_pipe
{
    char            **cmds;
    t_redirects     redir;
    struct s_pipe   *next;
}   t_pipe;

/* typedef struct s_execution
{
	t_builtin_cmd		builtins[12];
	char				*cmd_name;
	int					*pid;
}	t_execution; */

// [ls] → [grep src] → [wc -l] → NULL 

// PATH //
char 	*path(t_pipe *cmd_line, char **envp);
char 	*paths_helper(t_pipe *cmd_line, char *path_var);
char 	*absolute_path(t_pipe *cmd_line);
int 	free_path(char **paths);

// FORKs // 
int		exec(t_pipe *cmd_line, char **envp, int status);
void	child_exec(char *cmd, t_pipe *cmd_line, char **envp);
void	parent_exec(int status, pid_t pid);
int		status_check(int status);
int		are_you_builtin(t_pipe *cmd_line);

// SIGNALs //
void		sigint_glob(int sig);
int			get_signal_stat(void);
void		set_signal_stat(int value);
void		set_signals_interactive_parent(void);
void		set_signals_noninteractive(void);

// PIPEs and REDIRECTIONs//
void 	heredoc(t_redirects *redir);
void 	append(t_redirects *append);
void 	append(t_redirects *redir);
void 	pipe_handler(t_redirects *redir);

//TO DELETE FOR PARSER
void	print_err_msg(char *my_msg);
void	p_log_err(char *cmd, char *msg);
//void	exit_cleanup(int exit_status, t_minishell *minishell);
char	*getcwd_protec(char *buf, size_t size); // TODO: review, repeated from parsing.h
int		ft_arrlen(char **arr);
t_pipe 	*fake_parse(char *line);

# endif
	
	