#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

class TreeViewBuilder
{
public:
    TreeViewBuilder();

    enum class Style
    {
        PLAIN,
        SIMPLE,
        ARROW,
        EXPAND,
    };

    class TreeViewNode
    {
    public:
        explicit TreeViewNode(TreeViewBuilder &builder, const uint32_t level,
                              const std::string &name);

        void setName(const std::string &name);

        TreeViewNode &addChild(const std::string &name);

        const std::list<TreeViewNode> &getChildren() const;

        uint32_t getIndentLevel() const;

        const std::string &getName() const;

    private:
        TreeViewBuilder &builder_;
        uint32_t level_;
        std::string name_;
        std::list<TreeViewNode> children_;
    };

    std::string build(Style style = Style::SIMPLE, const uint32_t indentSize = 4,
                      const uint32_t globalIntentSize = 0);

    TreeViewNode &addOrSetRootNode(const std::string &name);

protected:
    std::size_t getNumOfNodes();
    void setNumOfNodes(const std::size_t num);

private:
    void flatNodes(const TreeViewNode &node,
                   std::vector<const TreeViewNode *> &container,
                   uint32_t &maxIndent);

    void clearRemainingIndents(std::vector<bool> &indents,
                               const uint32_t setIndex);

    struct StyleHeaders
    {
        StyleHeaders(const std::string &globalIndent,
                     const std::string &expendedList,
                     const std::string &emptyIntent, const std::string &itemHeader);

        const std::string strGlobalIndent;
        const std::string strExpendedList;
        const std::string strEmptyIntent;
        const std::string strItemHeader;
    };

    std::string strMultiply(const std::string &str, int32_t times);

    StyleHeaders generateStyle(const Style style, const uint32_t indentSize,
                               const uint32_t globalIndentSize);

    void buildImpl(TreeViewNode &node, std::vector<std::string> &lines,
                   Style style, const uint32_t indentSize,
                   const uint32_t globalIndentSize);

    std::unique_ptr<TreeViewNode> root_;
    std::size_t numOfNodes_;
};