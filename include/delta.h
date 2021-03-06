#ifndef DELTA_H_
#define DELTA_H_

#include "delta-file.h"
#include "strbuf-list.h"

#define DELIM '\n'

/* arrow types used in delta algorithms */
enum arrow_t { DELTA_UP, DELTA_LEFT, DELTA_TILT, DELTA_DOWN };

#define DELTA_FLAG (0x1 << 1)

/**
 * delta_table used to store the raw result
 */
struct delta_table {
    enum arrow_t **table; /* two dimensional arrow table */

    int *prev; /* previous table entries */
    int *sol;  /* actual solution entry */
    int row;   /* number of rows */
    int col;   /* number of columns */
};

struct delta_input {
    struct filespec *fs1; /* filespec of the first file */
    struct filespec *fs2; /* filespec of the second file */
    struct deltafile df1;
    struct deltafile df2;
};

struct basic_delta_result {
    size_t insertions;               /* number of insertions */
    size_t deletions;                /* number of deletions */
    size_t common;                   /* common number of lines */
    struct delta_input *input;       /* input to the delta */
    struct strbuf_list common_lines; /* buffer to store the
                                        similar lines */
    struct strbuf_list diff_lines;   /* buffer to store the non-similar lines */
};

/**
 * initialise the table for delta. This table stores the intermediate result
 * that delta computes while comparison. All the details of the comparisons are
 * stored in this table (yet in raw format).
 */
extern int delta_table_init(struct delta_table *table, int row, int col);

/**
 * release the memory
 */
extern void delta_table_free(struct delta_table *table);

/**
 * initialise the delta_input
 */
extern int delta_input_init(struct delta_input *di, struct filespec *fs1,
                            struct filespec *fs2);

/**
 * release the memory
 */
extern void delta_input_free(struct delta_input *di);

/**
 * backtracks the table generated by delta to produce more user
 * friendly results.
 */
extern int delta_backtrace_table(struct basic_delta_result *result,
                                 struct delta_table *table, struct deltafile *a,
                                 struct deltafile *b);

/**
 * initialise the basic_delta_result which stores the delta result in readable
 * form. delta_table can be converted into basic_delta_result by backtracking.
 */
extern void basic_delta_result_init(struct basic_delta_result *bdr,
                                    struct delta_input *di);

/**
 * main delta algorithm. It uses Longest Common Subsequence.
 */
extern size_t delta_basic_comparison_m(struct delta_table *out,
                                       struct deltafile *af,
                                       struct deltafile *bf);

/**
 * print the stats of the delta result.
 */
extern void delta_stat(struct basic_delta_result *result, struct strbuf *stat);

/**
 * print the summary to a given buffer
 */
extern void delta_summary(struct basic_delta_result *bdr,
                          struct strbuf *summary);

extern int delta_main(int argc, char *argv[]);

extern bool strbuf_delta_minimal(struct strbuf *out,
    struct basic_delta_result *result, struct strbuf *b, struct strbuf *a);

extern bool strbuf_delta_enhanced(struct strbuf *out,
                  struct basic_delta_result *result, struct strbuf *b,
                  struct strbuf *a);

extern void print_insertion_lines(struct strbuf *buf);

extern size_t print_lines(struct strbuf *buf, bool i_or_d);
#define DELIM '\n'
#endif
