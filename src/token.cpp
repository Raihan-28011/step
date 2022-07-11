//
// Created by raihan on 7/10/22.
//

#include "token.h"

std::array<string, step::TokenKind::t_total> const step::Token::tok_s {
        "t_def",
        "t_print",
        "t_input",
        "t_true",
        "t_false",
        "t_null",
        "t_ident",
        "t_string",
        "t_num",
        "t_equal",
        "t_semicolon",
        "t_comma",
        "t_dot",
        "t_lparen",
        "t_rparen",
        "t_lbrace",
        "t_rbrace",
        "t_lsqbrace",
        "t_rsqbrace",
        "t_plus",
        "t_minus",
        "t_star",
        "t_slash",
        "t_modulus",
        "t_equalequal",
        "t_notequal",
        "t_bang",
        "t_less",
        "t_greater",
        "t_lessequal",
        "t_greaterequal",
        "t_logicaland",
        "t_logicalor",
};
