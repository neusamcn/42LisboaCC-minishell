#include "../../include/minishell.h"

//FILE TO DELETE
void	exit_cleanup(int exit_status, t_minishell *minishell)
{
	int	i;

	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	rl_clear_history();
	i = 0;
	while (minishell->minienvp[i])
		free(minishell->minienvp[i++]);
	free(minishell->minienvp);
	// TODO: function to close all previously opened fd?
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// TODO: can i close stderr before sending an exit status?
	exit(exit_status);
}

char	*getcwd_protec(char *buf, size_t size)
{
	if (!getcwd(buf, size))
	{
		print_err_msg("getcwd failed");
		// TODO: cleanup function instead of exit
		exit_cleanup();
		exit(EXIT_FAILURE);
	}
	return (buf);
}

// TO DELETE
int ft_arrlen(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
        i++;
    return (i);
}

//TO DELETEt_pipe *
fake_parse(char *line)
{
    t_pipe      *p;
    char        **tokens;
    char        **cmds;
    int         i;
    int         cmd_i;

    p = malloc(sizeof(t_pipe));
    if (!p)
        return NULL;
    ft_memset(p, 0, sizeof(t_pipe));
    tokens = ft_split(line, ' ');
    if (!tokens)
        return p;
    cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
    if (!cmds)
    {
        free(p);
        return NULL;
    }
    cmd_i = 0;
    i = 0;
    while (tokens[i])
    {
        if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
        {
            p->redir.type = OUT;
            p->redir.filename = tokens[++i];
        }
        else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
        {
            p->redir.type = APPEND;
            p->redir.filename = tokens[++i];
        }
        else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
        {
            p->redir.type = IN;
            p->redir.filename = tokens[++i];
        }
        else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
        {
            p->redir.type = HEREDOC;
            p->redir.delimiter = tokens[++i];
        }
        else
            cmds[cmd_i++] = tokens[i];
        i++;
    }
    cmds[cmd_i] = NULL;
    p->cmds = cmds;
    return (p);
}