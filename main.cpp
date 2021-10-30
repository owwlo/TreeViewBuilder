#include "TreeViewBuilder.h"
#include <iostream>

int main()
{
  TreeViewBuilder builder;
  auto &root = builder.addOrSetRootNode("foobar");
  root.addChild("42");
  auto &childB = root.addChild("9527");
  auto &childB1 = childB.addChild("foo");
  childB1.addChild("bar");
  auto &childB1B = childB1.addChild("wakaka");
  childB1B.addChild("Catchphrase!");
  auto &childC = root.addChild("There are three things I love in life:");
  childC.addChild("Kicking @ss");
  childC.addChild("TBD");
  childC.addChild("insert 3rd thing here");

  using ParamT = std::pair<std::string, TreeViewBuilder::Style>;
  for (const auto param : std::vector<ParamT>{
           {"SIMPLE",
            TreeViewBuilder::Style::SIMPLE},
           {"EXPAND",
            TreeViewBuilder::Style::EXPAND},
           {"ARROW",
            TreeViewBuilder::Style::ARROW},
       })
  {
    root.setName("Style: " + param.first);
    std::cout << builder.build(param.second, 4, 0) << std::endl;
  }
  return 0;
}
