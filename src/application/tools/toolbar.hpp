#include "domain/tools/tool.hpp"
#include <memory>
#include <vector>


namespace Application::Tools {

class Toolbar final {
public: // Ctors
    Toolbar(std::vector<std::shared_ptr<Domain::Tools::Tool>> tools);
    ~Toolbar() = default;

public: // Accessors
    const std::vector<std::shared_ptr<Domain::Tools::Tool>> get_tools() const;

public: // Active tool
    std::shared_ptr<Domain::Tools::Tool> get_active_tool() const;
    void set_active_tool(std::shared_ptr<Domain::Tools::Tool> value);

private: // Data
    std::shared_ptr<Domain::Tools::Tool> active_tool = nullptr;
    std::vector<std::shared_ptr<Domain::Tools::Tool>> loaded_tools;
};

}
