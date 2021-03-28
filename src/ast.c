/**
 * generate an AST for given tokens
 * we use a LL(2) grammar here
 */

#include "ast.h"

bool gen_expr(Project *proj, Expr **expr_ptr);
bool gen_unexpr(Project *proj, Expr *expr);
bool gen_parenexpr(Project *proj, Expr *expr);

bool gen_expr(Project *proj, Expr **expr_ptr) {
    Token *cur = pool_peek(proj->tokens);
    Expr *may_lhs = pool_use(proj->program->exprs);

    switch (cur->type) {
        case TOKEN_UNOP:
            gen_unexpr(proj, may_lhs);
            break;
        case TOKEN_LPAREN:
            gen_parenexpr(proj, may_lhs);
            break;
        case TOKEN_NUMBER:
            pool_next(proj->tokens);
            may_lhs->type = EXPR_VALUE;
            may_lhs->content.value.type = VAL_FLOAT;
            may_lhs->content.value.content.float_val = cur->content.number;
            break;
        case TOKEN_RSYMBOL:
            pool_next(proj->tokens);
            may_lhs->type = EXPR_SYMBOL;
            may_lhs->content.name_idx = cur->content.name_idx;
            break;
        default:
            panic(cur->line, "internal error: token corrupted when parsing expression.");
    }

    Token *may_op = pool_peek(proj->tokens);
    
    if (may_op->type == TOKEN_BINOP) {
        Expr *expr = pool_use(proj->program->exprs);
        *expr_ptr = expr;
        pool_next(proj->tokens);
        expr->type = EXPR_BINARY;
        expr->content.bin_expr.op = may_op->content.bin_op;
        expr->content.bin_expr.lhs = may_lhs;
        
        gen_expr(proj, &expr->content.bin_expr.rhs);
        expr->content.bin_expr.rhs->parent = expr;
    } else {
        Expr *expr = may_lhs;
        *expr_ptr = expr;
        expr->type = may_lhs->type;
        expr->content = may_lhs->content;
    }
    
    return true;
}

bool gen_unexpr(Project *proj, Expr *expr) {
    Token *cur = pool_next(proj->tokens);

    expr->content.unary_expr.op = cur->content.un_op;
    gen_expr(proj, &expr->content.unary_expr.oprand);
    expr->content.unary_expr.oprand->parent = expr;
    return true;
}

bool gen_parenexpr(Project *proj, Expr *expr) {
    // eat '('
    pool_next(proj->tokens);
    expr->type = EXPR_PARENTHESES;
    gen_expr(proj, &expr->content.paren_expr.inside_expr);
    expr->content.paren_expr.inside_expr->parent = expr;
    // eat ')'
    pool_next(proj->tokens);
    return true;
}

bool gen_decl(Project *proj, Declaration *decl) {
    Token *key = pool_next(proj->tokens);
    Token *name = pool_next(proj->tokens);

    // TODO: do we need a check here?
    decl->type = key->content.decl;
    decl->name_idx = name->content.name_idx;

    return true;
}

bool gen_assign(Project *proj, Assignment *assign) {
    Token *target = pool_next(proj->tokens);
    // eat '='
    pool_next(proj->tokens);

    assign->dest_idx = target->content.name_idx;
    return gen_expr(proj, &assign->src);
}

bool gen_call(Project *proj, ProcedureCall *call) {
    Token *func = pool_next(proj->tokens);

    call->name_idx = func->content.name_idx;
    return gen_expr(proj, &call->arg);
}

bool gen_stat(Project *proj, Statement *stat) {
    Token *cur = pool_peek(proj->tokens);
    Token *next = pool_peek2(proj->tokens);

    if (cur->type == TOKEN_DECL) {
        stat->type = STAT_DECL;
        gen_decl(proj, &stat->content.decl);
    } else if (next->type == TOKEN_ASSIGN) {
        stat->type = STAT_ASSIGN;
        gen_assign(proj, &stat->content.assign);
    } else if (next->type == TOKEN_LPAREN) {
        stat->type = STAT_CALL;
        gen_call(proj, &stat->content.call);
    }

    // eat ';' or '.'
    Token *end = pool_next(proj->tokens);
    if (end->type == TOKEN_END) {
        return false;
    } else if (end->type == TOKEN_SEMICOLON) {
        return true;
    }
    panic(end->line, "internal error: token corrupted when generating AST.");
}

bool binop_primary(BinOpType a, BinOpType b) {
    switch (a) {
        case BINOP_DIV:
        case BINOP_MUL:
            return true;
        default:
            return false;
    }
}

void rotate_tree(Expr *e) {
    switch (e->type) {
        case EXPR_SYMBOL:
        case EXPR_VALUE:
            return;
        case EXPR_PARENTHESES:
            rotate_tree(e->content.paren_expr.inside_expr);
            return;
        case EXPR_UNARY:
            rotate_tree(e->content.unary_expr.oprand);
            return;
        case EXPR_BINARY: {
            Expr *lhs = e->content.bin_expr.lhs;
            Expr *rhs = e->content.bin_expr.rhs;
            if (lhs->type == EXPR_BINARY) {
                rotate_tree(lhs->content.bin_expr.rhs);
            } else {
                rotate_tree(lhs);
            }
            
            Expr *parent = e->parent;

            // this node cannot be rotated
            if (parent == NULL || parent->type == EXPR_PARENTHESES) {
                rotate_tree(rhs);
                return;
            }

            Expr *grand = parent->parent;

            if (parent->type == EXPR_UNARY) {
                e->content.bin_expr.lhs = parent;
                parent->content.unary_expr.oprand = lhs;
                parent->parent = e;
                e->parent = grand;
                if (grand != NULL) {
                    if (grand->type != EXPR_PARENTHESES) {
                        panic(0, "fuck your unary operator.\n");
                    }
                    grand->content.paren_expr.inside_expr = e;
                }
            } else if (parent->type == EXPR_BINARY && 
                binop_primary(parent->content.bin_expr.op, e->content.bin_expr.op)) {

                if (parent->content.bin_expr.rhs != e) {
                    panic(0, "error when rotating AST.");
                }

                // do the rotate, take care of parent relation
                parent->content.bin_expr.rhs = lhs;
                e->content.bin_expr.lhs = parent;
                parent->parent = e;
                e->parent = grand;

                if (grand != NULL) {
                    if (grand->type == EXPR_PARENTHESES) {
                        grand->content.paren_expr.inside_expr = e;
                    } else if (grand->type == EXPR_BINARY) {
                        if (grand->content.bin_expr.rhs == parent) {
                            grand->content.bin_expr.rhs = e;
                        } else {
                            grand->content.bin_expr.lhs = e;
                        }
                    } else {
                        panic(0, "what fuck.");
                    }
                }
            }
            rotate_tree(rhs);
            return;
        }
        default:
            panic(0, "corrupted expression type when rotating AST.");
    } 
}

Expr *get_expr_root(Expr *cur) {
    Expr *parent = cur;
    while (parent->parent != NULL) {
        parent = parent->parent;
    }
    return parent;
}

/**
 * an expression like 1 * 2 + 3 * 4 will be interpretered as
 * 1 * (2 + (3 * 4)), which is not appreciated
 * In this function, we rotate each tree of expression
 * a tree like
 *    *
 *  /  \
 * 1    +
 *    /  \
 *   2    *
 *      /  \
 *     3    4
 * will be rotated into
 *         +
 *       /  \
 *     /     \
 *    *      *
 *  /  \   /  \
 * 4    3 1    2
 * 
 * we will recursively apply this retation on each sub expression to fix the precedence
 */
void resolve_precedence(Project *proj) {
    Statement *s = NULL;
    FOR_EACH(s, proj->program->statements) {
        Expr *cur = NULL;
        if (s->type == STAT_ASSIGN) {
            cur = s->content.assign.src;
            rotate_tree(cur);
            s->content.assign.src = get_expr_root(cur);
        } else if (s->type == STAT_CALL) {
            cur = s->content.call.arg;
            rotate_tree(s->content.call.arg);
            s->content.call.arg = get_expr_root(cur);
        }
        
    }
    reset_iter(proj->program->statements);
}

void gen_program(Project *proj) {
    proj->program->statements = create_pool(sizeof(Statement));
    proj->program->exprs = create_pool(sizeof(Expr));

    while (gen_stat(proj, pool_use(proj->program->statements)));
    resolve_precedence(proj);
    reset_iter(proj->tokens);
}

void pretty_print_unop(UnOpType op) {
    switch (op) {
        case UNOP_NEG:
            printf("-");
            break;
        case UNOP_POS:
            printf("+");
            break;
    }
}

void pretty_print_binop(BinOpType op) {
    switch (op) {
        case BINOP_ADD:
            printf("+");
            break;
        case BINOP_DIV:
            printf("/");
            break;
        case BINOP_MUL:
            printf("*");
            break;
        case BINOP_SUB:
            printf("-");
            break;
    }
}

void pretty_print_ast(StringTable *strtab, Expr *expr) {
    switch (expr->type) {
        case EXPR_BINARY:
            pretty_print_binop(expr->content.bin_expr.op);
            printf(" (");
            pretty_print_ast(strtab, expr->content.bin_expr.lhs);
            pretty_print_ast(strtab, expr->content.bin_expr.rhs);
            printf(") ");
            break;
        case EXPR_PARENTHESES:
            printf(" (");
            pretty_print_ast(strtab, expr->content.paren_expr.inside_expr);
            printf(") ");
            break;
        case EXPR_SYMBOL:
            printf(" %s ", strtab_get(strtab, expr->content.name_idx));
            break;
        case EXPR_UNARY:
            pretty_print_unop(expr->content.unary_expr.op);
            pretty_print_ast(strtab, expr->content.unary_expr.oprand);
            break;
        case EXPR_VALUE:
            printf(" %Lf ", expr->content.value.content.float_val);
            break;
    }
} 
