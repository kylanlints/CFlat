#pragma once

#include <optional>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <functional>
#include <set>

//N_ = negative, O_ = open, C_ = closed, K_ = keyword K_D_ = discriptor keyword
enum class TokenType {IDENT, INT_LIT, N_INT_LIT, FLOAT_LIT, N_FLOAT_LIT, LONG_LIT, N_LONG_LIT, U_INT_LIT, 
                        U_LONG_LIT, SEMI, EQU, DOUB_EQU, ASTER, O_CURLY, C_CURLY, SLASH, PLUS, HYPHEN, 
                        PERCENT, O_PAREN, C_PAREN, EXCLAM, EXCLAM_EQU, CARAT, TILDE, LESS, GREATER, LESS_EQU, 
                        GREATER_EQU, DOUB_LESS, DOUB_GREATER, LESS_CARAT, CARAT_GREATER, AMPERSAND, 
                        DOUB_AMPERSAND, PIPE, DOUB_PIPE, K_INT, K_BOOL, K_CHAR, K_FLOAT, K_DOUBLE, K_LONG, 
                        K_SHORT, K_BYTE, K_IF, K_ELSE, K_EXIT, K_RETURN, K_D_CONST, K_D_UNSIGNED, K_FOR, 
                        K_ASM, K_ASM_DATA, K_ASM_BSS, K_ASM_RODATA};

const std::unordered_map<std::string, TokenType> keywords {
    {"int", TokenType::K_INT},
    {"bool", TokenType::K_BOOL},
    {"char", TokenType::K_CHAR},
    {"long", TokenType::K_LONG},
    {"short", TokenType::K_SHORT},
    {"byte", TokenType::K_BYTE},
    {"float", TokenType::K_FLOAT},
    {"double", TokenType::K_DOUBLE},
    {"if", TokenType::K_IF},
    {"else", TokenType::K_ELSE},
    {"for", TokenType::K_FOR},
    {"exit", TokenType::K_EXIT},
    {"return", TokenType::K_RETURN},
    {"unsigned", TokenType::K_D_UNSIGNED},
    {"const", TokenType::K_D_CONST},
    {"asm", TokenType::K_ASM},
    {"asm_data", TokenType::K_ASM_DATA},
    {"asm_bss", TokenType::K_ASM_BSS},
    {"asm_rodata", TokenType::K_ASM_RODATA}
};

const std::unordered_map<TokenType, std::string> debug_list {
    {TokenType::SEMI, ";"},
    {TokenType::EQU, "="},
    {TokenType::DOUB_EQU, "=="},
    {TokenType::ASTER, "*"},
    {TokenType::O_CURLY, "{"},
    {TokenType::C_CURLY, "}"},
    {TokenType::SLASH, "/"},
    {TokenType::PLUS, "+"},
    {TokenType::HYPHEN, "-"},
    {TokenType::PERCENT, "%"},
    {TokenType::O_PAREN, "("},
    {TokenType::C_PAREN, ")"},
    {TokenType::EXCLAM, "!"},
    {TokenType::EXCLAM_EQU, "!="},
    {TokenType::CARAT, "^"},
    {TokenType::TILDE, "~"},
    {TokenType::LESS, "<"},
    {TokenType::GREATER, ">"},
    {TokenType::LESS_EQU, "<="},
    {TokenType::GREATER_EQU, ">="},
    {TokenType::DOUB_LESS, "<<"},
    {TokenType::DOUB_GREATER, ">>"},
    {TokenType::LESS_CARAT, "<^"},
    {TokenType::CARAT_GREATER, "^>"},
    {TokenType::AMPERSAND, "&"},
    {TokenType::DOUB_AMPERSAND, "&&"},
    {TokenType::PIPE, "|"},
    {TokenType::DOUB_PIPE, "||"},
    {TokenType::K_INT, "int"},
    {TokenType::K_BOOL, "bool"},
    {TokenType::K_CHAR, "char"},
    {TokenType::K_LONG, "long"},
    {TokenType::K_SHORT, "short"},
    {TokenType::K_BYTE, "byte"},
    {TokenType::K_FLOAT, "float"},
    {TokenType::K_DOUBLE, "double"},
    {TokenType::K_IF, "if"},
    {TokenType::K_ELSE, "else"},
    {TokenType::K_FOR, "for"},
    {TokenType::K_EXIT, "exit", },
    {TokenType::K_EXIT, "return"},
    {TokenType::K_D_UNSIGNED, "unsigned"},
    {TokenType::K_D_CONST, "const"},
    {TokenType::K_ASM, "asm"},
    {TokenType::K_ASM_DATA, "asm_data"},
    {TokenType::K_ASM_BSS, "asm_bss"},
    {TokenType::K_ASM_RODATA, "asm_rodata"}
};

const std::set<TokenType> type_declr_kewords {TokenType::K_INT, TokenType::K_BOOL, TokenType::K_FLOAT, TokenType::K_DOUBLE, 
                                                TokenType::K_LONG, TokenType::K_CHAR, TokenType::K_SHORT, TokenType::K_BYTE};

struct Token {
    TokenType type;

    std::optional<std::string> value;

    u_int32_t line;

    u_int16_t char_on_line;
};

class Tokenizer {
private:
    const std::string m_src;
    int m_index = 0;
    int m_scope = 0;
    int m_max_scope = 0;
    std::vector<int> m_last_scopes;
    int m_bad_var_count = 0;
    int m_redefine_num = 0;
    std::hash<std::string> hasher;
    std::unordered_map<std::string, std::vector<int>> m_var_names;
    std::unordered_map<std::string, std::map<int, std::string>> m_var_renames;
    std::pair<std::string, std::string> last_rename;
    bool safe_to_rename = false;
    u_int32_t m_line = 1;
    u_int16_t m_char_on_line = 0;


    [[nodiscard]] bool next(int offset = 0) const
    {
        return (m_index + offset >= m_src.length()) ? false : true;
    }

    char consume()
    {
        m_char_on_line++;
        return m_src.at(m_index++);
    }

    void push_token(std::vector<Token>& tokens, TokenType type) {
        consume();
        tokens.push_back({.type = type, .line = m_line, .char_on_line = m_char_on_line});
    }

public:
    inline explicit Tokenizer(std::string src)
        : m_src(std::move(src)) {}

    inline std::vector<Token> Tokenize()
    {
        std::vector<Token> tokens;
        std::string buf;

        while (next()) {
            char c = m_src[m_index];
            if (std::isalpha(c)) {
                buf.push_back(consume());
                while (next() && (std::isalnum(m_src[m_index]) || m_src[m_index] == '_')) {
                    buf.push_back(consume());
                }

                if (keywords.contains(buf)) {
                    tokens.push_back({.type = keywords.at(buf), .line = m_line, .char_on_line = m_char_on_line});
                    buf.clear();
                } else {
                    if (!tokens.empty() && type_declr_kewords.contains(tokens.back().type)) {
                        if (m_var_names.contains(buf)) {
                            // this is by far the slowest part of the code so might change this later
                            // however it shouldnt hapen that much in normal code
                            bool is_bad = false;
                            for (int i : m_var_names.at(buf)){
                                if (i == m_scope) {
                                    std::cout << "Warning: cannot redefine variable at line: " << m_line << " character: " 
                                        << m_char_on_line << ", bad variable definition will be used" << std::endl;
                                    tokens.push_back({.type = TokenType::IDENT, .value = ("1___" + std::to_string(m_bad_var_count) + "___UNUSED___BAD___DEFINE___" + 
                                        std::to_string(hasher(buf))), .line = m_line, .char_on_line = m_char_on_line});
                                    m_bad_var_count++;
                                    buf.clear();
                                    is_bad = true;
                                    break;
                                }
                            }

                            if (!is_bad) {           // ↓ a number is necessary
                                    std::string name = "0" + buf + std::to_string(m_redefine_num);
                                    tokens.push_back({.type = TokenType::IDENT, .value = name, .line = m_line, .char_on_line = m_char_on_line});
                                    m_var_names.find(buf)->second.push_back(m_scope);
                                    bool rn = m_var_renames.contains(buf);
                                    if (rn) {
                                        m_var_renames.at(buf).insert({m_scope, name});
                                    } else {
                                        m_var_renames.insert({buf, std::map<int, std::string> {{m_scope, name}}});
                                    }
                                    m_redefine_num++;
                                    std::map<int, std::string> renames = m_var_renames.at(buf);
                                    last_rename = (rn && m_scope > 1) ? std::pair<std::string, std::string>{buf, (++renames.rbegin())->second} \
                                        : std::pair<std::string, std::string>{buf, buf};
                                    safe_to_rename = false;
                                    buf.clear();
                            }
                        } else {
                            tokens.push_back({.type = TokenType::IDENT, .value = buf, .line = m_line, .char_on_line = m_char_on_line});
                            m_var_names.insert({buf, std::vector<int> {m_scope}});
                            buf.clear();
                        }
                    } else {
                        if (m_var_renames.contains(buf) && m_scope) {
                            if (!last_rename.first.compare(buf)) {
                                if (safe_to_rename) {
                                    tokens.push_back({.type = TokenType::IDENT, .value = m_var_renames.at(buf).at(m_scope), .line = m_line, .char_on_line = m_char_on_line});
                                } else {
                                    tokens.push_back({.type = TokenType::IDENT, .value = last_rename.second, .line = m_line, .char_on_line = m_char_on_line});
                                }
                            } else {
                                tokens.push_back({.type = TokenType::IDENT, .value = m_var_renames.at(buf).at(m_scope), .line = m_line, .char_on_line = m_char_on_line});
                            }
                            buf.clear();
                        } else {
                            tokens.push_back({.type = TokenType::IDENT, .value = buf, .line = m_line, .char_on_line = m_char_on_line});
                            buf.clear();
                        }
                    }
                }
            } else if (std::isdigit(c)) {
                parseLiteral(buf, tokens, true);
            } else switch(c) { // Main characters like ;, (, ), *, or +
                case ' ':
                    consume();
                    break;
                case 10:
                    m_index++;
                    m_line++;
                    m_char_on_line = 0;
                    safe_to_rename = true;
                    break;
                case 9:
                    consume();
                    break;
                case ';':
                    push_token(tokens, TokenType::SEMI);
                    break;
                case '=':
                    if(next() && m_src[m_index + 1] == '=') {
                        consume();
                        push_token(tokens, TokenType::DOUB_EQU);
                    } else {
                        push_token(tokens, TokenType::EQU);
                    }
                    break;
                case '(':
                    push_token(tokens, TokenType::O_PAREN);
                    break;
                case ')':
                    push_token(tokens, TokenType::C_PAREN);
                    break;
                case '{':
                    push_token(tokens, TokenType::O_CURLY);
                    m_last_scopes.push_back(m_scope);
                    m_max_scope++;
                    m_scope = m_max_scope;
                    break;
                case '}':
                    push_token(tokens, TokenType::C_CURLY);
                    m_scope = m_last_scopes.back();
                    m_last_scopes.pop_back();
                    break;
                case '*':
                    push_token(tokens, TokenType::ASTER);
                    break;
                case '+':
                    push_token(tokens, TokenType::PLUS);
                    break;
                case '-':
                {
                    TokenType last_token = tokens.back().type;
                    if (next() && last_token != TokenType::IDENT && last_token != TokenType::INT_LIT && 
                        last_token != TokenType::N_INT_LIT && last_token != TokenType::FLOAT_LIT && 
                        last_token != TokenType::N_FLOAT_LIT && std::isdigit(m_src[m_index + 1])) { //negative numbers, will be converted to twos-complement later
                        consume();
                        parseLiteral(buf, tokens, false);
                    } else {
                        push_token(tokens, TokenType::HYPHEN);
                    }
                    break;
                }
                case '/':
                    if(next() && m_src[m_index + 1] == '/') {
                        do { //Comments
                            m_index++;
                        } while(next() && m_src[m_index] != 10);
                    } else if (next() && m_src[m_index + 1] == '*') {
                        do { //Extended commments
                            m_index++;
                            if (m_src[m_index] == 10) m_line++;
                        } while(next() && (m_src[m_index] != '*' || m_src[m_index + 1] != '/'));
                        m_index += 2;
                    } else {
                        push_token(tokens, TokenType::SLASH);
                    }
                    break;
                case '%':
                    push_token(tokens, TokenType::PERCENT);
                case '!':
                    if(next() && m_src[m_index + 1] == '=') {
                        consume();
                        push_token(tokens, TokenType::EXCLAM_EQU);
                    } else {
                        push_token(tokens, TokenType::EXCLAM);
                    }
                    break;
                case '>':
                    if(next() && m_src[m_index + 1] == '=') {
                        consume();
                        push_token(tokens, TokenType::GREATER_EQU);
                    } else if (next() && m_src[m_index + 1] == '>') {
                        consume();
                        push_token(tokens, TokenType::DOUB_GREATER);
                    } else {
                        push_token(tokens, TokenType::GREATER);
                    }
                    break;
                case '<':
                    if(next() && m_src[m_index + 1] == '=') {
                        consume();
                        push_token(tokens, TokenType::LESS_EQU);
                    } else if (next() && m_src[m_index + 1] == '<') {
                        consume();
                        push_token(tokens, TokenType::DOUB_LESS);
                    } else if (next() && m_src[m_index + 1] == '^') {
                        consume();
                        push_token(tokens, TokenType::LESS_CARAT);
                    } else {
                        push_token(tokens, TokenType::LESS);
                    }
                    break;
                case '^':
                    if (next() && m_src[m_index + 1] == '>') {
                        consume();
                        push_token(tokens, TokenType::CARAT_GREATER);
                    } else {
                        push_token(tokens, TokenType::CARAT);
                    }
                    break;
                case '~':
                    push_token(tokens, TokenType::TILDE);
                    break;
                case '&':
                    if(next() && m_src[m_index + 1] == '&') {
                        consume();
                        push_token(tokens, TokenType::DOUB_AMPERSAND);
                    } else {
                        push_token(tokens, TokenType::AMPERSAND);
                    }
                    break;
                case '|':
                    if(next() && m_src[m_index + 1] == '|') {
                        consume();
                        push_token(tokens, TokenType::DOUB_PIPE);
                    } else {
                        push_token(tokens, TokenType::PIPE);
                    }
                    break;
                default:
                    std::cout << "Unsupported character or bad use of character" << " at line: " << m_line << " character: " << m_char_on_line << std::endl;
                    exit(EXIT_FAILURE);
            }
        }

        return std::move(tokens);
    }
    inline void parseLiteral(std::string& buf, std::vector<Token>& tokens, bool positive) {
        TokenType type = positive ? TokenType::INT_LIT : TokenType::N_INT_LIT;
        buf.push_back(consume());
        while (next() && std::isdigit(m_src[m_index])) {
            buf.push_back(consume());
        }
        if (next() && m_src[m_index] == '.') {
            type = positive ? TokenType::FLOAT_LIT : TokenType::N_FLOAT_LIT;
            buf.push_back(consume());
            while (next() && std::isdigit(m_src[m_index])) {
                buf.push_back(consume());
            }
        }
        if (next()) {
            switch (m_src[m_index]) {
                case 'f':
                case 'd':
                    m_index++;
                    type = positive ? TokenType::FLOAT_LIT : TokenType::N_FLOAT_LIT;
                    break;
                case 'l':
                    m_index++;
                    type = positive ? TokenType::LONG_LIT : TokenType::N_LONG_LIT;
                    break;
                case 'u':
                    m_index++;
                    if (next() && m_src[m_index] == 'l') {
                        m_index++;
                        type = TokenType::U_LONG_LIT;
                        break;
                    }
                type = TokenType::U_INT_LIT;
            }
        }

        tokens.push_back({.type = type, .value = buf, .line = m_line, .char_on_line = m_char_on_line});
        buf.clear();
    }
};

