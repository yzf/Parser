/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     S_VARI = 259,
     S_PRED = 260,
     S_FUNC = 261,
     S_NEGA = 262,
     S_CONJ = 263,
     S_DISJ = 264,
     S_IMPL = 265,
     S_UNIV = 266,
     S_EXIS = 267,
     PERIOD = 268,
     LPAREN = 269,
     RPAREN = 270,
     LBRACKET = 271,
     RBRACKET = 272,
     COMMA = 273,
     EQUAL = 274,
     MARCO = 275,
     LBRACE = 276,
     RBRACE = 277,
     SEMICO = 278,
     AT = 279,
     LL = 280,
     RR = 281
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define S_VARI 259
#define S_PRED 260
#define S_FUNC 261
#define S_NEGA 262
#define S_CONJ 263
#define S_DISJ 264
#define S_IMPL 265
#define S_UNIV 266
#define S_EXIS 267
#define PERIOD 268
#define LPAREN 269
#define RPAREN 270
#define LBRACKET 271
#define RBRACKET 272
#define COMMA 273
#define EQUAL 274
#define MARCO 275
#define LBRACE 276
#define RBRACE 277
#define SEMICO 278
#define AT 279
#define LL 280
#define RR 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 27 "sm_parse.y"

    int num;
    char* s;
    struct __formula* f;
    struct __term* t;
    struct __terms* ts;



/* Line 2068 of yacc.c  */
#line 112 "../src/parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


