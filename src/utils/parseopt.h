#define MAX_OPT_NUM 8

#define CREATE_OPT(s, l, h) \\
while (0) { \\
    Opt *opt = (Opt *) malloc(sizeof(Opt)); \\
    opt->short_form = s; \\
    opt->long_form = l; \\
    opt->help_msg = h; \\
};

typedef struct Opt {
    char short_form;
    char *long_form;
    char *help_msg;
} Opt;

/**
 * stores all options
 */
typedef struct Opts {
    unsigned int num;
    Opt *opts[MAX_OPT_NUM];
} Opts;


