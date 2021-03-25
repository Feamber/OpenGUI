%{
#include <stdio.h>
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/StyleRule.h"
#include "OpenGUI/Style/StyleProperty.h"

extern char *lineptr;

static OGUI::StyleSheet styleSheet;
static OGUI::StyleRule styleRule;
static OGUI::StyleSelector::Part selectorPart;
static OGUI::StyleSelector selector;
static OGUI::StyleComplexSelector complexSelector;
static int complexSelectorCount;
static char value[100];

template<class T>
void reset(T& val)
{
    new(&val) T{};
}
//Button#test.test:test { left:10px }
%}
%code requires {
using yyscan_t = void*;
struct ParseContext {
    char*   linebuf;
    size_t  linebuf_size;
    size_t linebuf_len;
};
}
%code {
extern void yyerror(struct YYLTYPE * yylloc_param, yyscan_t yyscanner, const char *s);
extern int yylex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , yyscan_t yyscanner);
}

%define parse.error verbose
%locations
%define api.pure full
%lex-param {void * scanner}
%parse-param {void * scanner}

%union
{
    char* text;
}

%token LPAREN RPAREN LBRACKET RBRACKET GREATER COLON WHITESPACE NUM MULTIPLY
%token CLASS NAME IDENT SIZE STRING COMMA SEMICOLON KEYWORD URL COLOR

%type<text> CLASS NAME STRING SIZE IDENT COLOR URL NUM

%%
StyleSheet:
    Statement
    | StyleSheet Statement 
    {};

Statement:
    RuleSet
    {
    };

RuleSet:
    SelectorList LBRACKET RuleSetContent RBRACKET
    {
        int ruleId = styleSheet.styleRules.size();
        styleSheet.styleRules.push_back(std::move(styleRule));
        reset(styleRule);
        int newSize = styleSheet.styleSelectors.size();
        for(int i=complexSelectorCount; i < newSize; i++)
            styleSheet.styleSelectors[i].ruleIndex = ruleId;
        complexSelectorCount = newSize;
    };

SelectorList:
    ComplexSelector
    {
        complexSelector.priority = @1.first_line;
        std::reverse(complexSelector.selectors.begin(), complexSelector.selectors.end());
        styleSheet.styleSelectors.push_back(std::move(complexSelector));
        reset(complexSelector);
    }
    | ComplexSelector COMMA SelectorList
    {
        complexSelector.priority = @1.first_line;
        std::reverse(complexSelector.selectors.begin(), complexSelector.selectors.end());
        styleSheet.styleSelectors.push_back(std::move(complexSelector));
        reset(complexSelector);
    };

ComplexSelector:
    Selector ' ' ComplexSelector
    {
        if(complexSelector.selectors.size())
            complexSelector.selectors.back().relationship = OGUI::StyleSelectorRelationship::Descendent;
        complexSelector.selectors.push_back(std::move(selector));
        reset(selector);
    }
    | Selector GREATER ComplexSelector
    {
        if(complexSelector.selectors.size())
            complexSelector.selectors.back().relationship = OGUI::StyleSelectorRelationship::Child;
        complexSelector.selectors.push_back(std::move(selector));
        reset(selector);
    }
    | Selector
    {
        complexSelector.selectors.push_back(std::move(selector));
        reset(selector);
    };

Selector:
    SelectorPart
    {
        selector.parts.push_back(std::move(selectorPart));
        reset(selectorPart);
    }
    | SelectorPart Selector
    {
        selector.parts.push_back(std::move(selectorPart));
        reset(selectorPart);
    };

SelectorPart:
    IDENT
    {
        selectorPart.type = OGUI::StyleSelector::Type;
        selectorPart.value = $1;
    }
    | CLASS
    {
        selectorPart.type = OGUI::StyleSelector::Class;
        selectorPart.value = $1;
    }
    | COLON IDENT
    {
        selector.AddPseudoClass($2);
    }
    | NAME
    {
        selectorPart.type = OGUI::StyleSelector::Name;
        selectorPart.value = $1;
    }
    | MULTIPLY
    {
        selectorPart.type = OGUI::StyleSelector::Wildcard;
    };

RuleSetItem:
    IDENT COLON Value 
    {
        const char* errorMsg = nullptr;
        OGUI::ParseErrorType errorType;
        if(!OGUI::ParseProperty(styleSheet.storage, $1, value, styleRule, errorMsg, errorType))
        {
            yyerror(errorType == OGUI::ParseErrorType::InvalidProperty? &@1:&@3, scanner, errorMsg);
            YYABORT;
        }
        
        memset(value, 0, sizeof(value));
    };

RuleSetContent:
    RuleSetItem
    | RuleSetItem SEMICOLON RuleSetContent
    | /* empty */
    {};

Value:
    STRING { strcat(value, $1); }
    | IDENT { strcat(value, $1); }
    | COLOR { strcat(value, $1); }
    | URL { strcat(value, $1); }
    | NUM { strcat(value, $1); }
    | SizeList {}



SizeList:
    SIZE { strcat(value, $1); }
    | SIZE ',' SizeList
    {   
        static char temp[100];
        strcpy(temp, $1);
        strcat(temp, value);
        strcpy(value, temp); 
    };

%%

extern ParseContext* yyget_extra ( yyscan_t yyscanner );
void yyset_extra (ParseContext*  user_defined , yyscan_t yyscanner);
void yyerror(struct YYLTYPE * yylloc_param, yyscan_t yyscanner, const char *s)
{
    auto& yylloc = *yylloc_param;
    fprintf(stderr,"error: %s in line %d, column %d\n", s, yylloc.first_line, yylloc.first_column);
    if(!yyscanner)
        return;
    const char* buf = yyget_extra(yyscanner)->linebuf;
    if(strlen(buf) == 0)
        return;
    fprintf(stderr,"%s", buf);
    for(int i = 0; i < yylloc.first_column - 1; i++)
        fprintf(stderr,"_");
    for(int i = yylloc.first_column; i < yylloc.last_column; i++)
        fprintf(stderr,"^");
    fprintf(stderr,"\n");
}

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse(yyscan_t);
extern YY_BUFFER_STATE yy_scan_string (const char * yystr , yyscan_t yyscanner);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer,yyscan_t);
extern int yylex_init(yyscan_t*);
extern int yylex_destroy(yyscan_t);
extern void yyset_in(FILE* _in_str, yyscan_t yyscanner);
#include <optional>
namespace OGUI
{
std::optional<StyleSheet> ParseCSS(std::string_view content) {
    yyscan_t scanner = nullptr;
    yylex_init(&scanner);
    ParseContext context;
    char linebuf[500] = {};
    context.linebuf = linebuf;
    context.linebuf_size = 500;
    yyset_extra(&context, scanner);
    YY_BUFFER_STATE buffer = yy_scan_string((char*)content.data(), scanner);
    if(!yyparse(scanner))
    {
        yy_delete_buffer(buffer, scanner);
        yylex_destroy(scanner);
        OGUI::StyleSheet res = std::move(styleSheet);
        reset(styleSheet);
        return res;
    }
    else
    {
        yy_delete_buffer(buffer, scanner);
        yylex_destroy(scanner);
        return {};
    }
}
std::optional<StyleSheet> ParseCSSFile(std::string path) {
    auto F = fopen(path.c_str(), "r");
    if(!F)
        return {};
    yyscan_t scanner = nullptr;
    yylex_init(&scanner);
    ParseContext context;
    char linebuf[500] = {};
    context.linebuf = linebuf;
    context.linebuf_size = 500;
    yyset_extra(&context, scanner);
    yyset_in(F, scanner);
    if(!yyparse(scanner))
    {
        fclose(F);
        yylex_destroy(scanner);
        OGUI::StyleSheet res = std::move(styleSheet);
        reset(styleSheet);
        return res;
    }
    else
    {
        fclose(F);
        yylex_destroy(scanner);
        return {};
    }
}
}