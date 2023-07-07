/* Program to generate term-biased snippets for paragraphs of text.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2022, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Toby Fung 1356386
   Dated:     13/09/2022

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

/* you may need to delete this next line on some platforms
   in order to get it to compile; if you end up doing that
   make sure you put it back before you submit to the
   Gradescope system, because it is required there */
#include <strings.h>

/* maximum number of characters per word */
#define MAX_WORD_LEN 23
/* maximum number of words per paragraph */
#define MAX_PARA_LEN 10000

/* return code from get_word if end of paragraph found */
#define PARA_END 1
/* return code from get_word if a word successfully found */
#define WORD_FND 2

/* terminating punctuation that may follow a word */
#define TERM_PUNCT ".,;:!?"
/* terminating punctuation that needs dots added */
#define NEEDS_DOTS ",;:"
/* the string the separates paragraphs */
#define PARA_SEPARATOR "\n\n"
/* insertion characters to indicate "bold" */
#define BBOLD "**"
/* length of those characters when wrapped around a word */
#define BBOLD_LEN (2 * strlen(BBOLD))
/* insertion characters to indicate omitted text */
#define DDOTS "..."

/* maximum words in any output snippet */
#define MAX_SNIPPET_LEN 30
/* minimum words in any output snippet */
#define MIN_SNIPPET_LEN 20
/* maximum length of output lines */
#define MAX_OUTPUT_LINE 72

/* maximum terms allowed on command line */
#define MAX_TERMS 50
/* signal for no match between word and query term */
#define NO_MATCH (-1)

typedef char word_t[MAX_WORD_LEN + 1];
int get_word(word_t w, int limit);

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration
*/

typedef char para_t[MAX_PARA_LEN + 1][MAX_WORD_LEN + 1];

// functions prototype
int get_paragraph(word_t w, para_t para, int limit);
int print_format_para(para_t para, int limit);
int stage_2(para_t para, para_t target, int target_num, int para_num,
            int matches);
int stage_3(para_t para, para_t target, int target_num, int para_num);
int word_count = -1;

/* main program controls all the action */
int main(int argc, char *argv[])
{
    word_t w, temp;
    para_t target, para;
    int flag = 0;
    int matches = 0;
    int para_num = 1;

    // adding target query term into target array
    int i, target_num = (argc - 1), j;
    for (i = 1; i < argc; i++)
    {
        // converting query term to lower case
        for (j = 0; j < strlen(argv[i]); j++)
        {
            temp[j] = tolower(argv[i][j]);
        }
        strncpy(target[i - 1], temp, strlen(argv[i]));
    }

    // checking what get_word() returns
    while (flag != 1)
    {
        switch (get_word(w, MAX_WORD_LEN))
        {
        case WORD_FND:
            word_count += 1;
            get_paragraph(w, para, MAX_PARA_LEN);
            break;

        case PARA_END:
            printf("\n======= Stage 1 [para %d; %d words]\n",
                   para_num, word_count + 1);
            stage_2(para, target, target_num, para_num, matches);
            stage_3(para, target, target_num, para_num);
            word_count = 0, para_num += 1;
            get_paragraph(w, para, MAX_PARA_LEN);
            break;

        case EOF:
            printf("\n======= Stage 1 [para %d; %d words]\n",
                   para_num, word_count + 1);
            stage_2(para, target, target_num, para_num, matches);
            stage_3(para, target, target_num, para_num);
            flag = 1;
            break;

        default:
            flag = 1;
            break;
        }
    }

    // printing ta daa
    printf("\nta daa!\n");
    return 0;
}

/* extract a single word out of the standard input, but not
   more than "limit" characters in total. One character of
   sensible trailing punctuation is retained.
   argument array W must be limit+1 characters or bigger
*/
int get_word(word_t W, int limit)
{
    word_t skipped_char;
    int c, i = 0;
    char *p1;

    /* first, skip over any non alphanumerics */
    while ((c = getchar()) != EOF && !isalnum(c))
    {
        // adding "skipped" char into an array
        skipped_char[i] = c;
        i += 1;
    }

    // checking if "\n\n" is in the skipped array
    p1 = strstr(skipped_char, PARA_SEPARATOR);

    if (c == EOF)
    {
        return EOF;
    }
    /* ok, first character of next word has been found */
    *W = c;
    W += 1;
    limit -= 1;

    while (limit > 0 && (c = getchar()) != EOF && isalnum(c))
    {
        /* another character to be stored */
        *W = c;
        W += 1;
        limit -= 1;
    }
    /* take a look at that next character, is it a sensible trailing
       punctuation? */
    if (strchr(TERM_PUNCT, c) && (limit > 0))
    {
        /* yes, it is */
        *W = c;
        W += 1;
        limit -= 1;
    }

    /* now close off the string */
    *W = '\0';

    // if "\n\n" is present then return end of para
    if (p1 != NULL)
    {
        return PARA_END;
    }

    return WORD_FND;
}

// adds each word into array
int get_paragraph(word_t w, para_t para, int limit)
{
    if (limit > 0)
    {
        strcpy(para[word_count], w);
        limit -= 1;
    }
    return 0;
}

// printing the formatted paragraph
int print_format_para(para_t para, int limit)
{
    int i, line_count = strlen(para[0]);

    // prints the first word
    printf("%s", para[0]);

    // loops and prints each word until limit is reached
    for (i = 1; i < limit + 1; i++)
    {
        // checking if line count will be over 72
        if (line_count + 1 + strlen(para[i]) <= MAX_OUTPUT_LINE)
        {
            printf(" %s", para[i]);
            // adding 1 for empty space
            line_count += 1 + strlen(para[i]);
        }
        // yes it exceeds
        else if (line_count + 1 + strlen(para[i]) > MAX_OUTPUT_LINE)
        {
            printf("\n");
            printf("%s", para[i]);
            line_count = strlen(para[i]);
        }
    }
    return 0;
}

// find the number of query term in a paragraph
int stage_2(para_t para, para_t target, int target_num, int para_num,
            int matches)
{
    int i, j;
    char temp;

    matches = 0;
    // looping through the number of query terms
    for (i = 0; i < target_num; i++)
    {
        // looping through each word in the paragraph
        for (j = 0; j < word_count + 1; j++)
        {
            // finding the last char of the word
            temp = para[j][strlen(para[j]) - 1];

            // testing if the word matches with query
            if (strncasecmp(para[j], target[i], strlen(target[i])) == 0)
            {
                word_t highlight = BBOLD;

                // testing if the last char is a punctuation
                if (strlen(para[j]) != strlen(target[i]) &&
                    strlen(para[j]) == strlen(target[i]) + 1 &&
                    strchr(TERM_PUNCT, temp))
                {
                    // add the word to "**" then add "**" then add "punctuation"
                    strncat(highlight, para[j], strlen(target[i]));
                    strcat(highlight, BBOLD);
                    strncat(highlight, &temp, 1);
                    strcpy(para[j], highlight);
                    matches += 1;
                }
                // if no, add the word to "**" then add "**"
                else if (strlen(para[j]) == strlen(target[i]))
                {
                    strcat(highlight, para[j]);
                    strcat(highlight, BBOLD);
                    strcpy(para[j], highlight);
                    matches += 1;
                }
            }
        }
    }

    // printing output of the highlighted paragraph
    printf("\n======= Stage 2 [para %d; %d words; %d matches]\n",
           para_num, word_count + 1, matches);
    print_format_para(para, word_count);
    printf("\n");
    return 0;
}

// looping through each snippet and scoring them
int stage_3(para_t para, para_t target, int target_num, int para_num)
{
    para_t snippet, temp, max_para;
    int snip_size, i, j, k, start_point, pos, snip_word_count, query_found;
    int max_start = 0, max_len = 0, dots = 1, max_dots = 1;
    double max_score = 0;
    double query = 1.0, preword_punt = 0.6, end_punt = 0.3, exceed = (-0.1),
           score;
    for (snip_size = MIN_SNIPPET_LEN; snip_size < MAX_SNIPPET_LEN; snip_size++)
    {
        for (start_point = 0; start_point < (word_count - snip_size) + 2;
             start_point++)
        {
            pos = 0, snip_word_count = 0, score = 0.0;
            for (j = 0; j < target_num; j++)
            {

                strcpy(temp[j], target[j]);
            }

            for (i = start_point; i < snip_size + start_point; i++)
            {
                strcpy(snippet[pos], para[i]);
                pos += 1;
                if (strchr(para[i], '*'))
                {
                    for (k = 0; k < target_num; k++)
                    {
                        word_t highlight = BBOLD;
                        strcat(highlight, temp[k]);

                        if (!strncasecmp(para[i], highlight, strlen(highlight)))
                        {
                            score += (strlen(temp[k]) / 2.);
                            strcpy(temp[k], "----");
                            query_found = 1;
                        }
                    }
                    if (query_found != 1)
                    {
                        score += query;
                    }
                }
                query_found = 0;
            }
            snip_word_count = pos;

            score += 15. / (start_point + 10);
            score += exceed * (snip_size - MIN_SNIPPET_LEN);

            char last_char = snippet[snip_word_count - 1]
                                    [strlen(snippet[snip_word_count - 1]) - 1];
            if (strchr(".!?", last_char))
            {
                dots = 0;
                score += end_punt;
            }
            else if (strchr(NEEDS_DOTS, last_char))
            {
                score += end_punt;
            }

            if (start_point == 0 ||
                (start_point != 0 &&
                 strchr(TERM_PUNCT, para[start_point - 1]
                                        [strlen(para[start_point - 1]) - 1])))
            {
                score += preword_punt;
            }
            if (score > max_score ||
                (score == max_score && start_point < max_start) ||
                (score == max_score && start_point == max_start &&
                 snip_word_count < max_len))
            {
                max_len = snip_word_count;
                max_start = start_point;
                max_score = score;
                max_dots = dots;
                for (k = 0; k < snip_word_count; k++)
                {
                    strcpy(max_para[k], snippet[k]);
                }
            }
            score = 0, snip_word_count = 0, dots = 1;
        }
    }
    printf("\n======= Stage 3 [para %d; start %d; length %d; score %.2f]\n",
           para_num, max_start, max_len, max_score);
    print_format_para(max_para, max_len - 1);
    if (max_dots == 1)
    {
        printf("%s\n", DDOTS);
    }
    else
    {
        printf("\n");
    }
    return 0;
}

// "algorithms are fun"..

//* ./grok_playground alice children poison drink bottle < data3.txt > mytest3-out.txt
//* diff test3-out.txt mytest3-out.txt