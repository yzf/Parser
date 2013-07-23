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

class Formula;

class TreeNode {
public:
    Formula formula;
    bool is_cabalar;
    bool is_final_cabalar;
    vector<TreeNode> children_formulas;
public:
    TreeNode();
    TreeNode(Formula _formula);
    TreeNode(Formula _formula, bool _is_cabalar, bool _is_final_cabalar);
    void set_children(vector<TreeNode> _children_formulas);
    void output(FILE* out, int deep);
};

class FormulaTree {
public:
    TreeNode root;
    int leaf_count;
private:
    void output_node(FILE* out, TreeNode &node, int deep);
    void mark_final_cabalar_rec(TreeNode &node);
public:
    bool insert_node_children(vector<TreeNode> children_formulas, 
            int father_formula_id);
    bool append_node_child(TreeNode child, int father_formula_id);
    void mark_final_cabalars();
    void output(FILE* out);
};




#endif	/* _FORMULATREE_H_ */

