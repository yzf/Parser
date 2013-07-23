#include "FormulaTree.h"
#include "S2DLP.h"
#include <queue>
#include <assert.h>

TreeNode::TreeNode() :
                formula(Formula()), is_final_hz(false),
                is_cabalar(false), is_final_cabalar(false) {
    
}

TreeNode::TreeNode(Formula _formula) : 
                formula(_formula), is_final_hz(false),
                is_cabalar(false), is_final_cabalar(false) {
}

TreeNode::TreeNode(Formula _formula, bool _is_cabalar, bool _is_final_cabalar) :
                formula(_formula), is_final_hz(false), 
                is_cabalar(_is_cabalar), is_final_cabalar(_is_final_cabalar){
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

void TreeNode::output(FILE* out, int deep, OutputType type) {
    if (this->formula.get_formula() == NULL) {
        return ;
    }
    switch (type) {
        case HENGZHANG:
            if (! this->is_cabalar) {
                for (int i = 1; i < deep; ++ i) {
                    fprintf(out, "    ");
                }
                this->formula.output(out);
            }
            break;
        case CABALAR:
            if (this->is_final_hz || this->is_cabalar) {
                for (int i = 1; i < deep; ++ i) {
                    fprintf(out, "    ");
                }
                this->formula.output(out);
                if (this->is_final_cabalar) {
                    Rule rule = Rule(this->formula);
                    for (int i = 0; i < deep; ++ i) {
                        fprintf(out, "    ");
                    }
                    rule.output(out);
                    fprintf(out, "\n");
                }
            }
            break;
        case ALL:
            for (int i = 1; i < deep; ++ i) {
                fprintf(out, "    ");
            }
            this->formula.output(out);
            if (this->is_final_cabalar) {
                Rule rule = Rule(this->formula);
                for (int i = 0; i < deep; ++ i) {
                    fprintf(out, "    ");
                }
                rule.output(out);
                fprintf(out, "\n");
            }
            break;
        default:
            assert(0);
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

void FormulaTree::output_node(FILE* out, TreeNode &node, int deep, OutputType type) {
    node.output(out, deep, type);
    if (node.children_formulas.size() == 0) {
        this->leaf_count ++;
    }
    for (int i = 0; i < node.children_formulas.size(); ++ i) {
        output_node(out, node.children_formulas[i], deep + 1, type);
    }
}

void FormulaTree::output(FILE* out, OutputType type) {
    output_node(out, root, 0, type);
}

void FormulaTree::mark_leaves_rec(TreeNode &node, LeafType type) {
    assert(type == HENGZHANG_LEAF || type == CABALAR_LEAF);
    if (node.children_formulas.empty()) {
        if (type == HENGZHANG_LEAF) {
            node.is_final_hz = true;
        }
        else {
            node.is_cabalar = true;
            node.is_final_cabalar = true;
        }
        return;
    }
    for (int i = 0; i < node.children_formulas.size(); ++ i) {
        mark_leaves_rec(node.children_formulas[i], type);
    }
}

void FormulaTree::mark_leaves(LeafType type) {
    mark_leaves_rec(this->root, type);
}

void FormulaTree::output_all_process(FILE* out) {
    output(out, ALL);
}

void FormulaTree::output_hengzhang_process(FILE* out) {
    output(out, HENGZHANG);
}

void FormulaTree::output_cabalar_process(FILE* out) {
    output(out, CABALAR);
}

