
static char	*smash_key_val(char *k, char *v)
{
	char	*smashed;

	smashed = ft_strjoin(k, "=");
	if (v)
		smashed = ft_strjoin_free(smashed, v);
	return (smashed);
}

static int	where_is_it(char **envp, char *k)
{
	int		pos;
	size_t	klen;

	pos = -1;
	klen = ft_strlen(k);
	while (envp[++pos])
		if (!ft_strncmp(envp[pos], k, klen) && envp[pos][klen] == '=')
			return (pos);
	return (-1);
}

static char	**swap_it(char **envp, int pos, char *smashed)
{
	free(envp[pos]);
	envp[pos] = smashed;
	return (envp);
}

static char	**shove_it_in(char **envp, char *smashed, int sz)
{
	char	**newenvp;

	newenvp = malloc(sizeof(char *) * (sz + 2));
	if (!newenvp)
		return (free(smashed), envp);
	newenvp[sz] = smashed;
	newenvp[sz + 1] = NULL;
	while (sz--)
		newenvp[sz] = envp[sz];
	return (free(envp), newenvp);
}

char	**exp_minienv(t_export *mini, char *k, char *v, int sz)
{
	char	*smashed;
	int		pos;

	smashed = smash_key_val(k, v);
	pos = where_is_it(mini->envp, k);
	if (pos != -1)
		return (swap_it(mini->envp, pos, smashed));
	if (sz == -1)
	{
		sz = 0;
		while (mini->envp[sz])
			sz++;
	}
	return (shove_it_in(mini->envp, smashed, sz));
}

char	**exp_var(t_export *mini, char *k)
{
	if (where_is_it(mini->envp, k) != -1)
		return (mini->envp);
	return (exp_minienv(mini, k, NULL, -1));
}
