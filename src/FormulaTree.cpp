#include "FormulaTree.h"
#include "S2DLP.h"
#include <queue>

TreeNode::TreeNode() :
                formula(Formula()), is_cabalar(false), is_final_cabalar(false) {
    
}

TreeNode::TreeNode(Formula _formula) : 
                formula(_formula), is_cabalar(false), 
                is_final_cabalar(false) {
}

TreeNode::TreeNode(Formula _formula, bool _is_cabalar, bool _is_final_cabalar) :
                formula(_formula), is_cabalar(_is_cabalar), 
                is_final_cabalar(_is_final_cabalar){
}

void TreeNode::set_children(vector<TreeNode> _children_formulas) {
    for (int i = 0; i < _children_formulas.size(); ++ i) {
        if (! _children_formulas[i].is_cabalar) {
            _children_formulas[i].formula.convert_prenex();
            _children_formulas[i].formula.fix_universal_quantifier();
        }
    }
    this->children_formulas = _children_formulas;
}

void TreeNode::output(FILE* out, int deep) {
    if (this->formula.get_formula() == NULL) {
        return ;
    }
    fprintf(out, "%%%s", (this->is_cabalar ? "cabalar:  ":"hengzhang:"));
    for (int i = 1; i < deep; ++ i) {
        fprintf(out, "    ");
    }
    this->formula.output(out);
    if (this->is_final_cabalar) {
        Rule rule = Rule(this->formula);
        rule.output(out);
        fprintf(out, "\n");
    }
}

bool FormulaTree::insert_node_children(vector<TreeNode> children_formulas, 
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

bool FormulaTree::append_node_child(TreeNode child, int father_formula_id) {
    queue<TreeNode*> que;
    que.push(&root);
    while (! que.empty()) {
        TreeNode* cur_node = que.front();
        que.pop();
        if (cur_node->formula.formula_id == father_formula_id) {
            cur_node->children_formulas.push_back(child);
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

void FormulaTree::mark_final_cabalar_rec(TreeNode &node) {
    if (node.children_formulas.empty()) {
        node.is_cabalar = true;
        node.is_final_cabalar = true;
        return;
    }
    for (int i = 0; i < node.children_formulas.size(); ++ i) {
        mark_final_cabalar_rec(node.children_formulas[i]);
    }
}

void FormulaTree::mark_final_cabalars() {
    mark_final_cabalar_rec(this->root);
}
