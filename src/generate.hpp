#pragma once

#include <sstream>
#include <fmt/format.h>
#include <map>
#include <cmath>
#include "parser.hpp"
#include "divmagic.h"

#define RED_ZONE_BYTES 120

// not including rax
#define QWORD_REGS {"rcx", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rbx"}
#define DWORD_REGS {"ecx", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", "ebx"}
#define XMM_REGS {"xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"}
#define NV_QWORD_REGS {"r11", "r12", "r13", "r14", "r15", "rbx"} // not volatile
// word and byte registers are inefficient on modern cpus
// secondary reg used for division and moving large numbers is rdx

#define MAX_REG_NUM_MINUS_ONE 11
#define RCX_POS 1

enum class VarSize {BYTE, WORD, DWORD, QWORD};
enum class OpType {SINT, INT, SLONG, LONG, FLOAT, DOUBLE};

const std::string QWORDS[] = QWORD_REGS;
const std::string DWORDS[] = DWORD_REGS;
const std::string XMMS[] = XMM_REGS;
const std::string NV_QWORDS[] = NV_QWORD_REGS;

struct cmp_char_star {
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) < 0;
    }
};

struct IfExprInf {
    bool jump = false;
    char label = false;
};

// the use of use_e is to help the compiler
// optimize parenthetical statements by avoiding
// an extra mov statement. If the compiler should
// do this, use_e = 0 (!use_e is true). If the
// compiler has performed the optimization,
// use_e is set to -1 (use_e < 0)
struct ExprInf {
    std::string mov;
    std::string reg;
    long use_e = -1;
    char o_rax = 0;
    bool two_part = false;
    bool uses_reg = true;
    IfExprInf if_inf;
    OpType opType;
    VarType varType;
};

static ExprInf empty {.mov = "", .reg = "", .uses_reg = false, .opType = OpType::INT};
static ExprInf comma {.mov = ", ", .reg = "", .uses_reg = false, .opType = OpType::INT};

class Generator {
public:
    std::stringstream m_output;
    std::stringstream m_section_data;
    std::stringstream m_section_rodata;
    std::stringstream m_section_bss;
    std::stringstream m_output_end;

    inline explicit Generator(const NodeRoot root)
        : m_root(std::move(root))
    {     
        m_output << "section .text\n";
        m_section_data << "section .data\n";
        m_section_rodata << "section .rodata\n";
        m_section_bss << "section .bss\n";
    }
    
    [[nodiscard]] std::string gen_assem() {
        m_output << "global _start\n_start:\n";
        
        for (const NodeStmt* stmt : m_root.stmts) {
            gen_stmt(stmt);
        }

        m_output << "  mov rax, 60\n  xor rdi, rdi\n  syscall\n"; //exit with 0 if no previous exit statement

        return (m_section_data.str() + m_section_rodata.str() + m_section_bss.str() + m_output.str() + m_output_end.str());
    }

    const inline void gen_stmt(const NodeStmt* stmt, bool last = false) {
        std::visit([this, &last](auto&& arg) {stmt_Generator.gen(arg, last);}, stmt->stmt);
    }

    const inline void gen_expr(const NodeExpr* expr, ExprInf& information) {
        std::visit([this, &information](auto&& arg) {expr_Generator.gen(arg, information);}, expr->var);
    }

    const inline void gen_bin_expr(const NodeExprBin* bin_expr, ExprInf& information) {
        if (information.use_e > 0) information.use_e--;
        std::visit([this, &information](auto&& arg) {bin_expr_Generator.gen(arg, information);}, bin_expr->expr);
    }

    const inline void gen_log_expr(const NodeExprLog* log_expr, ExprInf information) {
        if (information.use_e > 0) information.use_e--;
        std::visit([this, &information](auto&& arg) {log_expr_Generator.gen(arg, information);}, log_expr->var);
    }

    const inline void gen_term(const NodeTerm* term, ExprInf& prefix) {
        std::visit([this, &prefix](auto&& arg) {term_Generator.gen(arg, prefix);}, term->var);
    }

    const inline void gen_empty_term(const NodeTerm* term, OpType opType) {
        empty.opType = opType;
        std::visit([this](auto&& arg) {term_Generator.gen(arg, empty);}, term->var);
    }

    inline void push(const std::string reg) {
        m_output << "  push " << reg << "\n";
    }

    inline void pop(const std::string reg) {
        m_output << "  pop " << reg << "\n";
    }

    template <typename T>
    inline void gen_standard_comparison(const T* comp, bool rh_first, ExprInf& prefix, const std::string cmp_mem_or_reg) {
        if (!rh_first) {
            m_output << get_cmp_op(prefix.opType, false) + cmp_mem_or_reg + ", ";
            gen_empty_term(std::get<NodeTerm*>(comp->rh->var), prefix.opType);
        } else {
            m_output << get_cmp_op(prefix.opType, false) + cmp_mem_or_reg + ", ";
            m_output << get_last_reg(prefix.opType) + '\n';
        }
    }

    inline void bool_set_xmm(ExprInf& prefix, const std::string j_instruc, const std::string parity_jump_stmt) {
        std::string float_mem = prefix.opType == OpType::FLOAT ? add_float_num(0x3F800000, "  dd  ") : add_float_num(0x3FF0000000000000, "  dq  ");
        std::string float_label = " .FL" + std::to_string(m_extra_label_cnt);
        size_t mid_float_set_comp_label = m_extra_label_cnt + 1;
        m_output << parity_jump_stmt + float_label + '\n' + "  j" + j_instruc + float_label + '\n' + prefix.mov + prefix.reg + "DWORD [" \
            + float_mem + "]\n.FL" + std::to_string(mid_float_set_comp_label) + ":\n";
        std::string reg_wc = get_before_comma(prefix.reg);
        m_output_end << ".FL" + std::to_string(m_extra_label_cnt) + ":\n" + "  xorps" + reg_wc + ',' + reg_wc + "\n  jmp .FL" + std::to_string(mid_float_set_comp_label) + '\n';
        m_extra_label_cnt += 2;
    }

    //TODO: in float comparisons were both sides return an integer and one of them is not could technically be optimized to avoid a redundant mov
    template <typename T>
    inline bool check_float_set_comp(const T* comp, ExprInf& prefix, const std::string true_op, const std::string false_op, 
                                        const std::string parity_jump_stmt) {
        std::string instruc;
        if (!prefix.if_inf.jump) {
            instruc = false_op;
            if (comp->num_result == CompareResult::FORCE_INT_NUM) {
                cmov_reg(instruc, "eax", "al", "edx", "setp", "1");
                return true;
            } else if (comp->num_result == CompareResult::SUB_RH_NUM) {
                cmov_reg(instruc, "edx", "dl", get_next_reg(OpType::INT), "setnp", "0");
                return true;
            }
        } else {
            instruc = true_op;
            if (comp->num_result == CompareResult::FORCE_INT_NUM) {
                cmov_reg(instruc, "eax", "al", "edx", "setnp", "0");
                return true;
            } else if (comp->num_result == CompareResult::SUB_RH_NUM) {
                cmov_reg(instruc, "edx", "dl", get_next_reg(OpType::INT), "setp", "1");
                return true;
            }
        }
        if (comp->num_result == CompareResult::SUB_NUM) {
            bool_set_xmm(prefix, instruc, parity_jump_stmt);
            return true;
        }
        return false;
    }
    
    template <typename T>
    inline std::string get_lh_if_comp(const T* comp, ExprInf& prefix) {
        if (comp->lh_ident && !comp->rh_ident) {
            return ' ' + get_address(std::get<NodeTermIdent*>(std::get<NodeTerm*>(comp->lh->var)->var)->ident, prefix);
        } else {
            gen_expr(comp->lh, prefix);
            return get_before_comma(get_a_reg(prefix.opType));
        }
    }

    template <typename T>
    inline std::string get_lh_if_comp_no_rh(const T* comp, ExprInf& prefix) {
        if (comp->lh_ident) {
            return ' ' + get_address(std::get<NodeTermIdent*>(std::get<NodeTerm*>(comp->lh->var)->var)->ident, prefix);
        } else {
            gen_expr(comp->lh, prefix);
            return get_before_comma(get_a_reg(prefix.opType));
        }
    }

    template <typename T>
    inline void gen_if_compare_expr(const T* comp, bool rh_first, ExprInf& prefix, const std::string false_op, const std::string true_op) {
        std::string cmp_mem_or_reg;
        if (prefix.opType == OpType::FLOAT || prefix.opType == OpType::DOUBLE) {
            if constexpr (std::is_same_v<T, NodeExprLogCompareLess> || std::is_same_v<T, NodeExprLogCompareLessEqu> || 
                            std::is_same_v<T, NodeExprLogCompareLessByPar> || std::is_same_v<T, NodeExprLogCompareLessEquByPar>) {
                cmp_mem_or_reg = get_lh_if_comp_no_rh(comp, prefix);
            } else if constexpr (std::is_same_v<T, NodeExprLogCompareGreater> || std::is_same_v<T, NodeExprLogCompareGreaterEqu> ||
                        std::is_same_v<T, NodeExprLogCompareGreaterByPar> || std::is_same_v<T, NodeExprLogCompareGreaterEquByPar>) {
                gen_expr(comp->lh, prefix);
                cmp_mem_or_reg = get_before_comma(get_a_reg(prefix.opType));
            } else if constexpr (std::is_same_v<T, NodeExprLogCompareEqu> || std::is_same_v<T, NodeExprLogCompareNotEqu>) {
                gen_expr(comp->lh, prefix);
                cmp_mem_or_reg = get_before_comma(get_a_reg(prefix.opType));
            } else {
                cmp_mem_or_reg = get_lh_if_comp_no_rh(comp, prefix);
            }
        } else {
            if constexpr (std::is_same_v<T, NodeExprLogCompareEqu> || std::is_same_v<T, NodeExprLogCompareNotEqu> ||
                            std::is_same_v<T, NodeExprLogCompareGreater> || std::is_same_v<T, NodeExprLogCompareGreaterEqu> || 
                            std::is_same_v<T, NodeExprLogCompareLess> || std::is_same_v<T, NodeExprLogCompareLessEqu>) {
                cmp_mem_or_reg = get_lh_if_comp(comp, prefix);
            } else {
                cmp_mem_or_reg = get_lh_if_comp_no_rh(comp, prefix);
            }
        }
        
        std::string set_stmt;
        bool jump_stmt_bool = prefix.if_inf.jump;
        const std::string jump_stmt = "  j" + (jump_stmt_bool ? true_op : false_op);
        const std::string parity_jump_stmt = "  jp";
        std::string label_ident;
        size_t label;
        if (static_cast<char>(comp->num_result)) {
            label = m_extra_label_cnt;
            label_ident = " .FL";
            set_stmt = "  set" + ((comp->num_result == CompareResult::SUB_RH_NUM) ? false_op : true_op);
        } else if (prefix.if_inf.label >= 2) {
            label = m_extra_label_cnt + prefix.if_inf.label - 2;
            label_ident = " .EL";
            m_use_extra_label++;
        } else if (prefix.if_inf.label == 1) {
            label = m_label_cnt + 1;
            label_ident = " .L";
            m_use_truth_label = true;
        } else {
            label = m_label_cnt_2;
            label_ident = " .L";
        }
        
        bool compared_as_int = false;
        if constexpr (std::is_same_v<T, NodeExprLogCompareEquByPar> || std::is_same_v<T, NodeExprLogCompareNotEquByPar> ||
                        std::is_same_v<T, NodeExprLogCompareGreaterByPar> || std::is_same_v<T, NodeExprLogCompareGreaterEquByPar> || 
                        std::is_same_v<T, NodeExprLogCompareLessByPar> || std::is_same_v<T, NodeExprLogCompareLessEquByPar>) {
            if (static_cast<char>(comp->comp_as_int)) {
                gen_expr(comp->rh, prefix);
                compared_as_int = true;
            }
        } else if ((prefix.opType == OpType::FLOAT || prefix.opType == OpType::DOUBLE) && !compared_as_int) {
            if constexpr (std::is_same_v<T, NodeExprLogCompareEqu> || std::is_same_v<T, NodeExprLogCompareEquByPar>) {
                size_t parity_jump;
                std::string parity_label_ident;
                if (m_encapsulated_if_or > 0) {
                    parity_jump = m_extra_label_cnt;
                    parity_label_ident = " .EL";
                    if (prefix.if_inf.label >= 2) {
                        label++;
                    }
                    m_use_mid_or_label++;
                } else if (m_encapsulated_if_and > 1) {
                    parity_jump = m_extra_label_cnt;
                    parity_label_ident = " .EL";
                    if (prefix.if_inf.label < 2) {
                        m_use_extra_label++;
                    }
                } else {
                    parity_jump = m_label_cnt_2;
                    parity_label_ident = " .L";
                } 
        
                // jump if parity tests for NaN state which is set to false (PF isn't fully useless!)
                m_output << get_cmp_op(prefix.opType, true) + cmp_mem_or_reg + ", ";
                if (!rh_first) {
                    gen_empty_term(std::get<NodeTerm*>(comp->rh->var), prefix.opType);
                } else {
                    m_output << get_last_reg(prefix.opType) + '\n';
                }
                if (!check_float_set_comp<T>(comp, prefix, true_op, false_op, parity_jump_stmt)) {
                    if (!(static_cast<char>(comp->num_result))) {
                        m_output << parity_jump_stmt + parity_label_ident + std::to_string(parity_jump) + '\n';
                    }
                } else return;
            } else if constexpr (std::is_same_v<T, NodeExprLogCompareLess> || std::is_same_v<T, NodeExprLogCompareLessEqu> ||
                                std::is_same_v<T, NodeExprLogCompareLessByPar> || std::is_same_v<T, NodeExprLogCompareLessEquByPar>) {
                if (!rh_first) {
                    std::string prev = prefix.reg;
                    std::string next_reg = ' ' + get_next_reg(prefix.opType);
                    prefix.reg = next_reg + ", ";
                    gen_empty_term(std::get<NodeTerm*>(comp->rh->var), prefix.opType);
                    prefix.reg = prev;
                    m_output << get_cmp_op(prefix.opType, false) + next_reg + ',' + cmp_mem_or_reg + '\n';
                } else {
                    m_output << get_cmp_op(prefix.opType, false) + get_last_reg(prefix.opType) + ',' + cmp_mem_or_reg + '\n';
                }
                if (check_float_set_comp<T>(comp, prefix, true_op, false_op, parity_jump_stmt)) return;
            } else {
                gen_standard_comparison(comp, rh_first, prefix, cmp_mem_or_reg);
                if (check_float_set_comp<T>(comp, prefix, true_op, false_op, parity_jump_stmt)) return;
            }
        } else if (!compared_as_int) {
            gen_standard_comparison(comp, rh_first, prefix, cmp_mem_or_reg);
        }

        if constexpr (std::is_same_v<T, NodeExprLogCompareEquByPar> || std::is_same_v<T, NodeExprLogCompareNotEquByPar> ||
                        std::is_same_v<T, NodeExprLogCompareGreaterByPar> || std::is_same_v<T, NodeExprLogCompareGreaterEquByPar> || 
                        std::is_same_v<T, NodeExprLogCompareLessByPar> || std::is_same_v<T, NodeExprLogCompareLessEquByPar>) {
            if (comp->comp_as_int) {
                if (static_cast<char>(comp->num_result)) {
                    m_output << "  xor eax, edx\n";
                    m_output << "  movzx eax, al\n";
                    //TODO: when implementing casts this might be able to be improved
                    if (prefix.opType == OpType::FLOAT) {
                        m_output << "  xorps xmm0, xmm0\n  cvtsi2ss xmm0, eax\n";
                    } else if (prefix.opType == OpType::DOUBLE) {
                        m_output << "  xorpx xmm0, xmm0\n  cvtsi2sd xmm0, rax\n";
                    }
                } else {
                    m_output << "  cmp al, dl\n  j" + true_op + label_ident + std::to_string(label) + '\n';
                }
                return;
            }
        } 
        
        if (!static_cast<char>(comp->num_result)) {
            m_output << jump_stmt + label_ident + std::to_string(label) + '\n';
        } else {
            if (prefix.opType != OpType::FLOAT && prefix.opType != OpType::DOUBLE) {
                std::string byte_reg = comp->num_result == CompareResult::SUB_RH_NUM ? " dl" : " al";
                m_output << set_stmt + byte_reg + '\n';
                if (comp->num_result == CompareResult::SUB_NUM || comp->num_result == CompareResult::FINAL_NUM) {
                    m_output << "  movzx eax, al\n";
                }
            } else {
                bool_set_xmm(prefix, prefix.if_inf.jump ? true_op : false_op, parity_jump_stmt);
            }
        }
    }

    template <typename T>
    inline void gen_if_compare_expr_by_par(const T* comp, ExprInf& prefix, std::string sf, std::string st, std::string uf, 
                                                std::string ut, std::string uff, std::string uft) {
        std::pair<std::string, std::string> ops = get_jump_op(prefix.opType, sf, st, uf, ut, uff, uft);
        if (!comp->comp_as_int) {
            gen_paren_start<T>(comp, prefix);
            gen_if_compare_expr<T>(comp, true, prefix, ops.first, ops.second);
            m_used_regs--;
        } else {
            gen_if_compare_expr<T>(comp, true, prefix, ops.first, ops.second);
        }
    }

    size_t gen_if_statement_start(const NodeExpr* if_expr, ExprInf& inf, bool last_no_else) {
        if (last_no_else) {
            m_label_cnt_2 -= m_label_backtrack - 2;
            gen_expr(if_expr, inf);
            m_label_cnt_2 += m_label_backtrack - 2;
        } else {
            gen_expr(if_expr, inf);
        }
        size_t label = m_label_cnt;
        m_label_cnt += 3;
        m_label_cnt_2 += 3;
        if (m_use_truth_label) {
            add_label(m_label_cnt - 2); 
            m_use_truth_label = false;
        }
        m_label_backtrack += 3;

        return label;
    }

    void gen_while_statement(const NodeExpr* if_expr, const NodeStmt* stmts, const std::optional<NodeStmt*> end_stmt, const std::string end_label, ExprInf& inf, bool last) {
        add_label(m_label_cnt);
        m_loop_labels.push_back(std::pair<std::string, std::string>{".L" + std::to_string(m_label_cnt), end_label});
        size_t prev_label_cnt = m_label_cnt;
        size_t prev_label_cnt_2 = m_label_cnt_2;
        size_t prev_label_backtrack = m_label_backtrack;
        m_label_cnt += 3;
        m_label_cnt_2 += 3;
        m_label_backtrack += 3;
        gen_stmt(stmts, false); //Should not be last
        if (end_stmt.has_value()) {
            gen_stmt(end_stmt.value());
        }
        inf.if_inf.jump = true; // Since we want to jump on true instead of false this adds a not
        size_t after_label_cnt = m_label_cnt;
        size_t after_label_cnt_2 = m_label_cnt_2;
        m_label_cnt = prev_label_cnt;
        m_label_cnt_2 = prev_label_cnt_2;
        if (last) {
            m_label_cnt_2 -= prev_label_backtrack - 2;
            gen_expr(if_expr, inf);
            m_label_cnt_2 += prev_label_backtrack - 2;
        } else {
            gen_expr(if_expr, inf);
        }
        if (m_use_truth_label) {
            add_label(m_label_cnt + 1); 
            m_use_truth_label = false;
        }
        m_label_cnt = after_label_cnt;
        m_label_cnt_2 = after_label_cnt_2;
    }

    // returns true if it successfully generated assembly
    bool optimize_mul(int64_t num, bool lh_paren, std::string add, std::string sub, bool is_imul, bool is_float, bool rh_first, const NodeExpr* term, ExprInf& inf) {
        bool neg;
        if (!num) {
            std::string reg = !inf.use_e ? " " + get_next_reg(inf.opType) + ", " : inf.reg;
            m_output << inf.mov + reg + "0\n  nop\n";
            if (!inf.use_e) {
                disable_use_e(inf, reg);
            }
            return true;
        } else if (num == 1 || ((inf.opType == OpType::FLOAT && num == 0x3F800000) || (inf.opType == OpType::DOUBLE && num == 0x3FF0000000000000))) {
            if (!lh_paren && !inf.use_e) {
                if (!rh_first) {
                    gen_term_e(term, inf, " " + get_next_reg(inf.opType) + ", ");
                }
            } else {
                gen_expr_or_mov_reg(rh_first, term, inf);
            }
            return true;
        } else if (num == 3 || ((inf.opType == OpType::FLOAT && num == 0x40400000) || (inf.opType == OpType::DOUBLE && num == 0x4008000000000000))) {
            std::string reg_wc = get_before_comma(inf.reg);
            std::string next_reg = double_reg(rh_first, term, inf, reg_wc);
            m_output << add + reg_wc + ',' + reg_wc + '\n';
            do_instruc(inf, add, next_reg, reg_wc);
            return true;
        } else if (is_float) {
            if (num == 2) {
                std::string reg_wc = get_before_comma(inf.reg);
                m_output << add + reg_wc + ',' + reg_wc + '\n';
                return true;
            } else return false;
        } else if (num == -1) {
            if (!lh_paren && !inf.use_e) {
                if (!rh_first) {
                    gen_term_e(term, inf, " " + get_next_reg(inf.opType) + ", ");
                }
            } else {
                gen_expr_or_mov_reg(rh_first, term, inf);
            }
            m_output << "  neg" + inf.reg + '\n';
            return true;
        } else if (num < 0) {
            num = -num;
            neg = true;
        } if (!(num & (num - 1))) { // is num even
            if (!lh_paren && !inf.use_e) {
                if (!rh_first) {
                    gen_term_e(term, inf, " " + get_next_reg(inf.opType) + ", ");
                }
                m_output << "  shl " + get_next_reg(inf.opType) + ", " + std::to_string(63-__builtin_clzll(num)) + '\n';
                if (neg) {
                    m_output << "  neg" + get_next_reg(inf.opType) + '\n';
                } 
            } else {
                gen_expr_or_mov_reg(rh_first, term, inf);
                m_output << "  shl" + inf.reg + std::to_string(63-__builtin_clzll(num)) + '\n';
                if (neg) {
                    m_output << "  neg" + get_before_comma(inf.reg) + '\n';
                } 
            }
            return true;
        } else if (!((num + 1) & (num))) { // one under even
            std::string reg_wc = get_before_comma(inf.reg);
            std::string next_reg = double_reg(rh_first, term, inf, reg_wc);
            std::string reg = !inf.use_e ? next_reg : reg_wc;
            m_output << "  shl " + reg + ", " + std::to_string(63-__builtin_clzll(num+1)) + '\n';
            do_instruc(inf, sub, next_reg, reg_wc);
            if (neg) {
                m_output << "  neg" + reg + '\n';
            }
            return true;
        } else if (!((num - 1) & (num - 2))) { // one over even
            std::string reg_wc = get_before_comma(inf.reg);
            std::string next_reg = double_reg(rh_first, term, inf, reg_wc);
            std::string reg = !inf.use_e ? next_reg : reg_wc;
            m_output << "  shl" + reg + ", " + std::to_string(63-__builtin_clzll(num)) + '\n';
            do_instruc(inf, add, next_reg, reg_wc);
            if (neg) {
                m_output << "  neg" + reg;
            } return true;
        } else if (neg) {
            num = -num;
        } if (is_imul && !inf.use_e) {
            gen_expr_or_mov_reg(rh_first, term, inf);
            std::string next_reg = get_next_reg(inf.opType);
            m_output << "  imul " + next_reg + "," + inf.reg + std::to_string(num);
            inf.use_e = false;
            return true;
        } else return false;
    }

    // only can optimize integer division
    bool optimize_div(const int64_t num, bool is_signed, bool is_long, bool is_float, const NodeExpr* term, ExprInf& inf) {
        if (!num) {
            std::cout << "Cannot div by zero, ignoring";
            m_output << "  nop ;bad div by\n";
            return true;
        } else if (num == 1) {
            gen_expr(term, inf);
            return true;
        } else if (is_float) {
            return false;
        } else if (!(num & (num - 1))) { // even
            if (is_signed) {
                if (num == 2) {
                    std::string reg_wc = get_before_comma(inf.reg);
                    std::string next_reg = double_reg(false, term, inf, reg_wc);
                    if (inf.opType == OpType::SLONG) {
                        m_output << "  shr " + next_reg + ", 0x3f\n"; // sign bit
                    } else {
                        m_output << "  shr " + next_reg + ", 0x1f\n"; // sign bit
                    }
                    bool use_e = inf.use_e;
                    do_instruc(inf, "  add", next_reg, reg_wc);
                    if (use_e) {
                        m_output << "  sar" + inf.reg + "1\n";
                    } else {
                        m_output << "  sar " + next_reg + ", " + "1\n";
                    }
                    return true;
                } else {
                    std::string prim;
                    std::string sec;
                    gen_expr(term, inf);
                    prim = get_before_comma(inf.reg);
                    sec = get_next_reg(inf.opType);

                    m_output << "  lea " + sec + ", es:[" + prim + " + " + fmt::format("0x{:x}", num - 1) + " ]\n" + \
                                "  test" + prim + "," + prim + '\n' +                                          \
                                "  cmovs" + prim + ", " + sec + '\n' + /* conditional move from SF flag */      \
                                "  sar" + prim + ", " + std::to_string(63-__builtin_clzll(num)) + '\n';
                    return true;
                }
            } else {
                std::string even_div_reg;
                if (!inf.use_e) {
                    even_div_reg = ' ' + get_next_reg(inf.opType) + ", ";
                    gen_term_e(term, inf, even_div_reg);
                } else {
                    even_div_reg = inf.reg;
                    gen_expr(term, inf);
                }
                m_output << "  shr" + even_div_reg + std::to_string(63-__builtin_clzll(num)) + '\n';
                return true;
            }
        } else if (!is_long) {
            if (is_signed) {
                gen_expr(term, inf);
                m_output << "  movsxd rdx, eax\n";

                // magic
                divmag::magicst mag = divmag::magic(static_cast<int>(num));

                // signed number is so large it becomes negative, happens with divide by 7 and 14
                if (mag.M < 0) {
                    m_output << "  mov rcx, " + std::to_string(static_cast<unsigned int>(mag.M)) + "\n"
                                "  imul rdx, rcx\n";
                } else {
                    m_output << "  imul rdx, " + std::to_string(mag.M) + "\n";
                }

                m_output << "  sar rdx, " + std::to_string(32 + mag.s) + "\n";
                if (num > 0) {
                    m_output << "  mov ecx, edx\n"
                                "  cdq\n"                                        // add one
                                "  mov eax, ecx\n"                               // if dividend
                                "  sub eax, edx\n";                              // is negative
                } else {
                    m_output << "  sar eax, 0x1f\n"                              // negate and sub
                                "  sub eax, edx\n";                              // one if negative
                }
                return true;
            } else {
                if (num > 0x80000000) {
                    gen_expr(term, inf);
                    m_output << "  cmp" + inf.reg + "-2147483647\n" //0x80000001
                                "  setae al\n";
                    if (!inf.use_e) {
                        m_output << "  movzx " + get_next_reg(inf.opType) + " al\n";
                        disable_use_e(inf, get_next_reg(inf.opType));
                    } else {
                        m_output << "  movzx eax, al\n";
                    }
                    return true;
                }

                gen_expr(term, inf);

                // magic
                divmag::magicut magu = divmag::magicu(static_cast<unsigned int>(num));
                if (magu.p > 0) {
                    m_output << "  shr eax, " + std::to_string(magu.p) + '\n';
                } else if (magu.d) {
                    m_output << "  inc rax\n";
                }
                m_output << "  mul " + std::to_string(magu.M) + "\n"
                            "  shr rax, " + std::to_string(32 + magu.s) + '\n';
                return true;
            }
        } else {
            if (is_signed) {
                gen_expr(term, inf);

                // magic
                divmag::magicslt magl = divmag::magicl(static_cast<long>(num));

                m_output << "  mov rdx, " + std::to_string(magl.M) + "\n"
                            "  mov rcx, rax\n"
                            "  imul rdx\n";

                // correct for magic number so large it becomes negative
                if (magl.M < 0) {
                    if (num > 0) {
                        m_output << "  lea rax, [rdx+rcx*1]\n";
                        if (magl.s) {
                            m_output << "  sar rax, " + std::to_string(magl.s) + "\n";
                        }
                    } else {
                        m_output << "  add rdx, rcx\n";
                        if (magl.s) {
                            m_output << "  sar rdx, " + std::to_string(magl.s) + "\n";
                        }
                    }
                } else if (num > 0) {
                    m_output << "  mov rax, rdx\n";
                    if (magl.s) {
                        m_output << "  sar rax, " + std::to_string(magl.s) + "\n";
                    }
                } else {
                    if (magl.s) {
                        m_output << "  sar rdx, " + std::to_string(magl.s) + "\n";
                    }
                }

                if (num > 0) {
                    m_output << "  sar rcx, 0x3f\n"                              // add one if dividend
                                "  sub rax, rcx\n";                              // is negative
                } else {
                    m_output << "  mov rax, rcx\n"                               // negate and
                                "  sar rax, 0x3f\n"                              // sub one
                                "  sub rax, rdx\n";                              // if negative
                }
                return true;
            } else {
                if (num > 0x8000000000000000) {
                    gen_expr(term, inf);
                    m_output << "  mov rdx, -9223372036854775807\n" //0x8000000000000001
                                "  cmp" + inf.reg + "rdx\n"
                                "  setae al\n";
                    if (!inf.use_e) {
                        m_output << "  movzx " + get_next_reg(inf.opType) + " al\n";
                        disable_use_e(inf, get_next_reg(inf.opType));
                    } else {
                        m_output << "  movzx rax, al\n";
                    }
                    return true;
                }

                gen_expr(term, inf);

                // magic
                divmag::magicult magul = divmag::magicul(static_cast<unsigned long>(num));
                if (magul.p > 0) {
                    m_output << "  shr rax, " + std::to_string(magul.p) + '\n';
                } else if (magul.d) {
                    m_output << "  add rax, 1\n"                               // cannot be inc, doesn't set CF otherwise
                                "  sbb rax, 0\n";
                }
                m_output << "  mov rcx, " + std::to_string(magul.M) + "\n"
                            "  mul rcx\n";
                if (magul.s) {
                    m_output << "  shr rdx, " + std::to_string(magul.s) + '\n';
                }
                return true;
            }
        }
    }

    template <typename T>
    inline void gen_mul_expr(const T* mul, bool rh_first, NodeExpr* term, int64_t num, ExprInf& prefix, 
        std::string add, std::string sub, std::string mul_cmd, bool is_imul, bool is_float) {
        bool lh_paren;
        if constexpr (std::is_same_v<T, NodeExprBinMul>) {
            lh_paren = mul->lh_paren;
        } else {
            lh_paren = false;
        }
        if (!mul->by_immediate || !optimize_mul(num, lh_paren, add, sub, is_imul, is_float, rh_first, term, prefix)) {
            gen_expr(mul->lh, prefix);
            m_output << mul_cmd;
            if (is_imul || is_float) {
                m_output << prefix.reg;
            }
            if (!rh_first) {
                gen_empty_term(std::get<NodeTerm*>(mul->rh->var), prefix.opType);
            } else {
                m_output << get_last_reg(prefix.opType) + '\n';
            }
        }
    };

    template <typename T>
    inline void gen_div_expr(const T* div, ExprInf& prefix, std::string div_cmd, std::string convert_cmd, bool is_signed, bool is_long, bool is_float) {
        if constexpr (std::is_same_v<T, NodeExprBinDiv>) {
            if (!div->by_immediate || !optimize_div(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(div->rh->var)->var)->bin_num,
                                                            is_signed, is_long, is_float, div->lh, prefix)) {
                gen_expr(div->lh, prefix);
                if (div->by_immediate) { // currently unreachable
                    make_imm_if_too_big(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(div->rh->var)->var)->bin_num, prefix);
                }
                if (!is_float) {
                    m_output << convert_cmd + div_cmd + ' ';
                } else {
                    m_output << div_cmd + prefix.reg;
                }
                gen_empty_term(std::get<NodeTerm*>(div->rh->var), prefix.opType);
            }
        } else {
            gen_expr(div->lh, prefix);
            if (!is_float) {
                m_output << convert_cmd + div_cmd + ' ';
            } else {
                m_output << div_cmd + prefix.reg;
            }
            m_output << get_last_reg(prefix.opType) + '\n';
        }
    };

    template <typename T>
    void add(const T* add, bool rh_first, ExprInf& prefix) {
        gen_expr(add->lh, prefix);
        switch (prefix.opType) {
            case OpType::FLOAT:
                m_output << "  addss" + prefix.reg;
                break;
            case OpType::DOUBLE:
                m_output << "  addsd" + prefix.reg;
                break;
            default:
                m_output << "  add" + prefix.reg;
        }
        if (!rh_first) {
            gen_empty_term(std::get<NodeTerm*>(add->rh->var), prefix.opType);
        } else {
            m_output << get_last_reg(prefix.opType) + '\n';
        }
    }

    template <typename T>
    void sub(const T* sub, bool rh_first, ExprInf& prefix) {
        gen_expr(sub->lh, prefix);
        switch (prefix.opType) {
            case OpType::FLOAT:
                m_output << "  subss" + prefix.reg;
                break;
            case OpType::DOUBLE:
                    m_output << "  subsd" + prefix.reg;
                break;
            default:
                if constexpr (std::is_same_v<T, NodeExprBinSub>) {
                    if (sub->by_immediate) {
                        make_imm_if_too_big(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(sub->rh->var)->var)->bin_num, prefix);
                    }
                }
                m_output << "  sub" + prefix.reg;
        }
        if (!rh_first) {
            gen_empty_term(std::get<NodeTerm*>(sub->rh->var), prefix.opType);
        } else {
            m_output << get_last_reg(prefix.opType) + '\n';
        }
    }

    template <typename T>
    void mul(const T* mul, bool rh_first, ExprInf& prefix) {
        int64_t num;
        NodeExpr* term;
        if (mul->by_immediate == 1) {
            num = std::get<NodeTermIntLit*>(std::get<NodeTerm*>(mul->lh->var)->var)->bin_num;
            term = mul->rh;
        } else if (mul->by_immediate == 2) {
            num = std::get<NodeTermIntLit*>(std::get<NodeTerm*>(mul->rh->var)->var)->bin_num;
            term = mul->lh;
        }

        switch (prefix.opType) {
            case OpType::FLOAT:
                gen_mul_expr<T>(mul, rh_first, term, num, prefix, "  addss", "  subss", "  mulss", false, true);
                break;
            case OpType::DOUBLE:
                gen_mul_expr<T>(mul, rh_first, term, num, prefix, "  addsd", "  subsd", "  mulsd", false, true);
                break;
            case OpType::SINT:
                gen_mul_expr<T>(mul, rh_first, term, num, prefix, "  add", "  sub", "  imul", true, false);
                break;
            case OpType::SLONG:
                gen_mul_expr<T>(mul, rh_first, term, num, prefix, "  add", "  sub", "  imul", true, false);
                break;
            case OpType::INT:
            case OpType::LONG:
                gen_mul_expr<T>(mul, rh_first, term, num, prefix, "  add", "  sub", "  mul ", false, false);
                break;
        }
    }

    template <typename T>
    void div(const T* div, ExprInf& prefix) {
        switch (prefix.opType) {
            case OpType::FLOAT:
                gen_div_expr<T>(div,  prefix, "  divss", "", true, false, true);
                break;
            case OpType::DOUBLE:
                gen_div_expr<T>(div, prefix, "  divsd", "", true, true, true);
                break;
            case OpType::SINT:
                gen_div_expr<T>(div, prefix, "  idiv", "  cdq\n", true, false, false);
                break;
            case OpType::SLONG:
                gen_div_expr<T>(div, prefix, "  idiv", "  cqo\n", true, true, false);
                break;
            case OpType::INT:
                gen_div_expr<T>(div, prefix, "  div", "  cdq\n", false, false, false);
                break;
            case OpType::LONG:
                gen_div_expr<T>(div, prefix, "  div", "  cqo\n", false, true, false);
                break;
        }
    }

    template <typename T>
    void shift_or_roll(const T* s, ExprInf& prefix, std::string instruc) {
        gen_expr(s->lh, prefix);
        
        std::string rh_expr;
        if constexpr (std::is_same_v<T, NodeExprBinRShiftByPar> || std::is_same_v<T, NodeExprBinLShiftByPar> ||
                        std::is_same_v<T, NodeExprBinRRollByPar> || std::is_same_v<T, NodeExprBinLRollByPar>) {
            rh_expr = get_last_reg(prefix.opType);
        } else {
            if (s->by_immediate) {
                m_output << instruc + prefix.reg + 
                    std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(s->rh->var)->var)->bin_num % 256) + '\n';
                return;
            }
            // guarenteed to be a variable at this point
            rh_expr = get_address(std::get<NodeTermIdent*>(std::get<NodeTerm*>(s->rh->var)->var)->ident, prefix);
        }
        if (m_used_regs != RCX_POS) {
            if (prefix.opType == OpType::SINT || prefix.opType == OpType::INT) {
                m_output << "  mov ecx, " + rh_expr + '\n';
            } else {
                m_output << "  mov rcx, " + rh_expr + '\n';
            }
        }
        m_output << instruc + prefix.reg + "cl\n";
    }

    template <typename T>
    std::tuple<char, std::string, std::string> paren_start(const T* expr, ExprInf& inf) {
        inf.use_e = -1; //TODO: when more memory is used as registers it should only do the first if statement 1 time later
        if ((m_used_regs >= MAX_REG_NUM_MINUS_ONE || (m_used_regs == MAX_REG_NUM_MINUS_ONE - 1 && m_o_rax)) && !inf.two_part) {
            if (!m_o_rax) {
                std::string first_reg = get_before_comma(inf.reg);
                inf.reg = get_a_reg(inf.opType);
                m_used_regs--;
                m_o_rax++;
                return std::tuple<char, std::string, std::string>{2, first_reg, get_before_comma(inf.reg)};
            }
            inf.reg = get_a_reg(inf.opType);
            std::string a_reg = get_before_comma(inf.reg);
            return std::tuple<char, std::string, std::string>{2, a_reg, a_reg};
        }
        if (expr->last_areg_op > 0) {
            std::string first_reg = get_before_comma(inf.reg);
            inf.use_e = expr->last_areg_op;
            inf.reg = get_a_reg(inf.opType);
            std::string a_reg = get_before_comma(inf.reg);
            return std::tuple<char, std::string, std::string>{0, first_reg, a_reg};
        } else {
            std::string first_reg = get_before_comma(inf.reg);
            inf.reg = ' ' + get_next_reg_inc(inf.opType) + ", ";
            return std::tuple<char, std::string, std::string>{1, first_reg, first_reg};
        }
    }

    template <typename T>
    void gen_paren_start(const T* expr, ExprInf& inf) {
        std::tuple<char, std::string, std::string> put_reg = paren_start<T>(expr, inf);
        gen_expr(expr->rh, inf);
        if (std::get<0>(put_reg) == 2 && !(--m_o_rax)) {
            m_used_regs++;
        }
        if ((std::get<0>(put_reg) == 0 && inf.use_e >= 0) || std::get<0>(put_reg) == 2) {
            std::string mov = (inf.opType == OpType::FLOAT || inf.opType == OpType::DOUBLE) ? "  movss" : "  mov";
            m_output << mov + ' ' + get_next_reg_inc(inf.opType) + ',' + std::get<2>(put_reg) + '\n';
        }
        inf.reg = std::get<1>(put_reg) + ", ";
        inf.use_e = -1;
    }

    inline void cast(Variable cast_type, bool round_float, ExprInf& inf) {
        switch (var_switch(cast_type.type, inf.varType)) {
            case var_switch(VarType::FLOAT, VarType::INT_32):
            case var_switch(VarType::FLOAT, VarType::INT_16):
            case var_switch(VarType::FLOAT, VarType::BYTE):
            {
                std::string dword_reg = inf.use_e ? " eax, " : ' ' + DWORDS[m_used_regs + 1] + ", ";
                if (!round_float) {
                    m_output << "  cvttss2si" + dword_reg + "xmm0\n";
                } else {
                    m_output << "  cvtss2si" + dword_reg + "xmm0\n";
                }
                break;
            }
            case var_switch(VarType::FLOAT, VarType::INT_64):
            {
                std::string qword_reg = inf.use_e ? " rax, " : ' ' + QWORDS[m_used_regs + 1] + ", ";
                if (!round_float) {
                    m_output << "  cvttss2si" + qword_reg + "xmm0\n";
                } else {
                    m_output << "  cvtss2si" + qword_reg + "xmm0\n";
                }
                break;
            }
            case var_switch(VarType::DOUBLE, VarType::INT_32):
            case var_switch(VarType::DOUBLE, VarType::INT_16):
            case var_switch(VarType::DOUBLE, VarType::BYTE):
            {
                std::string dword_reg = inf.use_e ? " eax, " : ' ' + DWORDS[m_used_regs + 1] + ", ";
                if (!round_float) {
                    m_output << "  cvttsd2si" + dword_reg + "xmm0\n";
                } else {
                    m_output << "  cvtsd2si" + dword_reg + "xmm0\n";
                }
                break;
            }
            case var_switch(VarType::DOUBLE, VarType::INT_64): 
            {
                std::string qword_reg = inf.use_e ? " rax, " : ' ' + QWORDS[m_used_regs + 1] + ", ";
                if (!round_float) {
                    m_output << "  cvttsd2si" + qword_reg + "xmm0\n";
                } else {
                    m_output << "  cvtsd2si" + qword_reg + "xmm0\n";
                }
                break;
            }
            case var_switch(VarType::INT_32, VarType::FLOAT):
            case var_switch(VarType::INT_16, VarType::FLOAT):
            case var_switch(VarType::BYTE, VarType::FLOAT):
            {
                std::string xmm_reg = inf.use_e ? "xmm0" : XMMS[m_used_regs + 1];
                m_output << "  xorps " + xmm_reg + ", " + xmm_reg + "\n  cvtsi2ss " + xmm_reg + ", eax\n";
                break;
            }
            case var_switch(VarType::INT_64, VarType::FLOAT):
            {
                std::string xmm_reg = inf.use_e ? "xmm0" : XMMS[m_used_regs + 1];
                m_output << "  xorps " + xmm_reg + ", " + xmm_reg + "\n  cvtsi2ss " + xmm_reg + ", rax\n";
                break;
            }
            case var_switch(VarType::INT_32, VarType::DOUBLE):
            case var_switch(VarType::INT_16, VarType::DOUBLE):
            case var_switch(VarType::BYTE, VarType::DOUBLE):
            {
                std::string xmm_reg = inf.use_e ? "xmm0" : XMMS[m_used_regs + 1];
                m_output << "  xorps " + xmm_reg + ", " + xmm_reg + "\n  cvtsi2sd " + xmm_reg + ", eax\n";
                break;
            }
            case var_switch(VarType::INT_64, VarType::DOUBLE):
            {
                std::string xmm_reg = inf.use_e ? "xmm0" : XMMS[m_used_regs + 1];
                m_output << "  xorps " + xmm_reg + ", " + xmm_reg + "\n  cvtsi2sd " + xmm_reg + ", rax\n";
                break;
            }
            case var_switch(VarType::INT_32, VarType::INT_64):
            case var_switch(VarType::INT_16, VarType::INT_64):
            case var_switch(VarType::BYTE, VarType::INT_64):
            {
                if ((inf.opType == OpType::SINT || inf.opType == OpType::SLONG)) {
                    if (inf.use_e) {
                        m_output << "  cdqe\n";
                    } else {
                        m_output << "  movsx " + QWORDS[m_used_regs + 1] + ", eax\n";
                    }
                } else {
                    if (inf.use_e) {
                        m_output << "  mov rax, eax\n";
                    } else {
                        m_output << "  movzx " + QWORDS[m_used_regs + 1] + ", eax\n";
                    }
                }
                break;
            }
            case var_switch(VarType::INT_32, VarType::BOOL):
            case var_switch(VarType::INT_16, VarType::BOOL):
            case var_switch(VarType::BYTE, VarType::BOOL):
                //TODO
            case var_switch(VarType::INT_64, VarType::BOOL):
                //TODO
            case var_switch(VarType::FLOAT, VarType::BOOL):
            case var_switch(VarType::DOUBLE, VarType::BOOL):
                ;//TODO
        }
    }

    struct Gen_stmt {
        Generator& main;

        void gen(const NodeStmtVarAssign* var_assign, bool last) { 
            if (var_assign->properties & term_assign) {
                if (var_assign->properties & 2) {
                    std::string const_term;
                    std::string const_reference;
                    std::string ident(var_assign->ident);
                    if (var_assign->properties & too_big_imm_assign || (var_assign->properties & double_instruc) == double_instruc) {
                        const_term = "  dq  ";
                        const_reference = "QWORD [" + ident + "]";
                    } else if (var_assign->properties & float_instruc) {
                        const_term = "  dd  ";
                        const_reference = "DWORD [" + ident + "]";
                    } else {
                        const_term = "  equ  ";
                        const_reference = ident;
                    }
                    main.m_section_rodata << "  " + ident + const_term + 
                        std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_assign->expr->var)->var)->bin_num) + '\n';
                    main.m_variables.insert({var_assign->ident, const_reference});
                } else {
                    ExprInf inf = main.get_expr_inf(var_assign->type, true); //doesnt matter if its signed or not
                    if (var_assign->properties & too_big_imm_assign) {
                        main.m_output << "  mov rdx, " + 
                            std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_assign->expr->var)->var)->bin_num) + '\n';
                        main.m_output << "  mov " + main.make_var(var_assign->ident, var_assign->type) + ", rdx\n";
                        return;
                    } else if ((var_assign->properties & double_instruc) == double_instruc) {
                        main.m_output << "  mov rax, " + 
                            std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_assign->expr->var)->var)->bin_num) + '\n';
                        main.m_output << "  mov " + main.make_var(var_assign->ident, var_assign->type) + ", rax\n";
                        return;
                    } else if (var_assign->properties & float_instruc) {
                        main.m_output << "  mov " + main.make_var(var_assign->ident, var_assign->type) + ", " + 
                            std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_assign->expr->var)->var)->bin_num) + '\n';
                        return;
                    }
                    main.m_output << "  mov " + main.make_var(var_assign->ident, var_assign->type);
                    main.gen_term(std::get<NodeTerm*>(var_assign->expr->var), comma);
                }
            } else {
                ExprInf inf = main.get_expr_inf(var_assign->type, (var_assign->properties & double_instruc));
                main.gen_expr(var_assign->expr, inf);
                std::string mov = (inf.opType == OpType::FLOAT || inf.opType == OpType::DOUBLE) ? "  movss" : "  mov";
                main.m_output << mov + ' ' + main.make_var(var_assign->ident, var_assign->type) + ", " + main.get_a_reg_exact_size(var_assign->type) + '\n';
            }
        }
        void gen(const NodeStmtExit* exit, bool last){ //TODO: make it just move into rdi first
            ExprInf expr_inf = ExprInf{.mov = "  mov", .reg = rax, .opType = OpType::SLONG};
            main.gen_expr(exit->val, expr_inf);
            main.m_output << "  mov rdi, rax\n"
                             "  mov rax, 60\n"
                             "  syscall\n";
        }
        void gen(const NodeStmtVarSet* var_set, bool last){
            if (var_set->properties & term_assign) {
                ExprInf inf = main.get_expr_inf(var_set->type, true); //doesnt matter if its signed or not
                if (var_set->properties & too_big_imm_assign) {
                    main.m_output << "  mov rdx, " + 
                        std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_set->expr->var)->var)->bin_num) + '\n';
                    main.m_output << "  mov " + main.m_variables.at(var_set->ident) + ", rdx\n";
                    return;
                } else if ((var_set->properties & double_instruc) == double_instruc) {
                    main.m_output << "  mov rax, " + 
                        std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_set->expr->var)->var)->bin_num) + '\n';
                    main.m_output << "  mov " + main.m_variables.at(var_set->ident) + ", rax\n";
                    return;
                } else if (var_set->properties & float_instruc) {
                    main.m_output << "  mov " + main.m_variables.at(var_set->ident) + ", " + 
                        std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(var_set->expr->var)->var)->bin_num) + '\n';
                    return;
                }
                main.m_output << inf.mov + ' ' + main.m_variables.at(var_set->ident);
                main.gen_term(std::get<NodeTerm*>(var_set->expr->var), comma);
            } else {
                ExprInf inf = main.get_expr_inf(var_set->type, (var_set->properties & double_instruc));
                main.gen_expr(var_set->expr, inf);
                std::string mov = (inf.opType == OpType::FLOAT || inf.opType == OpType::DOUBLE) ? "  movss" : "  mov";
                main.m_output << mov + ' ' + main.m_variables.at(var_set->ident) + ", " + main.get_a_reg_exact_size(var_set->type) + '\n';
            }
        }
        void gen(const NodeScope* scope, bool last) {
            if (scope->num_bytes != 0) {
                main.m_output << "  push rbp\n"
                                 "  mov rbp, rsp\n";

                if (scope->used_non_volatile_regs > 0) {
                    for (int i = 0; i < scope->used_non_volatile_regs; i++) {
                        main.push(NV_QWORDS[i]);
                    }
                }

                main.m_stack_loc = scope->num_bytes;

                // If the scope does not need to allocate more than the red zone there is no need to sub from rsp
                if (scope->num_bytes < RED_ZONE_BYTES) {
                    for (const NodeStmt* stmt : scope->stmts) {
                        main.gen_stmt(stmt);
                    }

                    if (scope->used_non_volatile_regs > 0) {
                        for (int i = 0; i < scope->used_non_volatile_regs; i++) {
                            main.pop(NV_QWORDS[i]);
                        }
                    }
                } else {
                    std::string bytes = std::to_string((scope->num_bytes - RED_ZONE_BYTES));
                    main.m_output << "  sub rsp, " + bytes + '\n';
                    for (const NodeStmt* stmt : scope->stmts) {
                        main.gen_stmt(stmt);
                    }

                    if (scope->used_non_volatile_regs > 0) {
                        main.m_output << "  add rsp, " + bytes + '\n';
                        for (int i = 0; i < scope->used_non_volatile_regs; i++) {
                            main.pop(NV_QWORDS[i]);
                        }
                    }
                    main.m_output << "  mov rsp, rbp\n";
                }

                main.m_output << "  pop rbp\n";
            } else {
                for (const NodeStmt* stmt : scope->stmts) {
                    main.gen_stmt(stmt);
                }
            }   
        }
        void gen(const NodeStmtGroup* stmtgroup, bool last) {
            size_t last_index = stmtgroup->stmts.size() - 1;
            for (size_t i = 0; i < last_index; i++) {
                main.gen_stmt(stmtgroup->stmts.at(i));
            }
            main.gen_stmt(stmtgroup->stmts[last_index], last);
        }
        void gen(const NodeStmtIf* _if, bool last) {
            ExprInf inf = main.get_expr_inf(_if->if_expr_type.type, _if->if_expr_type.is_signed);
            size_t label = main.gen_if_statement_start(_if->if_expr, inf, (last && !_if->else_stmts.has_value()));
            if (_if->else_stmts.has_value()) {
                size_t label2 = main.m_label_cnt - 1;
                size_t past_else_label;
                if (last) {
                    past_else_label = label2 - main.m_label_backtrack + 3;
                } else {
                    past_else_label = label2;
                }
                main.gen_stmt(_if->stmts, true);
                main.m_output << "  jmp .L" + std::to_string(past_else_label) + '\n';
                main.add_label(label);
                main.gen_stmt(_if->else_stmts.value());
                main.add_label(label2);
                if (last) {
                    main.m_label_backtrack = 0;
                }
            } else {
                main.gen_stmt(_if->stmts, true);
                main.add_label(label);
            }
        }
        void gen(const NodeStmtIfSet* _if, bool last) {
            ;
        }
        void gen(const NodeStmtWhile* _while, bool last) {
            if (_while->if_expr.has_value()) {
                ExprInf inf = main.get_expr_inf(_while->if_expr_type.type, _while->if_expr_type.is_signed);
                size_t label;
                std::string end_label_str;
                if (!_while->do_while) {
                    label = main.gen_if_statement_start(_while->if_expr.value(), inf, last);
                    end_label_str = ".L" + std::to_string(label);
                } else {
                    label = main.m_extra_label_cnt++;
                    end_label_str = ".B" + std::to_string(label);
                }
                main.gen_while_statement(_while->if_expr.value(), _while->stmts, std::nullopt, end_label_str, inf, last);
                main.m_output << end_label_str + ":\n";
            } else {
                std::string label = ".LINF" + std::to_string(main.m_extra_label_cnt);
                std::string end_label = ".B" + std::to_string(main.m_extra_label_cnt + 1);
                main.m_loop_labels.push_back(std::pair<std::string, std::string>{label, end_label});
                main.m_extra_label_cnt += 2;
                main.m_output << label + ":\n";
                main.gen_stmt(_while->stmts);
                main.m_output << "  jmp " + label + '\n' + end_label + ":\n";
            }
        }
        void gen(const NodeStmtFor* _for, bool last) {
            if (_for->if_expr.has_value()) {
                ExprInf inf = main.get_expr_inf(_for->if_expr_type.type, _for->if_expr_type.is_signed);
                main.gen_stmt(_for->init_stmt);
                //TODO: this is just for the case when someone does for(int x = 0, x != 0) or for(; x < 0; x++) and should be optimized out sometimes
                size_t label = main.gen_if_statement_start(_for->if_expr.value(), inf, last);
                std::string end_label_str = ".L" + std::to_string(label);
                main.gen_while_statement(_for->if_expr.value(), _for->stmts, _for->end_stmt, end_label_str, inf, last);
                main.m_output << end_label_str + ":\n";
            } else {
                std::string label = ".LINF" + std::to_string(main.m_extra_label_cnt);
                std::string end_label = ".B" + std::to_string(main.m_extra_label_cnt + 1);
                main.m_loop_labels.push_back(std::pair<std::string, std::string>{label, end_label});
                main.m_extra_label_cnt += 2;
                main.gen_stmt(_for->init_stmt);
                main.m_output << label + ":\n";
                main.gen_stmt(_for->stmts);
                main.gen_stmt(_for->end_stmt);
                main.m_output << "  jmp " + label + '\n' + end_label + ":\n";
            }
        }
        void gen(const NodeStmtBreak* _break, bool last) {
            main.m_output << "  jmp " + main.m_loop_labels.at(main.m_loop_labels.size() - _break->loop - 1).second + '\n';
        }
        void gen(const NodeStmtContinue* _continue, bool last) {
            main.m_output << "  jmp " + main.m_loop_labels.at(main.m_loop_labels.size() - _continue->loop - 1).first + '\n';
        }
        void gen(const NodeStmtAsm* _asm, bool last) {

        }
        void gen(const NodeStmtBlank* blank, bool last) {
            // do nothing
        }
    };

    struct Gen_expr {
        Generator& main;

        void gen(const NodeExprBin* exprbin, ExprInf& prefix){
            main.gen_bin_expr(exprbin, prefix);
        }
        void gen(const NodeExprLog* exprlog, ExprInf& prefix) {
            main.gen_log_expr(exprlog, prefix);
        }
        void gen(const NodeTerm* term, ExprInf& prefix) {
            main.gen_term(term, prefix);
        }
        void gen(const NodeExprCast* exprcast, ExprInf& prefix) {
            ExprInf prev_prefix = prefix;
            prefix = main.get_expr_inf(exprcast->from_type.type, exprcast->from_type.is_signed);
            main.gen_term(exprcast->expr, prefix);
            prefix = prev_prefix;
            main.cast(exprcast->from_type, exprcast->round_float, prefix);
        }
        void gen(const NodeExprTwoPart* two_part, ExprInf& prefix) {
            prefix.two_part = true;
            main.gen_expr(two_part->first, prefix);
            main.m_output << "  mov " << main.get_rbp_term(prefix.opType) << "," + main.get_before_comma(prefix.reg) + '\n';
            main.gen_expr(two_part->second, prefix);
        }
    };

    struct Gen_bin_expr {
        Generator& main;

        void gen(const NodeExprBinAdd* add, ExprInf& prefix){
            main.add<NodeExprBinAdd>(add, false, prefix);
        }
        void gen(const NodeExprBinSub* sub, ExprInf& prefix){
            main.sub<NodeExprBinSub>(sub, false, prefix);
        }
        void gen(const NodeExprBinMul* mul, ExprInf& prefix){
            main.mul<NodeExprBinMul>(mul, false, prefix);
        }
        void gen(const NodeExprBinDiv* div, ExprInf& prefix){
            main.div<NodeExprBinDiv>(div, prefix);
        }
        void gen(const NodeExprBinMod* mod, ExprInf& prefix){
            ;
        }
        void gen(const NodeExprBinAddByPar* add, ExprInf prefix){ // it is important that ExprInf is not passed by reference
            main.gen_paren_start<NodeExprBinAddByPar>(add, prefix);
            main.add<NodeExprBinAddByPar>(add, true, prefix);
            main.m_used_regs--;
        }
        void gen(const NodeExprBinSubByPar* sub, ExprInf prefix){ 
            main.gen_paren_start<NodeExprBinSubByPar>(sub, prefix);
            main.sub<NodeExprBinSubByPar>(sub, true, prefix);
            main.m_used_regs--;
        }
        void gen(const NodeExprBinMulByPar* mul, ExprInf prefix){
            main.gen_paren_start<NodeExprBinMulByPar>(mul, prefix);
            main.mul<NodeExprBinMulByPar>(mul, true, prefix);
            main.m_used_regs--;
        }
        void gen(const NodeExprBinDivByPar* div, ExprInf prefix){
            main.gen_paren_start<NodeExprBinDivByPar>(div, prefix);
            main.div<NodeExprBinDivByPar>(div, prefix);
            main.m_used_regs--;
        }
        void gen(const NodeExprBinModByPar* mod, ExprInf prefix){
            ;
        }
        // bitwise operators, no need for gen_term_empty, already constricted to ints
        void gen(const NodeExprBinAnd* _and, ExprInf& prefix){
            main.gen_expr(_and->lh, prefix);
            main.m_output << "  and" + prefix.reg;
            main.gen_term(std::get<NodeTerm*>(_and->rh->var), empty);
        }
        void gen(const NodeExprBinOr* _or, ExprInf& prefix){
            main.gen_expr(_or->lh, prefix);
            main.m_output << "  or" + prefix.reg;
            main.gen_term(std::get<NodeTerm*>(_or->rh->var), empty);
        }
        void gen(const NodeExprBinXor* _xor, ExprInf& prefix){
            main.gen_expr(_xor->lh, prefix);
            main.m_output << "  xor" + prefix.reg;
            main.gen_term(std::get<NodeTerm*>(_xor->rh->var), empty);
        }
        void gen(const NodeExprBinAndByPar* _and, ExprInf prefix){
            main.gen_paren_start<NodeExprBinAndByPar>(_and, prefix);
            main.m_output << "  and" + prefix.reg + main.get_last_reg(prefix.opType) + '\n';
            main.m_used_regs--;
        }
        void gen(const NodeExprBinOrByPar* _or, ExprInf prefix){
            main.gen_paren_start<NodeExprBinOrByPar>(_or, prefix);
            main.m_output << "  or" + prefix.reg + main.get_last_reg(prefix.opType) + '\n';
            main.m_used_regs--;
        }
        void gen(const NodeExprBinXorByPar* _xor, ExprInf prefix){
            main.gen_paren_start<NodeExprBinXorByPar>(_xor, prefix);
            main.m_output << "  xor" + prefix.reg + main.get_last_reg(prefix.opType) + '\n';
            main.m_used_regs--;
        }
        void gen(const NodeExprBinRShift* rshift, ExprInf& prefix){
            main.shift_or_roll<NodeExprBinRShift>(rshift, prefix, "  sar");
        }
        void gen(const NodeExprBinLShift* lshift, ExprInf& prefix){
            main.shift_or_roll<NodeExprBinLShift>(lshift, prefix, "  shl");
        }
        void gen(const NodeExprBinRRoll* rroll, ExprInf& prefix){
            main.shift_or_roll<NodeExprBinRRoll>(rroll, prefix, "  ror");
        }
        void gen(const NodeExprBinLRoll* lroll, ExprInf& prefix){
            main.shift_or_roll<NodeExprBinLRoll>(lroll, prefix, "  rol");
        }
        void gen(const NodeExprBinRShiftByPar* rshift, ExprInf prefix){
            main.gen_paren_start<NodeExprBinRShiftByPar>(rshift, prefix);
            main.shift_or_roll<NodeExprBinRShiftByPar>(rshift, prefix, "  sar");
            main.m_used_regs--;
        }
        void gen(const NodeExprBinLShiftByPar* lshift, ExprInf prefix){
            main.gen_paren_start<NodeExprBinLShiftByPar>(lshift, prefix);
            main.shift_or_roll<NodeExprBinLShiftByPar>(lshift, prefix, "  shl");
            main.m_used_regs--;
        }
        void gen(const NodeExprBinRRollByPar* rroll, ExprInf prefix){
            main.gen_paren_start<NodeExprBinRRollByPar>(rroll, prefix);
            main.shift_or_roll<NodeExprBinRRollByPar>(rroll, prefix, "  ror");
            main.m_used_regs--;
        }
        void gen(const NodeExprBinLRollByPar* lroll, ExprInf prefix){
            main.gen_paren_start<NodeExprBinLRollByPar>(lroll, prefix);
            main.shift_or_roll<NodeExprBinLRollByPar>(lroll, prefix, "  rol");
            main.m_used_regs--;
        }
    };

    struct Gen_log_expr {
        Generator& main;

        void gen(const NodeExprLogTest* test, ExprInf& prefix) { //TODO: make part of this a function
            std::string jump_stmt = prefix.if_inf.jump ? "  jnz" : "  jz";
            std::string label_ident;
            size_t label;
            if (prefix.if_inf.label >= 2) {
                label = main.m_extra_label_cnt + prefix.if_inf.label - 2;
                label_ident = " .EL";
                main.m_use_extra_label++;
            } else if (prefix.if_inf.label == 1) {
                label = main.m_label_cnt + 1;
                label_ident = " .L";
                main.m_use_truth_label = true;
            } else {
                label = main.m_label_cnt_2;
                label_ident = " .L";
            }

            if (prefix.opType == OpType::FLOAT || prefix.opType == OpType::DOUBLE) {
                std::string next_reg = main.get_next_reg(prefix.opType);
                std::string cmp_mem_or_reg;
                if (test->ident) {
                    cmp_mem_or_reg = ' ' + main.get_address(std::get<NodeTermIdent*>(std::get<NodeTerm*>(test->expr->var)->var)->ident, prefix);
                } else {
                    main.gen_expr(test->expr, prefix);
                    cmp_mem_or_reg = main.get_before_comma(prefix.reg);
                }

                size_t parity_jump;
                std::string parity_jump_stmt = prefix.if_inf.jump ? "  jnp" : "  jp";
                std::string parity_label_ident;
                if ((main.m_encapsulated_if_and + main.m_encapsulated_if_or) > 1) {
                    parity_jump = main.m_extra_label_cnt;
                    parity_label_ident = " .EL";
                    if (prefix.if_inf.label < 2) {
                        main.m_use_extra_label++;
                    }
                } else {
                    parity_jump = main.m_label_cnt_2;
                    parity_label_ident = " .L";
                }

                // jump if parity tests for NaN state which is set to false (PF isn't fully useless!)
                main.m_output << "  xorps " + next_reg + ", " + next_reg + '\n' + main.get_cmp_op(prefix.opType, false) + ' ' + next_reg + ',' + cmp_mem_or_reg + \
                    '\n' + parity_jump_stmt + parity_label_ident + std::to_string(parity_jump) + "\n  xorps " + next_reg + ", " + next_reg + '\n' + main.get_cmp_op(prefix.opType, false) + \
                    ' ' + next_reg + ',' + cmp_mem_or_reg + '\n';
            } else if (test->ident) {
                main.m_output << main.get_cmp_op(prefix.opType, false) + ' ' + 
                    main.get_address(std::get<NodeTermIdent*>(std::get<NodeTerm*>(test->expr->var)->var)->ident, prefix) + ", 0\n";
            } else {
                main.gen_expr(test->expr, prefix);
                std::string a_reg = main.get_a_reg_exact_size(prefix.varType);
                    main.m_output << "  test " + a_reg + ", " + a_reg + '\n';
            }
            main.m_output << jump_stmt + label_ident + std::to_string(label) + '\n';
        }
        void gen(const NodeExprLogCompareEqu* equ, ExprInf& prefix) {
            main.gen_if_compare_expr<NodeExprLogCompareEqu>(equ, false, prefix, "ne", "e");
        }
        void gen(const NodeExprLogCompareNotEqu* not_equ, ExprInf& prefix) {
            main.gen_if_compare_expr<NodeExprLogCompareNotEqu>(not_equ, false, prefix, "e", "ne");
        }
        void gen(const NodeExprLogCompareGreater* greater, ExprInf& prefix) {
            std::pair<std::string, std::string> ops = main.get_jump_op(prefix.opType, "le", "g", "be", "a", "nbe", "be");
            main.gen_if_compare_expr<NodeExprLogCompareGreater>(greater, false, prefix, ops.first, ops.second);
        }
        void gen(const NodeExprLogCompareGreaterEqu* greater_equ, ExprInf& prefix) {
            std::pair<std::string, std::string> ops = main.get_jump_op(prefix.opType, "l", "ge", "b", "ae", "nb", "b");
            main.gen_if_compare_expr<NodeExprLogCompareGreaterEqu>(greater_equ, false, prefix, ops.first, ops.second);
        }
        void gen(const NodeExprLogCompareLess* less, ExprInf& prefix) {
            std::pair<std::string, std::string> ops = main.get_jump_op(prefix.opType, "ge", "l", "ae", "b", "nbe", "be");
            main.gen_if_compare_expr<NodeExprLogCompareLess>(less, false, prefix, ops.first, ops.second);
        }
        void gen(const NodeExprLogCompareLessEqu* less_equ, ExprInf& prefix) {
            std::pair<std::string, std::string> ops = main.get_jump_op(prefix.opType, "g", "le", "a", "be", "nb", "b");
            main.gen_if_compare_expr<NodeExprLogCompareLessEqu>(less_equ, false, prefix, ops.first, ops.second);
        }
        void gen(const NodeExprLogCompareEquByPar* equ, ExprInf& prefix) {
            if (!equ->comp_as_int) {
                main.gen_paren_start<NodeExprLogCompareEquByPar>(equ, prefix);
                main.gen_if_compare_expr<NodeExprLogCompareEquByPar>(equ, true, prefix, "ne", "e");
                main.m_used_regs--;
            } else {
                main.gen_if_compare_expr<NodeExprLogCompareEquByPar>(equ, true, prefix, "ne", "e");
            }
        }
        void gen(const NodeExprLogCompareNotEquByPar* not_equ, ExprInf& prefix) {
            if (!not_equ->comp_as_int) {
                main.gen_paren_start<NodeExprLogCompareNotEquByPar>(not_equ, prefix);
                main.gen_if_compare_expr<NodeExprLogCompareNotEquByPar>(not_equ, true, prefix, "e", "ne");
                main.m_used_regs--;
            } else {
                main.gen_if_compare_expr<NodeExprLogCompareNotEquByPar>(not_equ, true, prefix, "e", "ne");
            }
        }
        void gen(const NodeExprLogCompareGreaterByPar* greater, ExprInf& prefix) {
            main.gen_if_compare_expr_by_par<NodeExprLogCompareGreaterByPar>(greater, prefix, "le", "g", "be", "a", "nbe", "be");
        }
        void gen(const NodeExprLogCompareGreaterEquByPar* greater_equ, ExprInf& prefix) {
            main.gen_if_compare_expr_by_par<NodeExprLogCompareGreaterEquByPar>(greater_equ, prefix, "l", "ge", "b", "ae", "nb", "b");
        }
        void gen(const NodeExprLogCompareLessByPar* less, ExprInf& prefix) {
            main.gen_if_compare_expr_by_par<NodeExprLogCompareLessByPar>(less, prefix, "ge", "l", "ae", "b", "nbe", "be");
        }
        void gen(const NodeExprLogCompareLessEquByPar* less_equ, ExprInf& prefix) {
            main.gen_if_compare_expr_by_par<NodeExprLogCompareLessEquByPar>(less_equ, prefix, "g", "le", "a", "be", "nb", "b");
        }
        void gen(const NodeExprLogOpAnd* _and, ExprInf& prefix) {
            // I'm actually really proud of the and and or code
            bool orig_jump = prefix.if_inf.jump;
            char orig_label = prefix.if_inf.label;
            prefix.if_inf.jump = false;
            prefix.if_inf.label = (main.m_encapsulated_if_and + main.m_encapsulated_if_or) ? orig_jump ? orig_label > 1 ? 
                                orig_label + 1
                                : 2 
                                : orig_label
                                : (orig_jump | orig_label);
            prefix.if_inf.label += (!prefix.if_inf.label) * orig_label * (orig_label >= 2);
            main.m_encapsulated_if_and++;
            main.gen_expr(_and->lh, prefix);
            
            prefix.if_inf.jump = orig_jump;
            prefix.if_inf.label = orig_label;
            main.m_encapsulated_if_and--;
            main.gen_expr(_and->rh, prefix);
            if (main.m_use_extra_label) {
                main.add_extra_label();
                main.m_use_extra_label--;
            }
        }
        void gen(const NodeExprLogOpOr* _or, ExprInf& prefix) {
            bool orig_jump = prefix.if_inf.jump;
            char orig_label = prefix.if_inf.label;
            prefix.if_inf.jump = true;
            prefix.if_inf.label = (orig_jump ^ orig_label) & orig_label ? orig_label > 1 ?
                                orig_label + 1
                                : 2
                                : !(orig_jump ^ orig_label);
            prefix.if_inf.label += (!prefix.if_inf.label) * orig_label * (orig_label >= 2) + \
                                    (prefix.if_inf.label == 1) * (main.m_encapsulated_if_and);
            main.m_encapsulated_if_or++;
            main.gen_expr(_or->lh, prefix);
            
            prefix.if_inf.jump = orig_jump;
            prefix.if_inf.label = orig_label;
            main.m_encapsulated_if_or--;
            if (main.m_use_mid_or_label) {
                main.add_extra_label();
                main.m_use_mid_or_label--;
            }
            main.gen_expr(_or->rh, prefix);
            if (main.m_use_extra_label) {
                main.add_extra_label();
                main.m_use_extra_label--;
            }
        }
    };
    
    struct Gen_term {
        Generator& main;

        void gen(const NodeTermIntLit* int_lit, const ExprInf& prefix){
            if (prefix.opType == OpType::FLOAT) {
                std::string float_mem = main.add_float_num(int_lit->bin_num, "  dd  ");
                main.m_output << prefix.mov + prefix.reg + "DWORD [" + float_mem + "]\n";
            } else if (prefix.opType == OpType::DOUBLE) {
                std::string double_mem = main.add_float_num(int_lit->bin_num, "  dq  ");
                main.m_output << prefix.mov + prefix.reg + "QWORD [" + double_mem + "]\n";
            } else if (fits_in_immediate(int_lit->bin_num, prefix.varType)) {
                main.m_output << prefix.mov + prefix.reg + std::to_string(static_cast<signed int>(int_lit->bin_num)) + '\n';
            } else if (prefix.uses_reg) {
                main.m_output << prefix.mov + prefix.reg + std::to_string(int_lit->bin_num) + '\n';
            } else {
                main.m_output << prefix.mov + prefix.reg + "rdx\n"; //! add byte and word types to this
            }
        }
        void gen(const NodeTermIdent* ident, const ExprInf& prefix){
            main.m_output << prefix.mov + prefix.reg + main.get_address(ident->ident, prefix) + '\n';
        }
        void gen(const NodeTermParen* paren, ExprInf& prefix){
            if (paren->only_too_large_imm) {
                main.m_output << "  mov rdx, " + 
                    std::to_string(std::get<NodeTermIntLit*>(std::get<NodeTerm*>(paren->expr->var)->var)->bin_num) + '\n';
            }
            main.gen_expr(paren->expr, prefix);
        }
        void gen(const NodeTermNotExpr* not_expr, ExprInf& prefix) {
            main.gen_term(not_expr->term, prefix);
            main.m_output << "  not" + main.get_before_comma(prefix.reg) + '\n';
        }
        void gen(const NodeTermNegExpr* neg_expr, ExprInf& prefix) {
            main.gen_term(neg_expr->term, prefix);
            main.m_output << "  neg" + main.get_before_comma(prefix.reg) + '\n';
        }
        void gen(const NodeTermLogNotExpr* log_not_expr, ExprInf& prefix) {
            prefix.if_inf.jump = !prefix.if_inf.jump;
            main.gen_expr(log_not_expr->expr, prefix);
        }
    };

private:
    const NodeRoot m_root;
    size_t m_stack_loc;
    size_t m_label_cnt = 0;
    size_t m_label_cnt_2 = 0;
    size_t m_extra_label_cnt = 0;
    size_t m_label_backtrack = 0;
    bool m_use_truth_label = false;
    size_t m_use_extra_label = 0;
    size_t m_use_mid_or_label = 0;
    size_t m_encapsulated_if_and = 0;
    size_t m_encapsulated_if_or = 0;
    std::vector<std::pair<std::string, std::string>> m_loop_labels;
    u_int8_t m_used_regs = 0;
    signed char m_last_mem_sign = 1;
    u_int32_t m_o_rax = 0;
    bool m_too_large_imm_used = 0;
    std::map<const char*, std::string, cmp_char_star> m_variables;
    std::map<int64_t, std::string> m_float_nums;
    Gen_stmt stmt_Generator {.main = *this};
    Gen_expr expr_Generator {.main = *this};
    Gen_bin_expr bin_expr_Generator {.main = *this};
    Gen_log_expr log_expr_Generator {.main = *this};
    Gen_term term_Generator {.main = *this};

    inline static const std::string rax = " rax, ";
    inline static const std::string eax = " eax, ";
    inline static const std::string xmm0 = " xmm0, ";

    const std::string make_var(const char* var, const VarType type) {
        u_int8_t size = var_sizes.at(type);
        std::string ind;
        switch (size) {
            case 1: 
                ind = "BYTE";
                break;
            case 2:
                ind = "WORD";
                break;
            case 4:
                ind = "DWORD";
                break;
            case 8:
                ind = "QWORD";
        }
        std::string var_loc = ind + " [rbp-" + std::to_string(m_stack_loc) + "]";
        m_variables.insert({var, var_loc});
        m_stack_loc -= size;
        return var_loc;
    }

    // returns a_reg without spaces or commas attached
    const std::string get_a_reg_exact_size(const VarType type) {
        switch (type) {
            case VarType::INT_64:
                return "rax";
            case VarType::INT_32:
                return "eax";
            case VarType::INT_16:
                return "ax";
            case VarType::BYTE:
            case VarType::BOOL:
                return "al";
            case VarType::DOUBLE:
            case VarType::FLOAT:
                return "xmm0";
            default:
                std::cout << "Something went wrong" << std::endl;
                return "";
        }
    }

    const std::string get_a_reg(const OpType opType) {
        switch (opType) {
            case OpType::SLONG:
            case OpType::LONG:
                return rax;
            case OpType::SINT:
            case OpType::INT:
                return eax;
            case OpType::DOUBLE:
            case OpType::FLOAT:
                return xmm0;
            default:
                std::cout << "Something went wrong" << std::endl;
                return "";
        }
    }

    const std::string get_cmp_op(OpType opType, const bool equ) {
        switch (opType) {
            case OpType::FLOAT:
                return equ ? "  ucomiss" : "  comiss";
            case OpType::DOUBLE:
                return equ ? "  ucomisd" : "  comisd";
            default:
                return "  cmp";
        }
    }

    const inline std::pair<std::string, std::string> get_jump_op(const OpType opType, const std::string sf, 
                                                                    const std::string st, const std::string uf, 
                                                                    const std::string ut, const std::string uff, 
                                                                    const std::string uft) {
        std::pair<std::string, std::string> ret;
        if (opType == OpType::FLOAT || opType == OpType::DOUBLE) {
            ret.first = uft;
            ret.second = uff;    
        } else if (opType == OpType::SINT || opType == OpType::SLONG) {
            ret.first = sf;
            ret.second = st;
        } else {
            ret.first = uf;
            ret.second = ut; 
        }
        return ret;
    }

    const std::string get_rbp_term(const OpType& opType) {
        switch (opType) {
            case OpType::SLONG:
            case OpType::LONG:
            case OpType::DOUBLE:
                return "QWORD [rbp]";
            case OpType::SINT:
            case OpType::INT:
            case OpType::FLOAT:
                return "DWORD [rbp]";
            default:
                return "garbage";
        }
    }

    const std::string get_address(const char* var, const ExprInf& inf) {
        if (*var == '2') {
            return get_rbp_term(inf.opType);
        }
        return m_variables.at(var);
    }

    const ExprInf get_expr_inf(const VarType var_type, const bool _signed) {
        switch (var_type) {
            case VarType::INT_16:
            case VarType::BOOL:
            case VarType::BYTE:
                return _signed ? ExprInf{.mov = "  movsx", .reg = eax, .opType = OpType::SINT, .varType = var_type} : 
                    ExprInf{.mov = "  movzx", .reg = eax, .opType = OpType::INT, .varType = var_type};
            case VarType::INT_64:
                return _signed ? ExprInf{.mov = "  mov", .reg = rax, .opType = OpType::SLONG, .varType = var_type} : 
                    ExprInf{.mov = "  mov", .reg = rax, .opType = OpType::LONG, .varType = var_type};
            case VarType::INT_32:
                return _signed ? ExprInf{.mov = "  mov", .reg = eax, .opType = OpType::SINT, .varType = var_type} : 
                    ExprInf{.mov = "  mov", .reg = eax, .opType = OpType::INT, .varType = var_type};
            case VarType::FLOAT:
                return ExprInf{.mov = "  movss", .reg = xmm0, .opType = OpType::FLOAT, .varType = var_type};
            case VarType::DOUBLE:
                return ExprInf{.mov = "  movsd", .reg = xmm0, .opType = OpType::DOUBLE, .varType = var_type};
            default:
                std::cout << "Something went wrong" << std::endl;
                return empty;
        }
    }

    const OpType get_op_type(const VarType var_type, const bool _signed) {
        switch (var_type) {
            case VarType::INT_32:
            case VarType::INT_16:
            case VarType::BOOL:
            case VarType::BYTE:
                return _signed ? OpType::SINT : OpType::INT;
            case VarType::INT_64:
                return _signed ? OpType::SLONG : OpType::LONG;
            case VarType::FLOAT:
                return OpType::FLOAT;
            case VarType::DOUBLE:
                return OpType::DOUBLE;
            default:
                std::cout << "Something went wrong" << std::endl;
                return OpType::INT;
        }
    }
    
    inline const std::string get_reg_or_adr(const OpType opType, const size_t index, signed char mem_sign) {
        if (opType == OpType::SLONG || opType == OpType::LONG) {
            if (index < sizeof(QWORDS)/sizeof(*QWORDS)) {
                return QWORDS[index];
            } else {
                if (mem_sign == m_last_mem_sign) {
                    m_stack_loc -= 8 * mem_sign;
                }
                return "QWORD [rbp-" + std::to_string(m_stack_loc) + "]";
            }
        } else if (opType == OpType::FLOAT || opType == OpType::DOUBLE) {
            if (index < sizeof(XMMS)/sizeof(*XMMS)) {
                return XMMS[index];
            } else {
                if (opType == OpType::FLOAT) {
                    if (mem_sign == m_last_mem_sign) {
                        m_stack_loc -= 4 * mem_sign;
                    }
                    return "DWORD [rbp-" + std::to_string(m_stack_loc) + "]";
                } else {
                    if (mem_sign == m_last_mem_sign) {
                        m_stack_loc -= 8 * mem_sign;
                    }
                    return "QWORD [rbp-" + std::to_string(m_stack_loc) + "]";
                }
            }
        } else {
            if (index < sizeof(DWORDS)/sizeof(*DWORDS)) {
                return DWORDS[index];
            } else {
                if (mem_sign == m_last_mem_sign) {
                    m_stack_loc -= 4 * mem_sign;
                }
                return "DWORD [rbp-" + std::to_string(m_stack_loc) + "]";
            }
        }
    }

    inline const std::string get_next_reg_inc(const OpType opType) {
        std::string ret = get_reg_or_adr(opType, m_used_regs++, 1);
        m_last_mem_sign = 1;
        return ret;
    }

    inline const std::string get_next_reg(const OpType opType) {
        std::string ret = get_reg_or_adr(opType, m_used_regs, 1);
        m_last_mem_sign = 1;
        return ret;
    }

    inline const std::string get_last_reg(const OpType opType) {
        std::string ret = get_reg_or_adr(opType, m_used_regs - 1, -1);
        m_last_mem_sign = -1;
        return ret;
    }

    inline std::string get_before_comma(const std::string str) {
        return str.substr(0, str.length() - 2);
    }

    inline void add_label(const size_t cnt) {
        m_output << ".L" + std::to_string(cnt) + ":\n";
    }

    inline void add_extra_label() {
        m_output << ".EL" + std::to_string(m_extra_label_cnt++) + ":\n";
    }

    inline void cmov_reg(const std::string cmov_cc, const std::string dest, const std::string dest_byte, const std::string temp, 
                            const std::string parity_set, const std::string temp_val) {
        m_output << "  " + parity_set + ' ' + dest_byte + '\n' +
                    "  mov " + temp + ", " + temp_val + '\n' + 
                    "  cmov" + cmov_cc + ' ' + dest +  ", " + temp + '\n';
    }

    inline std::string add_float_num(const int64_t num, const std::string ro_cmd) {
        if (!m_float_nums.contains(num)) {
            std::string float_mem = "FI" + std::to_string(m_float_nums.size());
            m_section_rodata << "  " + float_mem + ro_cmd + std::to_string(num) + '\n';
            m_float_nums.insert({num, float_mem});
            return float_mem;
        } else return m_float_nums.at(num);
    }

    inline void make_imm_if_too_big(const int64_t num, const ExprInf& inf) {
        if (!fits_in_immediate(num, inf.varType)) {
            m_output << "  mov rdx, " + std::to_string(num) + '\n'; //! Byte and word types
        }
    }

    inline void disable_use_e(ExprInf& inf, const std::string& reg) {
        inf.reg = ' ' + reg + ", ";
        inf.use_e = -1;
    }

    inline void gen_expr_or_mov_reg(const bool move_reg, const NodeExpr* term, ExprInf& inf) {
        if (!move_reg) {
            gen_expr(term, inf);
        } else {
            m_output << inf.mov + inf.reg + get_last_reg(inf.opType) + '\n';
        }
    }

    // moves value into the a register and the next register (increments used_regs if use_e is 0)
    // returns next_reg
    inline std::string double_reg(const bool rh_first, const NodeExpr* term, ExprInf& inf, std::string& reg_wc) {
        if (!rh_first) {
            std::string next_reg = get_next_reg(inf.opType);
            gen_expr(term, inf);
            m_output << inf.mov + " " + next_reg + "," + reg_wc + '\n';
            if (!inf.use_e) m_used_regs++;
            return next_reg;
        } else {
            std::string next_reg = get_next_reg(inf.opType);
            m_output << inf.mov + inf.reg + get_last_reg(inf.opType) + '\n';
            m_output << inf.mov + " " + next_reg + "," + get_before_comma(inf.reg) + '\n';
            if (!inf.use_e) m_used_regs++;
            return next_reg;
        }
    }
    //TODO: what does prev do
    inline void gen_term_e(const NodeExpr* term, ExprInf& inf, std::string new_reg) {
        std::string prev = inf.reg;
        inf.reg = new_reg;
        gen_expr(term, inf);
        inf.use_e = -1;
    }

    // Does an instruction but flips registers if use_e is 0
    inline void do_instruc(ExprInf& inf, const std::string instruc, const std::string next_reg, const std::string reg_wc) {
            if (inf.use_e) {
                m_output << instruc + inf.reg + next_reg + '\n';
            } else {
                m_output << instruc + ' ' + next_reg + "," + reg_wc + '\n';
                disable_use_e(inf, next_reg);
            }
    }
};
