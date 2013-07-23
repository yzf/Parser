/* 
 * File:   FormulaTree.h
 * Author: yzf
 *
 * Created on July 21, 2013, 7:26 PM
 */

#ifndef _FORMULATREE_H_
#define	_FORMULATREE_H_

#include "Formula.h"
#include <vector>

enum OutputType {
    ALL,
    HENGZHANG,
    CABALAR,
};

enum LeafType {
    HENGZHANG_LEAF,
    CABALAR_LEAF
};

class Formula;

class TreeNode {
public:
    Formula formula;
    bool is_final_hz;
    bool is_cabalar;
    bool is_final_cabalar;
    vector<TreeNode> children_formulas;
public:
    TreeNode();
    TreeNode(Formula _formula);
    TreeNode(Formula _formula, bool _is_cabalar, bool _is_final_cabalar);
    void set_children(vector<TreeNode> _children_formulas);
    void output(FILE* out, int deep, OutputType type);
};

class FormulaTree {
public:
    TreeNode root;
    int leaf_count;
private:
    void output(FILE* out, OutputType type);
    void output_node(FILE* out, TreeNode &node, int deep, OutputType type);
    void mark_leaves_rec(TreeNode &node, LeafType type);
public:
    bool insert_node_children(vector<TreeNode> children_formulas, 
            int father_formula_id);
    bool append_node_child(TreeNode child, int father_formula_id);
    void mark_leaves(LeafType type);
    void output_all_process(FILE* out);
    void output_hengzhang_process(FILE* out);
    void output_cabalar_process(FILE* out);
};




#endif	/* _FORMULATREE_H_ */

