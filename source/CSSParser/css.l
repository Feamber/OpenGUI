%{
#include <stdio.h>
#include <algorithm>
#include "css.tab.h"
extern void yyerror(struct YYLTYPE * yylloc_param, void* scanner, const char *s);

size_t getline(char **lineptr, size_t *n, FILE *stream);
#define YY_INPUT(buf,result,max_size) do {                       \
    yyextra->linebuf_len = getline(&yyextra->linebuf,              \
                                   &yyextra->linebuf_size, yyin);  \
    if (yyextra->linebuf_len == -1) result = YY_NULL;              \
    else if (yyextra->linebuf_len <= max_size) {                   \
      memcpy(buf, yyextra->linebuf, yyextra->linebuf_len);         \
      result = yyextra->linebuf_len;                               \
    }                                                              \
    else { /* Handle long lines */ }                               \
  } while(0)
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }
%}
%option extra-type="struct ParseContext*"
%option nomain
%option noinput nounput
%option noyywrap
%option case-insensitive
%option bison-locations
%option reentrant
%option bison-bridge

h			[0-9a-f]
nonascii	[\200-\377]
unicode		\\{h}{1,6}(\r\n|[ \t\r\n\f])?
escape		{unicode}|\\[ -~\200-\377]
nmstart		[_a-z]|{nonascii}|{escape}
nmchar		[_a-zA-Z0-9-]|{nonascii}|{escape}
string1		\"([\t !#$%&(-~]|\\{nl}|\’|{nonascii}|{escape})*\"
string2		\’([\t !#$%&(-~]|\\{nl}|\"|{nonascii}|{escape})*\’
color       "rgba("{w}{num}{w}","{w}{num}{w}","{w}{num}{w}","{w}{num}{w}")"
ident		{nmstart}{nmchar}*
name		{nmchar}+
num			[0-9]+|[0-9]*"."[0-9]+
string		{string1}|{string2}
url			([!#$%&*-~]|{nonascii}|{escape})*
s			[ \t\r\n\f]
w			{s}*
nl			\n|\r\n|\r|\f
range		\?{1,6}|{h}(\?{0,5}|{h}(\?{0,4}|{h}(\?{0,3}|{h}(\?{0,2}|{h}(\??|{h})))))
size_type   "px"|"%"
size        ({num}{size_type})|"0"

%%
"."{ident}      { yylval->text = _strdup(yytext + 1); return CLASS; }
"#"{ident}      { yylval->text = _strdup(yytext + 1); return NAME; }
{string}        { yylval->text = _strdup(yytext); return STRING; }
{size}          { yylval->text = _strdup(yytext); return SIZE; }
{num}           { yylval->text = _strdup(yytext); return NUM; }
{ident}         { yylval->text = _strdup(yytext); return IDENT; }
"{"             { return LBRACKET; }
"}"             { return RBRACKET; }
"("             { return LPAREN; }
")"             { return RPAREN; }
">"             { return GREATER; }
{w}             /*white space*/
","             { return COMMA; }
";"             { return SEMICOLON; }
":"             { return COLON; }
"*"             { return MULTIPLY; }
"url("{url}")"  { yylval->text = _strdup(yytext+4); yylval->text[strlen(yylval->text) - 1] = 0; return URL; }
{color}         { yylval->text = _strdup(yytext+5); yylval->text[strlen(yylval->text) - 1] = 0; ; return COLOR; }
.               { yyerror(yylloc, nullptr, "unexcepted token."); fprintf(stderr, "%s\n", yytext); }
%%

typedef intptr_t ssize_t;

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = (char*)malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = (char*)realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}