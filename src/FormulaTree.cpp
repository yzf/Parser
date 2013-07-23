#include "FormulaTree.h"
#include <queue>

TreeNode::TreeNode() :
                formula(Formula()), deep(0) {
    
}

TreeNode::TreeNode(Formula _formula, int _deep) : 
                formula(_formula), deep(_deep) {
}

void TreeNode::set_children(vector<TreeNode> _children_formulas) {
    for (int i = 0; i < _children_formulas.size(); ++ i) {
        _children_formulas[i].formula.convert_prenex();
        _children_formulas[i].formula.fix_universal_quantifier();
    }
    this->children_formulas = _children_formulas;
}

void TreeNode::output(FILE* out) {
    if (this->deep == 0) {
        return ;
    }
    for (int i = 1; i < this->deep; ++ i) {
        fprintf(out, "   ");
    }
    this->formula.output(out);
}

bool FormulaTree::insert_node(vector<TreeNode> children_formulas, TreeNode father) {
    queue<TreeNode*> que;
    que.push(&root);
    while (! que.empty()) {
        TreeNode* cur_node = que.front();
        que.pop();
        if (cur_node->deep > father.deep) {
            continue;
        }
        if (cur_node->deep == father.deep && 
                (cur_node->deep == 0 || cur_node->formula == father.formula)) {
            cur_node->set_children(children_formulas);
            return true;
        }
        for (int i = 0; i < cur_node->children_formulas.size(); ++ i) {
            que.push(&(cur_node->children_formulas[i]));
        }
    }
    return false;
}

void FormulaTree::output_node(FILE* out, TreeNode &node) {
    node.output(out);
    if (node.children_formulas.size() == 0) {
        this->leaf_count ++;
    }
    for (int i = 0; i < node.children_formulas.size(); ++ i) {
        output_node(out, node.children_formulas[i]);
    }
}

void FormulaTree::output(FILE* out) {
    output_node(out, root);
}
