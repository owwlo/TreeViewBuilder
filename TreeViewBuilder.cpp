#include "TreeViewBuilder.h"

#include <iterator>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

const char kVerticalLine = '|';
const char kExpandChar = '+';
const char kEmptyChar = ' ';
const char kDash = '-';
const char kArrow = '>';

TreeViewBuilder::TreeViewBuilder() : root_(nullptr), numOfNodes_(0) {}

void TreeViewBuilder::TreeViewNode::setName(const std::string &name) { name_ = name; }

TreeViewBuilder::TreeViewNode &TreeViewBuilder::TreeViewNode::addChild(const std::string &name)
{
    children_.emplace_back(builder_, level_ + 1, name);
    return children_.back();
}

TreeViewBuilder::TreeViewNode::TreeViewNode(TreeViewBuilder &builder, const uint32_t level,
                                            const std::string &name)
    : builder_(builder), level_(level), name_(name)
{
    builder_.setNumOfNodes(builder.getNumOfNodes() + 1);
}

const std::list<TreeViewBuilder::TreeViewNode> &TreeViewBuilder::TreeViewNode::getChildren() const { return children_; }

uint32_t TreeViewBuilder::TreeViewNode::getIndentLevel() const { return level_; }

const std::string &TreeViewBuilder::TreeViewNode::getName() const { return name_; }

std::string TreeViewBuilder::build(Style style, const uint32_t indentSize,
                                   const uint32_t globalIntentSize)
{
    if (!root_)
    {
        return "";
    }
    std::vector<std::string> lines(numOfNodes_, "");
    buildImpl(*root_, lines, style, indentSize, globalIntentSize);

    const char *const delim = "\n";
    std::ostringstream ss;
    std::copy(lines.begin(), lines.end(),
              std::ostream_iterator<std::string>(ss, delim));
    return ss.str();
}

TreeViewBuilder::TreeViewNode &TreeViewBuilder::addOrSetRootNode(const std::string &name)
{
    if (root_)
    {
        root_->setName(name);
    }
    else
    {
        root_.reset(new TreeViewNode(*this, 0, name));
    }
    return *root_;
}

std::size_t TreeViewBuilder::getNumOfNodes() { return numOfNodes_; }
void TreeViewBuilder::setNumOfNodes(const std::size_t num) { numOfNodes_ = num; }

void TreeViewBuilder::flatNodes(const TreeViewNode &node,
                                std::vector<const TreeViewNode *> &container,
                                uint32_t &maxIndent)
{
    maxIndent = std::max(maxIndent, node.getIndentLevel());
    container.push_back(&node);
    for (const auto &subNode : node.getChildren())
    {
        flatNodes(subNode, container, maxIndent);
    }
}

void TreeViewBuilder::clearRemainingIndents(std::vector<bool> &indents,
                                            const uint32_t setIndex)
{
    for (auto i = setIndex; i < indents.size(); i++)
    {
        indents[i] = false;
    }
}

TreeViewBuilder::StyleHeaders::StyleHeaders(const std::string &globalIndent,
                                            const std::string &expendedList,
                                            const std::string &emptyIntent, const std::string &itemHeader)
    : strGlobalIndent(globalIndent), strExpendedList(expendedList),
      strEmptyIntent(emptyIntent), strItemHeader(itemHeader) {}

std::string TreeViewBuilder::strMultiply(const std::string &str, int32_t times)
{
    std::ostringstream ss;
    while (times--)
    {
        ss << str;
    }
    return ss.str();
}

TreeViewBuilder::StyleHeaders TreeViewBuilder::generateStyle(const Style style, const uint32_t indentSize,
                                                             const uint32_t globalIndentSize)
{
    if (indentSize < 3)
    {
        throw std::runtime_error("Indent size should be >= 3.");
    }
    const uint32_t realIntent = indentSize - 1;

    const char *line;
    const char *arrow;
    const char *vertical;
    const char *empty = &kEmptyChar;
    switch (style)
    {
    case Style::PLAIN:
    {
        line = &kEmptyChar;
        arrow = &kEmptyChar;
        vertical = &kEmptyChar;
        break;
    }
    case Style::SIMPLE:
    {
        line = &kDash;
        arrow = &kDash;
        vertical = &kVerticalLine;
        break;
    }
    case Style::ARROW:
    {
        line = &kDash;
        arrow = &kArrow;
        vertical = &kVerticalLine;
        break;
    }
    case Style::EXPAND:
    {
        line = &kDash;
        arrow = &kDash;
        vertical = &kExpandChar;
        break;
    }
    default:
        throw std::runtime_error("Unsupport Style.");
    }

    const std::string strLine(1, *line);
    const std::string strArrow(1, *arrow);
    const std::string strVertical(1, *vertical);
    const std::string strEmpty(1, *empty);

    const std::string strGlobalIndent = strMultiply(strEmpty, globalIndentSize);
    const std::string strExpendedList =
        strVertical + strMultiply(strEmpty, realIntent - 1) + strEmpty;
    const std::string strEmptyIntent = strMultiply(strEmpty, realIntent) + strEmpty;
    const std::string strItemHeader =
        strVertical + strMultiply(strLine, realIntent - 2) + strArrow + strEmpty;
    return StyleHeaders(strGlobalIndent, strExpendedList, strEmptyIntent,
                        strItemHeader);
}

void TreeViewBuilder::buildImpl(TreeViewNode &node, std::vector<std::string> &lines,
                                Style style, const uint32_t indentSize,
                                const uint32_t globalIndentSize)
{
    // Flat nodes
    std::vector<const TreeViewNode *> nodes;
    nodes.reserve(numOfNodes_);
    uint32_t maxIndent = 0;
    flatNodes(node, nodes, maxIndent);

    const auto styleHeaders =
        generateStyle(style, indentSize, globalIndentSize);

    // Build the tree
    std::vector<bool> indentBitmap(maxIndent + 1, false);
    auto dst = lines.rbegin();
    for (auto src = nodes.rbegin(); src != nodes.rend(); src++, dst++)
    {
        const auto &node = **src;
        const auto indent = node.getIndentLevel();
        auto indentTmp = indent;
        std::string prefix = "";
        if (indentTmp > 0)
        {
            prefix = styleHeaders.strItemHeader;
            indentTmp--;
        }
        while (indentTmp > 0)
        {
            if (indentBitmap[indentTmp - 1])
            {
                prefix = styleHeaders.strExpendedList + prefix;
            }
            else
            {
                prefix = styleHeaders.strEmptyIntent + prefix;
            }
            indentTmp--;
        }
        *dst = styleHeaders.strGlobalIndent + prefix + node.getName();
        clearRemainingIndents(indentBitmap, indent);
        if (indent > 0)
        {
            indentBitmap[indent - 1] = true;
        }
    }
}
