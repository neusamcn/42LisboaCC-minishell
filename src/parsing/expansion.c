/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 20:06:59 by megi              #+#    #+#             */
/*   Updated: 2026/05/09 20:11:07 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
    t_token	*tokenize_input(char *input_str)
*/

/* 
    parser() {
        1. tokenise
            - token is variable depending on quote state and variable name being valid 
            E.G. $?, $_, $<alphanumeric>, $! check subject 
        2. sytax check
        3. convert to command pipeline 
            - expand if variable
                - token->val: $var
                - token->exp = true/false
                - token->quoted = true/false
    }

    1. Happens building command pipeline. Expand the token to when converting to the cmd pipeline
        export var = "Milena"
        echo "$var" -> Milena -> exp = true
        echo '$var' -> $var -> exp = false
        'joh"n"' -> joh"n"

    2. Heredoc after command pipeline is built / during execution
        cat << del
        > $var
        > del
        Milena
        
        cat << del
        > "$var"
        > del
        "Milena"

        cat << "del" / ""del / del"" / "d"el
        > $var
        > del
        $var
*/


/* $VAR → replace with env value
$? → replace with last exit status
~ → replace with HOME (optional/bonus)
"$VAR" → expand inside double quotes
'$VAR' → do NOT expand inside single quotes */

/* "hello $USER world"
         ↓
scan char by char:
  - regular char → copy as-is
  - ' → copy everything until next ' literally
  - " → copy until next " BUT expand $ inside
  - $ → look ahead, grab var name, look up in envp, substitute */

  char    *expand_token(char *word, t_shelly *shelly)
{
    // scan word char by char
    // build result string
    // when you hit $:
    //   grab name (alphanumeric + _)
    //   look up in shelly->envp
    //   append value (or "" if not found)
    // when you hit ':
    //   copy everything until next ' literally (no expansion)
    // when you hit ":
    //   copy until next " but still expand $
    // return result
}