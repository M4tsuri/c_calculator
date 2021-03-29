#include "project.h"

Value val_div(Value *lhs, Value *rhs) {
    Value res;
    if (lhs->type == VAL_BOT || rhs->type == VAL_BOT) {
        res.type = VAL_BOT;
    }

    if (lhs->type == VAL_TOP || rhs->type == VAL_TOP) {
        res.type = VAL_TOP;
    }

    if (lhs->type == VAL_FLOAT || rhs->type == VAL_FLOAT) {
        res.type = VAL_FLOAT;

        float lval = lhs->content.float_val;
        float rval = rhs->content.float_val;
        if (lhs->type == VAL_INTEGER) {
            lval = (long double)lhs->content.int_val;
        }
        if (rhs->type == VAL_INTEGER) {
            rval = (long double)rhs->content.int_val;
        }

        if (rval == 0.0) {
            res.type = VAL_TOP;
            return res;
        }

        res.content.float_val = lval / rval;
        return res;
    } 

    if (rhs->content.int_val == 0) {
        res.type = VAL_TOP;
        return res;
    }
    res.type = VAL_INTEGER;
    res.content.int_val = lhs->content.int_val / rhs->content.int_val;
    return res;
}

Value val_sub(Value *lhs, Value *rhs) {
    Value res;
    if (lhs->type == VAL_BOT || rhs->type == VAL_BOT) {
        res.type = VAL_BOT;
    }

    if (lhs->type == VAL_TOP || rhs->type == VAL_TOP) {
        res.type = VAL_TOP;
    }

    if (lhs->type == VAL_FLOAT || rhs->type == VAL_FLOAT) {
        res.type = VAL_FLOAT;

        float lval = lhs->content.float_val;
        float rval = rhs->content.float_val;
        if (lhs->type == VAL_INTEGER) {
            lval = (long double)lhs->content.int_val;
        }
        if (rhs->type == VAL_INTEGER) {
            rval = (long double)rhs->content.int_val;
        }

        res.content.float_val = lval - rval;
        return res;
    } 

    res.type = VAL_INTEGER;
    res.content.int_val = lhs->content.int_val - rhs->content.int_val;
    return res;
}

Value val_mul(Value *lhs, Value *rhs) {
    Value res;
    if (lhs->type == VAL_BOT || rhs->type == VAL_BOT) {
        res.type = VAL_BOT;
    }

    if (lhs->type == VAL_TOP || rhs->type == VAL_TOP) {
        res.type = VAL_TOP;
    }

    if (lhs->type == VAL_FLOAT || rhs->type == VAL_FLOAT) {
        res.type = VAL_FLOAT;

        float lval = lhs->content.float_val;
        float rval = rhs->content.float_val;
        if (lhs->type == VAL_INTEGER) {
            lval = (long double)lhs->content.int_val;
        }
        if (rhs->type == VAL_INTEGER) {
            rval = (long double)rhs->content.int_val;
        }

        res.content.float_val = lval * rval;
        return res;
    } 

    res.type = VAL_INTEGER;
    res.content.int_val = lhs->content.int_val * rhs->content.int_val;
    return res;
}

Value val_add(Value *lhs, Value *rhs) {
    Value res;
    if (lhs->type == VAL_BOT || rhs->type == VAL_BOT) {
        res.type = VAL_BOT;
    }

    if (lhs->type == VAL_TOP || rhs->type == VAL_TOP) {
        res.type = VAL_TOP;
    }

    if (lhs->type == VAL_FLOAT || rhs->type == VAL_FLOAT) {
        res.type = VAL_FLOAT;

        float lval = lhs->content.float_val;
        float rval = rhs->content.float_val;
        if (lhs->type == VAL_INTEGER) {
            lval = (long double)lhs->content.int_val;
        }
        if (rhs->type == VAL_INTEGER) {
            rval = (long double)rhs->content.int_val;
        }

        res.content.float_val = lval + rval;
        return res;
    } 

    res.type = VAL_INTEGER;
    res.content.int_val = lhs->content.int_val + rhs->content.int_val;
    return res;
}

Value exec_binop(BinOpType op, Value *lhs, Value *rhs) {
    switch (op) {
        case BINOP_ADD:
            return val_add(lhs, rhs);
        case BINOP_DIV:
            return val_div(lhs, rhs);
        case BINOP_MUL:
            return val_mul(lhs, rhs);
        case BINOP_SUB:
            return val_sub(lhs, rhs);
    }
}

Value val_neg(Value *oprand) {
    Value val = *oprand;
    if (val.type == VAL_FLOAT) {
        val.content.float_val = -val.content.float_val;
    } else if (val.type == VAL_INTEGER) {
        val.content.int_val = -val.content.int_val;
    }

    return val;
}

Value exec_unop(UnOpType op, Value *lhs) {
    switch (op) {
        case UNOP_NEG:
            return val_neg(lhs);
        default:
            return *lhs;
    }
}


Value exec_expr(Project *proj, Expr *expr) {
    switch (expr->type) {
        case EXPR_BINARY: {
            Value lhs = exec_expr(proj, expr->content.bin_expr.lhs);
            Value rhs = exec_expr(proj, expr->content.bin_expr.rhs);
            return exec_binop(expr->content.bin_expr.op, &lhs, &rhs);
        }
        case EXPR_PARENTHESES:
            return exec_expr(proj, expr->content.paren_expr.inside_expr);
        case EXPR_SYMBOL:
            return symtab_get(proj->symtab, expr->content.sym_idx)->content.value;
        case EXPR_UNARY: {
            Value lhs = exec_expr(proj, expr->content.unary_expr.oprand);
            return exec_unop(expr->content.unary_expr.op, &lhs);
        }
        case EXPR_VALUE:
            return expr->content.value;
    }
} 

void write_val(Project *proj, Value *val) {
    switch (val->type) {
        case VAL_BOT:
            fprintf(proj->output_file, "undefine<met uninitialized variable>\n");
            break;
        case VAL_TOP:
            fprintf(proj->output_file, "undefine<met invalid expression>\n");
            break;
        case VAL_FLOAT:
            fprintf(proj->output_file, "%Lf\n", val->content.float_val);
            break;
        case VAL_INTEGER:
            fprintf(proj->output_file, "%lld\n", val->content.int_val);
            break;
    }
}

void exec_ast(Project *proj) {
    Statement *s;
    FOR_EACH(s, proj->program->statements) {
        switch (s->type) {
            case STAT_ASSIGN: {
                Value val = exec_expr(proj, s->content.assign.src);
                symtab_assign(proj->symtab, s->content.assign.dest_idx, &val);
                break;
            }
            case STAT_CALL: {
                Value val = exec_expr(proj, s->content.call.arg);
                write_val(proj, &val);
                break;
            }
            default:
                break;
        }
    }
}