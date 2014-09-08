#include <functional>
#include <stack>
#include <string>
#include <ccspec/core/example_group.h>

namespace ccspec {
namespace core {

using std::function;
using std::stack;
using std::string;

stack<ExampleGroup*> groups_being_defined;

// Public methods.

ExampleGroup::~ExampleGroup() {
    for (auto& child : children_)
        delete child;
}

void ExampleGroup::addExample(Example example) {
    examples_.push_back(example);
}

void ExampleGroup::run() {
    for (auto& example : examples_)
        example.run();
    for (auto child : children_)
        child->run();
}

// Private methods.

ExampleGroup::ExampleGroup(string desc) : desc_(desc) {}

void ExampleGroup::addChild(ExampleGroup* example_group) {
    children_.push_back(example_group);
}

// Friend methods.

ExampleGroup* describe(string desc, function<void ()> spec) {
    ExampleGroup* example_group = new ExampleGroup(desc);

    if (!groups_being_defined.empty()) {
        ExampleGroup* current_group = groups_being_defined.top();
        current_group->addChild(example_group);
    }

    groups_being_defined.push(example_group);
    spec();
    groups_being_defined.pop();

    return example_group;
}

ExampleGroup* context(string desc, function<void ()> spec) {
    return describe(desc, spec);
}

} // namespace core
} // namespace ccspec
