/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/04/01 20:19:13 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

int g_signal = 0;

typedef enum e_fds
{
	STDIN,
	STDOUT,
}   t_fds;

typedef enum e_builts
{
	BUILTINS,
	NON_BUILTINS
}	t_builtins_check;

typedef enum e_redir_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC,
	FD,
}	t_redir_type;

typedef struct s_redirections
{
	t_redir_type	type;
	char	*filename;
    char    *delimiter;
	struct s_redirections	*next;
}   t_redirects;

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
char 	*paths_helper(t_pipe *cmd_line, char **envp, int i);
char 	*absolute_path(t_pipe *cmd_line);
int 	free_path(char **paths);

// FORKs and SIGNALs
int		exec(t_pipe *cmd_line, char **envp, int status);
void	child_exec(char *cmd, t_pipe *cmd_line, char **envp);
void	signal_handler(pid_t pid);
int		status_check(int status);
int		are_you_builtin(t_pipe *cmd_line);

// PIPEs //
/* void 	pipe_handler(t_pipe *cmd_line);
void 	heredoc(t_pipe *cmd_line);
void append(t_pipe *cmd_line); */

void heredoc(t_redirects *heredoc);
void signal_heredoc(t_redirects *heredoc_signals);
void append(t_redirects *append);
void pipe_handler(t_redirects *in_or_out);


# endif

