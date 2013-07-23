#include "FormulaTree.h"
#include <queue>

TreeNode::TreeNode() :
                formula(Formula()) {
    
}

TreeNode::TreeNode(Formula _formula) : 
                formula(_formula) {
}

void TreeNode::set_children(vector<TreeNode> _children_formulas) {
    for (int i = 0; i < _children_formulas.size(); ++ i) {
        _children_formulas[i].formula.convert_prenex();
        _children_formulas[i].formula.fix_universal_quantifier();
    }
    this->children_formulas = _children_formulas;
}

void TreeNode::output(FILE* out, int deep) {
    if (this->formula.get_formula() == NULL) {
        return ;
    }
    for (int i = 1; i < deep; ++ i) {
        fprintf(out, "   ");
    }
    this->formula.output(out);
}

bool FormulaTree::insert_node(vector<TreeNode> children_formulas, 
        int father_formula_id) {
    queue<TreeNode*> que;
    que.push(&root);
    while (! que.empty()) {
        TreeNode* cur_node = que.front();
        que.pop();
        if (cur_node->formula.formula_id == father_formula_id) {
            cur_node->set_children(children_formulas);
            return true;
        }
        for (int i = 0; i < cur_node->children_formulas.size(); ++ i) {
            que.push(&(cur_node->children_formulas[i]));
        }
    }
    return false;
}

void FormulaTree::output_node(FILE* out, TreeNode &node, int deep) {
    node.output(out, deep);
    if (node.children_formulas.size() == 0) {
        this->leaf_count ++;
    }
    for (int i = 0; i < node.children_formulas.size(); ++ i) {
        output_node(out, node.children_formulas[i], deep + 1);
    }
}

void FormulaTree::output(FILE* out) {
    output_node(out, root, 0);
}
