#include "execution.h"
#include "minishell.h"

#include "../include/minishell.h"

static int ft_arrlen(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
        i++;
    return (i);
}

t_cmd_line *fake_parse(char *line)
{
    t_cmd_line  *p;
    char        **tokens;
    int         i;
    int         cmd_i;

    p = malloc(sizeof(t_cmd_line));
    if (!p)
        return NULL;
    ft_memset(p, 0, sizeof(t_cmd_line));
    tokens = ft_split(line, ' ');
    if (!tokens)
        return p;
    p->cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
    if (!p->cmds)
        return NULL;
    cmd_i = 0;
    i = 0;
    while (tokens[i])
    {
        if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
            { p->redir.type = OUT; p->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
            { p->redir.type = APPEND; p->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
            { p->redir.type = IN; p->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
            { p->redir.type = HEREDOC; p->redir.delimiter = tokens[++i]; }
        else
            p->cmds[cmd_i++] = tokens[i];
        i++;
    }
    p->cmds[cmd_i] = NULL;
    return (p);
}

int	main(int ac, char **av, char **envp)
{
	char	*prompt;
	t_cmd_line	*cmd_line;
	int		last_status;

	(void)ac;
	(void)av;
	if (!envp)
		return 0;
	last_status = 0;
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		if (*prompt)
			add_history(prompt);
		cmd_line = fake_parse(prompt);
		if (!cmd_line)
		{
			free(prompt);
			continue ;
		}
		last_status = exec(cmd_line, envp, last_status);
		free(prompt);
	}
	return 0;
}


