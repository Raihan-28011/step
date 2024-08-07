#include "OpCode.hpp"
#include "VirtualMachin.hpp"
int main() {
    auto &vm = Step::StackVirtualMachine::instance();

    vm.push_back(Step::OpCode::iconst);
    vm.push_back(100);

    // vm.run();
    vm.print();
    
}
