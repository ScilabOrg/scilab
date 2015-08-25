/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CFG_HXX__
#define __CFG_HXX__

#include <memory>

namespace slint
{

struct CFGNode
{
    std::shared_ptr<CFGNode> parent;
    unsigned int position;
    std::vector<std::shared_ptr<CFGNode>> children;

    inline static std::shared_ptr<CFGNode> getRoot()
    {
        CFGNode * root = new CFGNode();
        root->parent = nullptr;
        return std::shared_ptr<CFGNode>(root);
    }

    inline static std::shared_ptr<CFGNode> getNew()
    {
        return std::shared_ptr<CFGNode>(new CFGNode());
    }

    inline bool hasChildren() const
    {
        return !children.empty();
    }


    inline void addIfThen()
    {

    }


    inline static void appendChild(std::shared_ptr<CFGNode> & parent, std::shared_ptr<CFGNode> & child)
    {
        child->parent = parent;
        child->position = parent->children.size();
        parent->children.emplace_back(child);
    }

    inline void replace(std::shared_ptr<CFGNode> & node)
    {
        if (hasChildren())
        {
            addChildrenToFinal(node, children);
        }
        children.clear();

        node->parent = parent;
        node->position = position;

        if (parent)
        {
            parent->children[position] = node;
        }
    }

    inline static void addChildrenToFinal(std::shared_ptr<CFGNode> & node, std::vector<std::shared_ptr<CFGNode>> & children)
    {
        if (node->hasChildren())
        {
            for (auto c : node->children)
            {
                addChildrenToFinal(c, children);
            }
        }
        else
        {
            node->children = children;
        }
    }

    struct __Compare
    {
        inline bool operator()(const std::shared_ptr<CFGNode> & L, const std::shared_ptr<CFGNode> & R) const
        {
            return L.get() < R.get();
        }
    };
};

class CFGVisitor : public ast::ConstVisitor
{
    std::shared_ptr<CFGNode> nodeIn;
    std::shared_ptr<CFGNode> current;
    unsigned int E;
    unsigned int N;
    std::set<std::shared_ptr<CFGNode>, CFGNode::__Compare> visited;

public:

    CFGVisitor() { }

    inline unsigned int getMcCabeComplexity(const ast::FunctionDec & fd)
    {
        E = N = 0;
        nodeIn = CFGNode::getRoot();
        auto nodeOut = CFGNode::getNew();
        CFGNode::appendChild(nodeIn, nodeOut);
        current = nodeIn;

        fd.getBody().accept(*this);

        getMcCabeComplexity(nodeIn);
        visited.clear();
        nodeIn.reset();

        return E - N + 2;
    }

private:

    inline void getMcCabeComplexity(const std::shared_ptr<CFGNode> & node)
    {
        if (visited.find(node) == visited.end())
        {
            visited.emplace(node);
            ++N;
            E += node->children.size();
            for (const auto & c : node->children)
            {
                getMcCabeComplexity(c);
            }
            visited.erase(node);
        }
    }


    void visit(const ast::SeqExp & e)
    {
        const ast::exps_t & exps = e.getExps();
        for (const auto exp : exps)
        {
            exp->accept(*this);
        }
    }

    void visit(const ast::IfExp & e)
    {
        auto nodeIf = CFGNode::getNew();
        auto nodeThen = CFGNode::getNew();
        auto nodeElse = CFGNode::getNew();
        auto nodeAfter = CFGNode::getNew();
        CFGNode::appendChild(nodeThen, nodeAfter);
        CFGNode::appendChild(nodeElse, nodeAfter);
        CFGNode::appendChild(nodeIf, nodeThen);
        CFGNode::appendChild(nodeIf, nodeElse);
        std::wcerr << "zob=" << 1 << std::endl;
        current->replace(nodeIf);

        current = nodeThen;
        e.getThen().accept(*this);

        current = nodeElse;
        e.getElse().accept(*this);

        current = nodeAfter;
    }

    void visit(const ast::WhileExp & e)
    {
        auto nodeWhile = CFGNode::getNew();
        auto nodeDo = CFGNode::getNew();
        auto nodeAfter = CFGNode::getNew();
        CFGNode::appendChild(nodeWhile, nodeDo);
        CFGNode::appendChild(nodeWhile, nodeAfter);
        CFGNode::appendChild(nodeDo, nodeWhile);
        current->replace(nodeWhile);

        current = nodeDo;
        e.getBody().accept(*this);

        current = nodeAfter;
    }

    void visit(const ast::ForExp & e)
    {
        auto nodeFor = CFGNode::getNew();
        auto nodeDo = CFGNode::getNew();
        auto nodeAfter = CFGNode::getNew();
        CFGNode::appendChild(nodeFor, nodeDo);
        CFGNode::appendChild(nodeFor, nodeAfter);
        CFGNode::appendChild(nodeDo, nodeFor);
        current->replace(nodeFor);

        current = nodeDo;
        e.getBody().accept(*this);

        current = nodeAfter;
    }

    void visit(const ast::TryCatchExp & e)
    {
    }

    void visit(const ast::SelectExp & e)
    {

    }


    /* Nothing to do since each of the following exp doesn't introduce any arc */

    void visit(const ast::CaseExp & e)
    {
    }

    void visit(const ast::FunctionDec & e)
    {
    }

    void visit(const ast::CallExp & e)
    {
    }

    void visit(const ast::SimpleVar & e)
    {
    }

    void visit(const ast::NotExp & e)
    {
    }

    void visit(const ast::TransposeExp & e)
    {
    }

    void visit(const ast::MatrixExp & e)
    {
    }

    void visit(const ast::CellExp & e)
    {
    }

    void visit(const ast::OpExp & e)
    {
    }

    void visit(const ast::LogicalOpExp & e)
    {
    }

    void visit(const ast::ListExp & e)
    {
    }

    void visit(const ast::DollarVar & e)
    {
    }

    void visit(const ast::ColonVar & e)
    {
    }

    void visit(const ast::ArrayListVar & e)
    {
    }

    void visit(const ast::DoubleExp & e)
    {
    }

    void visit(const ast::BoolExp & e)
    {
    }

    void visit(const ast::StringExp & e)
    {
    }

    void visit(const ast::CommentExp & e)
    {
    }

    void visit(const ast::NilExp & e)
    {
    }

    void visit(const ast::CellCallExp & e)
    {
    }

    void visit(const ast::AssignExp & e)
    {
    }

    void visit(const ast::BreakExp & e)
    {
    }

    void visit(const ast::ContinueExp & e)
    {
    }

    void visit(const ast::ReturnExp & e)
    {
    }

    void visit(const ast::FieldExp & e)
    {
    }

    void visit(const ast::MatrixLineExp & e)
    {
    }

    void visit(const ast::ArrayListExp & e)
    {
    }

    void visit(const ast::AssignListExp & e)
    {
    }

    void visit(const ast::VarDec & e)
    {
    }

    void visit(const ast::OptimizedExp & e)
    {
    }

    void visit(const ast::DAXPYExp & e)
    {
    }
};

} //namespace slint

#endif // __CFG_HXX__
