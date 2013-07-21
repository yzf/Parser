/* 
 * File:   HengZhangTree.h
 * Author: yzf
 *
 * Created on July 21, 2013, 7:26 PM
 */

#ifndef HENGZHANGTREE_H
#define	HENGZHANGTREE_H

#include "Formula.h"
#include <vector>

class Formula;

class HengZhangNode {
public:
    Formula formula;
    int deep;
    vector<HengZhangNode> children_formulas;
public:
    HengZhangNode();
    HengZhangNode(Formula _formula, int _deep);
    void set_children(vector<HengZhangNode> _children_formulas);
    void output(FILE* out);
};

class HengZhangTree {
public:
    HengZhangNode root;
    int leaf_count;
private:
    void output_node(FILE* out, HengZhangNode &node);
public:
    bool insert_node(vector<HengZhangNode> children_formulas, HengZhangNode father);
    void output(FILE* out);
};




#endif	/* HENGZHANGTREE_H */

