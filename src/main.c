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
    char        **tokens;
    t_cmd_line  *head;
    t_cmd_line  *current;
    int         i;
    int         cmd_i;

    tokens = ft_split(line, ' ');
    if (!tokens)
        return NULL;
    head = malloc(sizeof(t_cmd_line));
    ft_memset(head, 0, sizeof(t_cmd_line));
    head->cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
    current = head;
    cmd_i = 0;
    i = 0;
    while (tokens[i])
    {
        if (ft_strcmp(tokens[i], "|") == 0)
        {
            current->cmds[cmd_i] = NULL;  // завершаем текущую ноду
            current->next = malloc(sizeof(t_cmd_line)); // новая нода
            ft_memset(current->next, 0, sizeof(t_cmd_line));
            current = current->next;
            current->cmds = malloc(sizeof(char *) * (ft_arrlen(tokens) + 1));
            cmd_i = 0;
        }
        else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
            { current->redir.type = OUT; current->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
            { current->redir.type = APPEND; current->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
            { current->redir.type = IN; current->redir.filename = tokens[++i]; }
        else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
            { current->redir.type = HEREDOC; current->redir.delimiter = tokens[++i]; }
        else
            current->cmds[cmd_i++] = tokens[i];
        i++;
    }
    current->cmds[cmd_i] = NULL;
    return head;
}

int	main(int ac, char **av, char **envp)
{
	char	    *prompt;
	t_cmd_line	*cmd_line;
//	int		status;

	(void)ac;
	(void)av;
    if (!envp || !envp[0])
        write(1, "envp is NULL!\n", 14);
	//status = 0;
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
        exec_loop(cmd_line, envp);
		free(prompt);
	}
	return 0;
}


