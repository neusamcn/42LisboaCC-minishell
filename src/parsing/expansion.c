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