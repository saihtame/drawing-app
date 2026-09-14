#include "toolbar.hpp"


namespace Application::Tools {


Toolbar::Toolbar(std::vector<std::shared_ptr<Domain::Tools::Tool>> tools) {
    loaded_tools = tools;
}

const std::vector<std::shared_ptr<Domain::Tools::Tool>> Toolbar::get_tools() const {
    return loaded_tools;
}

std::shared_ptr<Domain::Tools::Tool> Toolbar::get_active_tool() const {
    return active_tool;
}

void Toolbar::set_active_tool(std::shared_ptr<Domain::Tools::Tool> value) {
    active_tool = value;
}

}
