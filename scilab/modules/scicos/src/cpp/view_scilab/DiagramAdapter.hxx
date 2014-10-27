/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef DIAGRAMADAPTER_HXX_
#define DIAGRAMADAPTER_HXX_

#include <string>
#include <vector>

#include "utilities.hxx"
#include "BaseAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/Diagram.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

class BlockAdapter;

class DiagramAdapter : public BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>
{
public:
    DiagramAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Diagram> adaptee);
    DiagramAdapter(const DiagramAdapter& adapter);
    ~DiagramAdapter();

    static const std::wstring getSharedTypeStr()
    {
        return L"diagram";
    }

    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

    std::vector<BlockAdapter*> getBlocks() const;
    void addBlock(const BlockAdapter* newBlock);
    void setBlocks(const std::vector<BlockAdapter*>& newBlocks);
    void clearBlocks();
    std::vector<LinkAdapter*> getLinks() const;
    void addLink(const LinkAdapter* newLink);
    void setLinks(const std::vector<LinkAdapter*>& newLinks);
    void clearLinks();
    std::vector<TextAdapter*> getTexts() const;
    void addText(const TextAdapter* newText);
    void setTexts(const std::vector<TextAdapter*>& newTexts);
    void clearTexts();

    types::InternalType* getContribContent() const;
    void setContribContent(types::InternalType* v);

private:
    std::vector<BlockAdapter*> blockAdapters;
    std::vector<LinkAdapter*> linkAdapters;
    std::vector<TextAdapter*> textAdapters;

    types::InternalType* contrib_content;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* MODELADAPTER_HXX_ */
