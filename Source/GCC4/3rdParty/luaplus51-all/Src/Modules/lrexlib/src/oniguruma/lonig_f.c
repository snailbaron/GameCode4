/* lonig_f.c - Lua binding of Oniguruma library */
/* See Copyright Notice in the file LICENSE */

#include <oniguruma.h>
#include <lua.h>
#include <lauxlib.h>
#include "../common.h"

static flag_pair onig_flags[] = {
#ifdef ONIG_INFINITE_DISTANCE
  { "ONIG_INFINITE_DISTANCE",                   ONIG_INFINITE_DISTANCE },
#endif
#ifdef ONIG_NREGION
  { "ONIG_NREGION",                             ONIG_NREGION },
#endif
#ifdef ONIG_MAX_BACKREF_NUM
  { "ONIG_MAX_BACKREF_NUM",                     ONIG_MAX_BACKREF_NUM },
#endif
#ifdef ONIG_MAX_REPEAT_NUM
  { "ONIG_MAX_REPEAT_NUM",                      ONIG_MAX_REPEAT_NUM },
#endif
#ifdef ONIG_MAX_MULTI_BYTE_RANGES_NUM
  { "ONIG_MAX_MULTI_BYTE_RANGES_NUM",           ONIG_MAX_MULTI_BYTE_RANGES_NUM },
#endif
#ifdef ONIG_MAX_ERROR_MESSAGE_LEN
  { "ONIG_MAX_ERROR_MESSAGE_LEN",               ONIG_MAX_ERROR_MESSAGE_LEN },
#endif
#ifdef ONIG_OPTION_DEFAULT
  { "ONIG_OPTION_DEFAULT",                      ONIG_OPTION_DEFAULT },
  { "DEFAULT",                                  ONIG_OPTION_DEFAULT },
#endif
#ifdef ONIG_OPTION_NONE
  { "ONIG_OPTION_NONE",                         ONIG_OPTION_NONE },
  { "NONE",                                     ONIG_OPTION_NONE },
#endif
#ifdef ONIG_OPTION_IGNORECASE
  { "ONIG_OPTION_IGNORECASE",                   ONIG_OPTION_IGNORECASE },
  { "IGNORECASE",                               ONIG_OPTION_IGNORECASE },
#endif
#ifdef ONIG_OPTION_EXTEND
  { "ONIG_OPTION_EXTEND",                       ONIG_OPTION_EXTEND },
  { "EXTEND",                                   ONIG_OPTION_EXTEND },
#endif
#ifdef ONIG_OPTION_MULTILINE
  { "ONIG_OPTION_MULTILINE",                    ONIG_OPTION_MULTILINE },
  { "MULTILINE",                                ONIG_OPTION_MULTILINE },
#endif
#ifdef ONIG_OPTION_SINGLELINE
  { "ONIG_OPTION_SINGLELINE",                   ONIG_OPTION_SINGLELINE },
  { "SINGLELINE",                               ONIG_OPTION_SINGLELINE },
#endif
#ifdef ONIG_OPTION_FIND_LONGEST
  { "ONIG_OPTION_FIND_LONGEST",                 ONIG_OPTION_FIND_LONGEST },
  { "FIND_LONGEST",                             ONIG_OPTION_FIND_LONGEST },
#endif
#ifdef ONIG_OPTION_FIND_NOT_EMPTY
  { "ONIG_OPTION_FIND_NOT_EMPTY",               ONIG_OPTION_FIND_NOT_EMPTY },
  { "FIND_NOT_EMPTY",                           ONIG_OPTION_FIND_NOT_EMPTY },
#endif
#ifdef ONIG_OPTION_NEGATE_SINGLELINE
  { "ONIG_OPTION_NEGATE_SINGLELINE",            ONIG_OPTION_NEGATE_SINGLELINE },
  { "NEGATE_SINGLELINE",                        ONIG_OPTION_NEGATE_SINGLELINE },
#endif
#ifdef ONIG_OPTION_DONT_CAPTURE_GROUP
  { "ONIG_OPTION_DONT_CAPTURE_GROUP",           ONIG_OPTION_DONT_CAPTURE_GROUP },
  { "DONT_CAPTURE_GROUP",                       ONIG_OPTION_DONT_CAPTURE_GROUP },
#endif
#ifdef ONIG_OPTION_CAPTURE_GROUP
  { "ONIG_OPTION_CAPTURE_GROUP",                ONIG_OPTION_CAPTURE_GROUP },
  { "CAPTURE_GROUP",                            ONIG_OPTION_CAPTURE_GROUP },
#endif
#ifdef ONIG_OPTION_NOTBOL
  { "ONIG_OPTION_NOTBOL",                       ONIG_OPTION_NOTBOL },
  { "NOTBOL",                                   ONIG_OPTION_NOTBOL },
#endif
#ifdef ONIG_OPTION_NOTEOL
  { "ONIG_OPTION_NOTEOL",                       ONIG_OPTION_NOTEOL },
  { "NOTEOL",                                   ONIG_OPTION_NOTEOL },
#endif
#ifdef ONIG_OPTION_POSIX_REGION
  { "ONIG_OPTION_POSIX_REGION",                 ONIG_OPTION_POSIX_REGION },
  { "POSIX_REGION",                             ONIG_OPTION_POSIX_REGION },
#endif
#ifdef ONIG_OPTION_MAXBIT
  { "ONIG_OPTION_MAXBIT",                       ONIG_OPTION_MAXBIT },
  { "MAXBIT",                                   ONIG_OPTION_MAXBIT },
#endif
#ifdef ONIG_SYN_OP_VARIABLE_META_CHARACTERS
  { "ONIG_SYN_OP_VARIABLE_META_CHARACTERS",     ONIG_SYN_OP_VARIABLE_META_CHARACTERS },
#endif
#ifdef ONIG_SYN_OP_DOT_ANYCHAR
  { "ONIG_SYN_OP_DOT_ANYCHAR",                  ONIG_SYN_OP_DOT_ANYCHAR },
#endif
#ifdef ONIG_SYN_OP_ASTERISK_ZERO_INF
  { "ONIG_SYN_OP_ASTERISK_ZERO_INF",            ONIG_SYN_OP_ASTERISK_ZERO_INF },
#endif
#ifdef ONIG_SYN_OP_ESC_ASTERISK_ZERO_INF
  { "ONIG_SYN_OP_ESC_ASTERISK_ZERO_INF",        ONIG_SYN_OP_ESC_ASTERISK_ZERO_INF },
#endif
#ifdef ONIG_SYN_OP_PLUS_ONE_INF
  { "ONIG_SYN_OP_PLUS_ONE_INF",                 ONIG_SYN_OP_PLUS_ONE_INF },
#endif
#ifdef ONIG_SYN_OP_ESC_PLUS_ONE_INF
  { "ONIG_SYN_OP_ESC_PLUS_ONE_INF",             ONIG_SYN_OP_ESC_PLUS_ONE_INF },
#endif
#ifdef ONIG_SYN_OP_QMARK_ZERO_ONE
  { "ONIG_SYN_OP_QMARK_ZERO_ONE",               ONIG_SYN_OP_QMARK_ZERO_ONE },
#endif
#ifdef ONIG_SYN_OP_ESC_QMARK_ZERO_ONE
  { "ONIG_SYN_OP_ESC_QMARK_ZERO_ONE",           ONIG_SYN_OP_ESC_QMARK_ZERO_ONE },
#endif
#ifdef ONIG_SYN_OP_BRACE_INTERVAL
  { "ONIG_SYN_OP_BRACE_INTERVAL",               ONIG_SYN_OP_BRACE_INTERVAL },
#endif
#ifdef ONIG_SYN_OP_ESC_BRACE_INTERVAL
  { "ONIG_SYN_OP_ESC_BRACE_INTERVAL",           ONIG_SYN_OP_ESC_BRACE_INTERVAL },
#endif
#ifdef ONIG_SYN_OP_VBAR_ALT
  { "ONIG_SYN_OP_VBAR_ALT",                     ONIG_SYN_OP_VBAR_ALT },
#endif
#ifdef ONIG_SYN_OP_ESC_VBAR_ALT
  { "ONIG_SYN_OP_ESC_VBAR_ALT",                 ONIG_SYN_OP_ESC_VBAR_ALT },
#endif
#ifdef ONIG_SYN_OP_LPAREN_SUBEXP
  { "ONIG_SYN_OP_LPAREN_SUBEXP",                ONIG_SYN_OP_LPAREN_SUBEXP },
#endif
#ifdef ONIG_SYN_OP_ESC_LPAREN_SUBEXP
  { "ONIG_SYN_OP_ESC_LPAREN_SUBEXP",            ONIG_SYN_OP_ESC_LPAREN_SUBEXP },
#endif
#ifdef ONIG_SYN_OP_ESC_AZ_BUF_ANCHOR
  { "ONIG_SYN_OP_ESC_AZ_BUF_ANCHOR",            ONIG_SYN_OP_ESC_AZ_BUF_ANCHOR },
#endif
#ifdef ONIG_SYN_OP_ESC_CAPITAL_G_BEGIN_ANCHOR
  { "ONIG_SYN_OP_ESC_CAPITAL_G_BEGIN_ANCHOR",   ONIG_SYN_OP_ESC_CAPITAL_G_BEGIN_ANCHOR },
#endif
#ifdef ONIG_SYN_OP_DECIMAL_BACKREF
  { "ONIG_SYN_OP_DECIMAL_BACKREF",              ONIG_SYN_OP_DECIMAL_BACKREF },
#endif
#ifdef ONIG_SYN_OP_BRACKET_CC
  { "ONIG_SYN_OP_BRACKET_CC",                   ONIG_SYN_OP_BRACKET_CC },
#endif
#ifdef ONIG_SYN_OP_ESC_W_WORD
  { "ONIG_SYN_OP_ESC_W_WORD",                   ONIG_SYN_OP_ESC_W_WORD },
#endif
#ifdef ONIG_SYN_OP_ESC_LTGT_WORD_BEGIN_END
  { "ONIG_SYN_OP_ESC_LTGT_WORD_BEGIN_END",      ONIG_SYN_OP_ESC_LTGT_WORD_BEGIN_END },
#endif
#ifdef ONIG_SYN_OP_ESC_B_WORD_BOUND
  { "ONIG_SYN_OP_ESC_B_WORD_BOUND",             ONIG_SYN_OP_ESC_B_WORD_BOUND },
#endif
#ifdef ONIG_SYN_OP_ESC_S_WHITE_SPACE
  { "ONIG_SYN_OP_ESC_S_WHITE_SPACE",            ONIG_SYN_OP_ESC_S_WHITE_SPACE },
#endif
#ifdef ONIG_SYN_OP_ESC_D_DIGIT
  { "ONIG_SYN_OP_ESC_D_DIGIT",                  ONIG_SYN_OP_ESC_D_DIGIT },
#endif
#ifdef ONIG_SYN_OP_LINE_ANCHOR
  { "ONIG_SYN_OP_LINE_ANCHOR",                  ONIG_SYN_OP_LINE_ANCHOR },
#endif
#ifdef ONIG_SYN_OP_POSIX_BRACKET
  { "ONIG_SYN_OP_POSIX_BRACKET",                ONIG_SYN_OP_POSIX_BRACKET },
#endif
#ifdef ONIG_SYN_OP_QMARK_NON_GREEDY
  { "ONIG_SYN_OP_QMARK_NON_GREEDY",             ONIG_SYN_OP_QMARK_NON_GREEDY },
#endif
#ifdef ONIG_SYN_OP_ESC_CONTROL_CHARS
  { "ONIG_SYN_OP_ESC_CONTROL_CHARS",            ONIG_SYN_OP_ESC_CONTROL_CHARS },
#endif
#ifdef ONIG_SYN_OP_ESC_C_CONTROL
  { "ONIG_SYN_OP_ESC_C_CONTROL",                ONIG_SYN_OP_ESC_C_CONTROL },
#endif
#ifdef ONIG_SYN_OP_ESC_OCTAL3
  { "ONIG_SYN_OP_ESC_OCTAL3",                   ONIG_SYN_OP_ESC_OCTAL3 },
#endif
#ifdef ONIG_SYN_OP_ESC_X_HEX2
  { "ONIG_SYN_OP_ESC_X_HEX2",                   ONIG_SYN_OP_ESC_X_HEX2 },
#endif
#ifdef ONIG_SYN_OP_ESC_X_BRACE_HEX8
  { "ONIG_SYN_OP_ESC_X_BRACE_HEX8",             ONIG_SYN_OP_ESC_X_BRACE_HEX8 },
#endif
#ifdef ONIG_SYN_OP2_ESC_CAPITAL_Q_QUOTE
  { "ONIG_SYN_OP2_ESC_CAPITAL_Q_QUOTE",         ONIG_SYN_OP2_ESC_CAPITAL_Q_QUOTE },
#endif
#ifdef ONIG_SYN_OP2_QMARK_GROUP_EFFECT
  { "ONIG_SYN_OP2_QMARK_GROUP_EFFECT",          ONIG_SYN_OP2_QMARK_GROUP_EFFECT },
#endif
#ifdef ONIG_SYN_OP2_OPTION_PERL
  { "ONIG_SYN_OP2_OPTION_PERL",                 ONIG_SYN_OP2_OPTION_PERL },
#endif
#ifdef ONIG_SYN_OP2_OPTION_RUBY
  { "ONIG_SYN_OP2_OPTION_RUBY",                 ONIG_SYN_OP2_OPTION_RUBY },
#endif
#ifdef ONIG_SYN_OP2_PLUS_POSSESSIVE_REPEAT
  { "ONIG_SYN_OP2_PLUS_POSSESSIVE_REPEAT",      ONIG_SYN_OP2_PLUS_POSSESSIVE_REPEAT },
#endif
#ifdef ONIG_SYN_OP2_PLUS_POSSESSIVE_INTERVAL
  { "ONIG_SYN_OP2_PLUS_POSSESSIVE_INTERVAL",    ONIG_SYN_OP2_PLUS_POSSESSIVE_INTERVAL },
#endif
#ifdef ONIG_SYN_OP2_CCLASS_SET_OP
  { "ONIG_SYN_OP2_CCLASS_SET_OP",               ONIG_SYN_OP2_CCLASS_SET_OP },
#endif
#ifdef ONIG_SYN_OP2_QMARK_LT_NAMED_GROUP
  { "ONIG_SYN_OP2_QMARK_LT_NAMED_GROUP",        ONIG_SYN_OP2_QMARK_LT_NAMED_GROUP },
#endif
#ifdef ONIG_SYN_OP2_ESC_K_NAMED_BACKREF
  { "ONIG_SYN_OP2_ESC_K_NAMED_BACKREF",         ONIG_SYN_OP2_ESC_K_NAMED_BACKREF },
#endif
#ifdef ONIG_SYN_OP2_ESC_G_SUBEXP_CALL
  { "ONIG_SYN_OP2_ESC_G_SUBEXP_CALL",           ONIG_SYN_OP2_ESC_G_SUBEXP_CALL },
#endif
#ifdef ONIG_SYN_OP2_ATMARK_CAPTURE_HISTORY
  { "ONIG_SYN_OP2_ATMARK_CAPTURE_HISTORY",      ONIG_SYN_OP2_ATMARK_CAPTURE_HISTORY },
#endif
#ifdef ONIG_SYN_OP2_ESC_CAPITAL_C_BAR_CONTROL
  { "ONIG_SYN_OP2_ESC_CAPITAL_C_BAR_CONTROL",   ONIG_SYN_OP2_ESC_CAPITAL_C_BAR_CONTROL },
#endif
#ifdef ONIG_SYN_OP2_ESC_CAPITAL_M_BAR_META
  { "ONIG_SYN_OP2_ESC_CAPITAL_M_BAR_META",      ONIG_SYN_OP2_ESC_CAPITAL_M_BAR_META },
#endif
#ifdef ONIG_SYN_OP2_ESC_V_VTAB
  { "ONIG_SYN_OP2_ESC_V_VTAB",                  ONIG_SYN_OP2_ESC_V_VTAB },
#endif
#ifdef ONIG_SYN_OP2_ESC_U_HEX4
  { "ONIG_SYN_OP2_ESC_U_HEX4",                  ONIG_SYN_OP2_ESC_U_HEX4 },
#endif
#ifdef ONIG_SYN_OP2_ESC_GNU_BUF_ANCHOR
  { "ONIG_SYN_OP2_ESC_GNU_BUF_ANCHOR",          ONIG_SYN_OP2_ESC_GNU_BUF_ANCHOR },
#endif
#ifdef ONIG_SYN_OP2_ESC_P_BRACE_CHAR_PROPERTY
  { "ONIG_SYN_OP2_ESC_P_BRACE_CHAR_PROPERTY",   ONIG_SYN_OP2_ESC_P_BRACE_CHAR_PROPERTY },
#endif
#ifdef ONIG_SYN_OP2_ESC_P_BRACE_CIRCUMFLEX_NOT
  { "ONIG_SYN_OP2_ESC_P_BRACE_CIRCUMFLEX_NOT",  ONIG_SYN_OP2_ESC_P_BRACE_CIRCUMFLEX_NOT },
#endif
#ifdef ONIG_SYN_OP2_ESC_H_XDIGIT
  { "ONIG_SYN_OP2_ESC_H_XDIGIT",                ONIG_SYN_OP2_ESC_H_XDIGIT },
#endif
#ifdef ONIG_SYN_OP2_INEFFECTIVE_ESCAPE
  { "ONIG_SYN_OP2_INEFFECTIVE_ESCAPE",          ONIG_SYN_OP2_INEFFECTIVE_ESCAPE },
#endif
#ifdef ONIG_SYN_CONTEXT_INDEP_ANCHORS
  { "ONIG_SYN_CONTEXT_INDEP_ANCHORS",           ONIG_SYN_CONTEXT_INDEP_ANCHORS },
#endif
#ifdef ONIG_SYN_CONTEXT_INDEP_REPEAT_OPS
  { "ONIG_SYN_CONTEXT_INDEP_REPEAT_OPS",        ONIG_SYN_CONTEXT_INDEP_REPEAT_OPS },
#endif
#ifdef ONIG_SYN_CONTEXT_INVALID_REPEAT_OPS
  { "ONIG_SYN_CONTEXT_INVALID_REPEAT_OPS",      ONIG_SYN_CONTEXT_INVALID_REPEAT_OPS },
#endif
#ifdef ONIG_SYN_ALLOW_UNMATCHED_CLOSE_SUBEXP
  { "ONIG_SYN_ALLOW_UNMATCHED_CLOSE_SUBEXP",    ONIG_SYN_ALLOW_UNMATCHED_CLOSE_SUBEXP },
#endif
#ifdef ONIG_SYN_ALLOW_INVALID_INTERVAL
  { "ONIG_SYN_ALLOW_INVALID_INTERVAL",          ONIG_SYN_ALLOW_INVALID_INTERVAL },
#endif
#ifdef ONIG_SYN_ALLOW_INTERVAL_LOW_ABBREV
  { "ONIG_SYN_ALLOW_INTERVAL_LOW_ABBREV",       ONIG_SYN_ALLOW_INTERVAL_LOW_ABBREV },
#endif
#ifdef ONIG_SYN_STRICT_CHECK_BACKREF
  { "ONIG_SYN_STRICT_CHECK_BACKREF",            ONIG_SYN_STRICT_CHECK_BACKREF },
#endif
#ifdef ONIG_SYN_DIFFERENT_LEN_ALT_LOOK_BEHIND
  { "ONIG_SYN_DIFFERENT_LEN_ALT_LOOK_BEHIND",   ONIG_SYN_DIFFERENT_LEN_ALT_LOOK_BEHIND },
#endif
#ifdef ONIG_SYN_CAPTURE_ONLY_NAMED_GROUP
  { "ONIG_SYN_CAPTURE_ONLY_NAMED_GROUP",        ONIG_SYN_CAPTURE_ONLY_NAMED_GROUP },
#endif
#ifdef ONIG_SYN_ALLOW_MULTIPLEX_DEFINITION_NAME
  { "ONIG_SYN_ALLOW_MULTIPLEX_DEFINITION_NAME", ONIG_SYN_ALLOW_MULTIPLEX_DEFINITION_NAME },
#endif
#ifdef ONIG_SYN_FIXED_INTERVAL_IS_GREEDY_ONLY
  { "ONIG_SYN_FIXED_INTERVAL_IS_GREEDY_ONLY",   ONIG_SYN_FIXED_INTERVAL_IS_GREEDY_ONLY },
#endif
#ifdef ONIG_SYN_NOT_NEWLINE_IN_NEGATIVE_CC
  { "ONIG_SYN_NOT_NEWLINE_IN_NEGATIVE_CC",      ONIG_SYN_NOT_NEWLINE_IN_NEGATIVE_CC },
#endif
#ifdef ONIG_SYN_BACKSLASH_ESCAPE_IN_CC
  { "ONIG_SYN_BACKSLASH_ESCAPE_IN_CC",          ONIG_SYN_BACKSLASH_ESCAPE_IN_CC },
#endif
#ifdef ONIG_SYN_ALLOW_EMPTY_RANGE_IN_CC
  { "ONIG_SYN_ALLOW_EMPTY_RANGE_IN_CC",         ONIG_SYN_ALLOW_EMPTY_RANGE_IN_CC },
#endif
#ifdef ONIG_SYN_ALLOW_DOUBLE_RANGE_OP_IN_CC
  { "ONIG_SYN_ALLOW_DOUBLE_RANGE_OP_IN_CC",     ONIG_SYN_ALLOW_DOUBLE_RANGE_OP_IN_CC },
#endif
#ifdef ONIG_SYN_WARN_CC_OP_NOT_ESCAPED
  { "ONIG_SYN_WARN_CC_OP_NOT_ESCAPED",          ONIG_SYN_WARN_CC_OP_NOT_ESCAPED },
#endif
#ifdef ONIG_SYN_WARN_REDUNDANT_NESTED_REPEAT
  { "ONIG_SYN_WARN_REDUNDANT_NESTED_REPEAT",    ONIG_SYN_WARN_REDUNDANT_NESTED_REPEAT },
#endif
#ifdef ONIG_META_CHAR_ESCAPE
  { "ONIG_META_CHAR_ESCAPE",                    ONIG_META_CHAR_ESCAPE },
#endif
#ifdef ONIG_META_CHAR_ANYCHAR
  { "ONIG_META_CHAR_ANYCHAR",                   ONIG_META_CHAR_ANYCHAR },
#endif
#ifdef ONIG_META_CHAR_ANYTIME
  { "ONIG_META_CHAR_ANYTIME",                   ONIG_META_CHAR_ANYTIME },
#endif
#ifdef ONIG_META_CHAR_ZERO_OR_ONE_TIME
  { "ONIG_META_CHAR_ZERO_OR_ONE_TIME",          ONIG_META_CHAR_ZERO_OR_ONE_TIME },
#endif
#ifdef ONIG_META_CHAR_ONE_OR_MORE_TIME
  { "ONIG_META_CHAR_ONE_OR_MORE_TIME",          ONIG_META_CHAR_ONE_OR_MORE_TIME },
#endif
#ifdef ONIG_META_CHAR_ANYCHAR_ANYTIME
  { "ONIG_META_CHAR_ANYCHAR_ANYTIME",           ONIG_META_CHAR_ANYCHAR_ANYTIME },
#endif
#ifdef ONIG_INEFFECTIVE_META_CHAR
  { "ONIG_INEFFECTIVE_META_CHAR",               ONIG_INEFFECTIVE_META_CHAR },
#endif
#ifdef ONIG_NORMAL
  { "ONIG_NORMAL",                              ONIG_NORMAL },
#endif
#ifdef ONIG_MISMATCH
  { "ONIG_MISMATCH",                            ONIG_MISMATCH },
#endif
#ifdef ONIG_NO_SUPPORT_CONFIG
  { "ONIG_NO_SUPPORT_CONFIG",                   ONIG_NO_SUPPORT_CONFIG },
#endif
#ifdef ONIG_MAX_CAPTURE_HISTORY_GROUP
  { "ONIG_MAX_CAPTURE_HISTORY_GROUP",           ONIG_MAX_CAPTURE_HISTORY_GROUP },
#endif
#ifdef ONIG_TRAVERSE_CALLBACK_AT_FIRST
  { "ONIG_TRAVERSE_CALLBACK_AT_FIRST",          ONIG_TRAVERSE_CALLBACK_AT_FIRST },
#endif
#ifdef ONIG_TRAVERSE_CALLBACK_AT_LAST
  { "ONIG_TRAVERSE_CALLBACK_AT_LAST",           ONIG_TRAVERSE_CALLBACK_AT_LAST },
#endif
#ifdef ONIG_TRAVERSE_CALLBACK_AT_BOTH
  { "ONIG_TRAVERSE_CALLBACK_AT_BOTH",           ONIG_TRAVERSE_CALLBACK_AT_BOTH },
#endif
#ifdef ONIG_REGION_NOTPOS
  { "ONIG_REGION_NOTPOS",                       ONIG_REGION_NOTPOS },
#endif
#ifdef ONIG_CHAR_TABLE_SIZE
  { "ONIG_CHAR_TABLE_SIZE",                     ONIG_CHAR_TABLE_SIZE },
#endif
#ifdef ONIG_STATE_NORMAL
  { "ONIG_STATE_NORMAL",                        ONIG_STATE_NORMAL },
#endif
#ifdef ONIG_STATE_SEARCHING
  { "ONIG_STATE_SEARCHING",                     ONIG_STATE_SEARCHING },
#endif
#ifdef ONIG_STATE_COMPILING
  { "ONIG_STATE_COMPILING",                     ONIG_STATE_COMPILING },
#endif
#ifdef ONIG_STATE_MODIFY
  { "ONIG_STATE_MODIFY",                        ONIG_STATE_MODIFY },
#endif
  { NULL, 0 }
};

static flag_pair onig_error_flags[] = {
#ifdef ONIGERR_MEMORY
  { "ONIGERR_MEMORY",                                       ONIGERR_MEMORY },
#endif
#ifdef ONIGERR_TYPE_BUG
  { "ONIGERR_TYPE_BUG",                                     ONIGERR_TYPE_BUG },
#endif
#ifdef ONIGERR_PARSER_BUG
  { "ONIGERR_PARSER_BUG",                                   ONIGERR_PARSER_BUG },
#endif
#ifdef ONIGERR_STACK_BUG
  { "ONIGERR_STACK_BUG",                                    ONIGERR_STACK_BUG },
#endif
#ifdef ONIGERR_UNDEFINED_BYTECODE
  { "ONIGERR_UNDEFINED_BYTECODE",                           ONIGERR_UNDEFINED_BYTECODE },
#endif
#ifdef ONIGERR_UNEXPECTED_BYTECODE
  { "ONIGERR_UNEXPECTED_BYTECODE",                          ONIGERR_UNEXPECTED_BYTECODE },
#endif
#ifdef ONIGERR_MATCH_STACK_LIMIT_OVER
  { "ONIGERR_MATCH_STACK_LIMIT_OVER",                       ONIGERR_MATCH_STACK_LIMIT_OVER },
#endif
#ifdef ONIGERR_DEFAULT_ENCODING_IS_NOT_SETTED
  { "ONIGERR_DEFAULT_ENCODING_IS_NOT_SETTED",               ONIGERR_DEFAULT_ENCODING_IS_NOT_SETTED },
#endif
#ifdef ONIGERR_SPECIFIED_ENCODING_CANT_CONVERT_TO_WIDE_CHAR
  { "ONIGERR_SPECIFIED_ENCODING_CANT_CONVERT_TO_WIDE_CHAR", ONIGERR_SPECIFIED_ENCODING_CANT_CONVERT_TO_WIDE_CHAR },
#endif
#ifdef ONIGERR_INVALID_ARGUMENT
  { "ONIGERR_INVALID_ARGUMENT",                             ONIGERR_INVALID_ARGUMENT },
#endif
#ifdef ONIGERR_END_PATTERN_AT_LEFT_BRACE
  { "ONIGERR_END_PATTERN_AT_LEFT_BRACE",                    ONIGERR_END_PATTERN_AT_LEFT_BRACE },
#endif
#ifdef ONIGERR_END_PATTERN_AT_LEFT_BRACKET
  { "ONIGERR_END_PATTERN_AT_LEFT_BRACKET",                  ONIGERR_END_PATTERN_AT_LEFT_BRACKET },
#endif
#ifdef ONIGERR_EMPTY_CHAR_CLASS
  { "ONIGERR_EMPTY_CHAR_CLASS",                             ONIGERR_EMPTY_CHAR_CLASS },
#endif
#ifdef ONIGERR_PREMATURE_END_OF_CHAR_CLASS
  { "ONIGERR_PREMATURE_END_OF_CHAR_CLASS",                  ONIGERR_PREMATURE_END_OF_CHAR_CLASS },
#endif
#ifdef ONIGERR_END_PATTERN_AT_ESCAPE
  { "ONIGERR_END_PATTERN_AT_ESCAPE",                        ONIGERR_END_PATTERN_AT_ESCAPE },
#endif
#ifdef ONIGERR_END_PATTERN_AT_META
  { "ONIGERR_END_PATTERN_AT_META",                          ONIGERR_END_PATTERN_AT_META },
#endif
#ifdef ONIGERR_END_PATTERN_AT_CONTROL
  { "ONIGERR_END_PATTERN_AT_CONTROL",                       ONIGERR_END_PATTERN_AT_CONTROL },
#endif
#ifdef ONIGERR_META_CODE_SYNTAX
  { "ONIGERR_META_CODE_SYNTAX",                             ONIGERR_META_CODE_SYNTAX },
#endif
#ifdef ONIGERR_CONTROL_CODE_SYNTAX
  { "ONIGERR_CONTROL_CODE_SYNTAX",                          ONIGERR_CONTROL_CODE_SYNTAX },
#endif
#ifdef ONIGERR_CHAR_CLASS_VALUE_AT_END_OF_RANGE
  { "ONIGERR_CHAR_CLASS_VALUE_AT_END_OF_RANGE",             ONIGERR_CHAR_CLASS_VALUE_AT_END_OF_RANGE },
#endif
#ifdef ONIGERR_CHAR_CLASS_VALUE_AT_START_OF_RANGE
  { "ONIGERR_CHAR_CLASS_VALUE_AT_START_OF_RANGE",           ONIGERR_CHAR_CLASS_VALUE_AT_START_OF_RANGE },
#endif
#ifdef ONIGERR_UNMATCHED_RANGE_SPECIFIER_IN_CHAR_CLASS
  { "ONIGERR_UNMATCHED_RANGE_SPECIFIER_IN_CHAR_CLASS",      ONIGERR_UNMATCHED_RANGE_SPECIFIER_IN_CHAR_CLASS },
#endif
#ifdef ONIGERR_TARGET_OF_REPEAT_OPERATOR_NOT_SPECIFIED
  { "ONIGERR_TARGET_OF_REPEAT_OPERATOR_NOT_SPECIFIED",      ONIGERR_TARGET_OF_REPEAT_OPERATOR_NOT_SPECIFIED },
#endif
#ifdef ONIGERR_TARGET_OF_REPEAT_OPERATOR_INVALID
  { "ONIGERR_TARGET_OF_REPEAT_OPERATOR_INVALID",            ONIGERR_TARGET_OF_REPEAT_OPERATOR_INVALID },
#endif
#ifdef ONIGERR_NESTED_REPEAT_OPERATOR
  { "ONIGERR_NESTED_REPEAT_OPERATOR",                       ONIGERR_NESTED_REPEAT_OPERATOR },
#endif
#ifdef ONIGERR_UNMATCHED_CLOSE_PARENTHESIS
  { "ONIGERR_UNMATCHED_CLOSE_PARENTHESIS",                  ONIGERR_UNMATCHED_CLOSE_PARENTHESIS },
#endif
#ifdef ONIGERR_END_PATTERN_WITH_UNMATCHED_PARENTHESIS
  { "ONIGERR_END_PATTERN_WITH_UNMATCHED_PARENTHESIS",       ONIGERR_END_PATTERN_WITH_UNMATCHED_PARENTHESIS },
#endif
#ifdef ONIGERR_END_PATTERN_IN_GROUP
  { "ONIGERR_END_PATTERN_IN_GROUP",                         ONIGERR_END_PATTERN_IN_GROUP },
#endif
#ifdef ONIGERR_UNDEFINED_GROUP_OPTION
  { "ONIGERR_UNDEFINED_GROUP_OPTION",                       ONIGERR_UNDEFINED_GROUP_OPTION },
#endif
#ifdef ONIGERR_INVALID_POSIX_BRACKET_TYPE
  { "ONIGERR_INVALID_POSIX_BRACKET_TYPE",                   ONIGERR_INVALID_POSIX_BRACKET_TYPE },
#endif
#ifdef ONIGERR_INVALID_LOOK_BEHIND_PATTERN
  { "ONIGERR_INVALID_LOOK_BEHIND_PATTERN",                  ONIGERR_INVALID_LOOK_BEHIND_PATTERN },
#endif
#ifdef ONIGERR_INVALID_REPEAT_RANGE_PATTERN
  { "ONIGERR_INVALID_REPEAT_RANGE_PATTERN",                 ONIGERR_INVALID_REPEAT_RANGE_PATTERN },
#endif
#ifdef ONIGERR_TOO_BIG_NUMBER
  { "ONIGERR_TOO_BIG_NUMBER",                               ONIGERR_TOO_BIG_NUMBER },
#endif
#ifdef ONIGERR_TOO_BIG_NUMBER_FOR_REPEAT_RANGE
  { "ONIGERR_TOO_BIG_NUMBER_FOR_REPEAT_RANGE",              ONIGERR_TOO_BIG_NUMBER_FOR_REPEAT_RANGE },
#endif
#ifdef ONIGERR_UPPER_SMALLER_THAN_LOWER_IN_REPEAT_RANGE
  { "ONIGERR_UPPER_SMALLER_THAN_LOWER_IN_REPEAT_RANGE",     ONIGERR_UPPER_SMALLER_THAN_LOWER_IN_REPEAT_RANGE },
#endif
#ifdef ONIGERR_EMPTY_RANGE_IN_CHAR_CLASS
  { "ONIGERR_EMPTY_RANGE_IN_CHAR_CLASS",                    ONIGERR_EMPTY_RANGE_IN_CHAR_CLASS },
#endif
#ifdef ONIGERR_MISMATCH_CODE_LENGTH_IN_CLASS_RANGE
  { "ONIGERR_MISMATCH_CODE_LENGTH_IN_CLASS_RANGE",          ONIGERR_MISMATCH_CODE_LENGTH_IN_CLASS_RANGE },
#endif
#ifdef ONIGERR_TOO_MANY_MULTI_BYTE_RANGES
  { "ONIGERR_TOO_MANY_MULTI_BYTE_RANGES",                   ONIGERR_TOO_MANY_MULTI_BYTE_RANGES },
#endif
#ifdef ONIGERR_TOO_SHORT_MULTI_BYTE_STRING
  { "ONIGERR_TOO_SHORT_MULTI_BYTE_STRING",                  ONIGERR_TOO_SHORT_MULTI_BYTE_STRING },
#endif
#ifdef ONIGERR_TOO_BIG_BACKREF_NUMBER
  { "ONIGERR_TOO_BIG_BACKREF_NUMBER",                       ONIGERR_TOO_BIG_BACKREF_NUMBER },
#endif
#ifdef ONIGERR_INVALID_BACKREF
  { "ONIGERR_INVALID_BACKREF",                              ONIGERR_INVALID_BACKREF },
#endif
#ifdef ONIGERR_NUMBERED_BACKREF_OR_CALL_NOT_ALLOWED
  { "ONIGERR_NUMBERED_BACKREF_OR_CALL_NOT_ALLOWED",         ONIGERR_NUMBERED_BACKREF_OR_CALL_NOT_ALLOWED },
#endif
#ifdef ONIGERR_TOO_LONG_WIDE_CHAR_VALUE
  { "ONIGERR_TOO_LONG_WIDE_CHAR_VALUE",                     ONIGERR_TOO_LONG_WIDE_CHAR_VALUE },
#endif
#ifdef ONIGERR_EMPTY_GROUP_NAME
  { "ONIGERR_EMPTY_GROUP_NAME",                             ONIGERR_EMPTY_GROUP_NAME },
#endif
#ifdef ONIGERR_INVALID_GROUP_NAME
  { "ONIGERR_INVALID_GROUP_NAME",                           ONIGERR_INVALID_GROUP_NAME },
#endif
#ifdef ONIGERR_INVALID_CHAR_IN_GROUP_NAME
  { "ONIGERR_INVALID_CHAR_IN_GROUP_NAME",                   ONIGERR_INVALID_CHAR_IN_GROUP_NAME },
#endif
#ifdef ONIGERR_UNDEFINED_NAME_REFERENCE
  { "ONIGERR_UNDEFINED_NAME_REFERENCE",                     ONIGERR_UNDEFINED_NAME_REFERENCE },
#endif
#ifdef ONIGERR_UNDEFINED_GROUP_REFERENCE
  { "ONIGERR_UNDEFINED_GROUP_REFERENCE",                    ONIGERR_UNDEFINED_GROUP_REFERENCE },
#endif
#ifdef ONIGERR_MULTIPLEX_DEFINED_NAME
  { "ONIGERR_MULTIPLEX_DEFINED_NAME",                       ONIGERR_MULTIPLEX_DEFINED_NAME },
#endif
#ifdef ONIGERR_MULTIPLEX_DEFINITION_NAME_CALL
  { "ONIGERR_MULTIPLEX_DEFINITION_NAME_CALL",               ONIGERR_MULTIPLEX_DEFINITION_NAME_CALL },
#endif
#ifdef ONIGERR_NEVER_ENDING_RECURSION
  { "ONIGERR_NEVER_ENDING_RECURSION",                       ONIGERR_NEVER_ENDING_RECURSION },
#endif
#ifdef ONIGERR_GROUP_NUMBER_OVER_FOR_CAPTURE_HISTORY
  { "ONIGERR_GROUP_NUMBER_OVER_FOR_CAPTURE_HISTORY",        ONIGERR_GROUP_NUMBER_OVER_FOR_CAPTURE_HISTORY },
#endif
#ifdef ONIGERR_INVALID_CHAR_PROPERTY_NAME
  { "ONIGERR_INVALID_CHAR_PROPERTY_NAME",                   ONIGERR_INVALID_CHAR_PROPERTY_NAME },
#endif
#ifdef ONIGERR_INVALID_CODE_POINT_VALUE
  { "ONIGERR_INVALID_CODE_POINT_VALUE",                     ONIGERR_INVALID_CODE_POINT_VALUE },
#endif
#ifdef ONIGERR_INVALID_WIDE_CHAR_VALUE
  { "ONIGERR_INVALID_WIDE_CHAR_VALUE",                      ONIGERR_INVALID_WIDE_CHAR_VALUE },
#endif
#ifdef ONIGERR_TOO_BIG_WIDE_CHAR_VALUE
  { "ONIGERR_TOO_BIG_WIDE_CHAR_VALUE",                      ONIGERR_TOO_BIG_WIDE_CHAR_VALUE },
#endif
#ifdef ONIGERR_NOT_SUPPORTED_ENCODING_COMBINATION
  { "ONIGERR_NOT_SUPPORTED_ENCODING_COMBINATION",           ONIGERR_NOT_SUPPORTED_ENCODING_COMBINATION },
#endif
#ifdef ONIGERR_INVALID_COMBINATION_OF_OPTIONS
  { "ONIGERR_INVALID_COMBINATION_OF_OPTIONS",               ONIGERR_INVALID_COMBINATION_OF_OPTIONS },
#endif
#ifdef ONIGERR_OVER_THREAD_PASS_LIMIT_COUNT
  { "ONIGERR_OVER_THREAD_PASS_LIMIT_COUNT",                 ONIGERR_OVER_THREAD_PASS_LIMIT_COUNT },
#endif
  { NULL, 0 }
};

int LOnig_get_flags (lua_State *L) {
  const flag_pair* fps[] = { onig_flags, onig_error_flags, NULL };
  return get_flags (L, fps);
}

