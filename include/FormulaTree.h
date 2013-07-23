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
    int deep;
    vector<TreeNode> children_formulas;
public:
    TreeNode();
    TreeNode(Formula _formula, int _deep);
    void set_children(vector<TreeNode> _children_formulas);
    void output(FILE* out);
};

class FormulaTree {
public:
    TreeNode root;
    int leaf_count;
private:
    void output_node(FILE* out, TreeNode &node);
public:
    bool insert_node(vector<TreeNode> children_formulas, TreeNode father);
    void output(FILE* out);
};




#endif	/* _FORMULATREE_H_ */

