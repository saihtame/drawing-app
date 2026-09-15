#pragma once
#include "domain/tools/tool.hpp"
#include <memory>
#include <vector>


namespace Application::Tools {

class Toolbar final {
public: // Ctors
    Toolbar(std::vector<std::shared_ptr<Domain::Tools::Tool>> tools);
    ~Toolbar() = default;

public: // Accessors
    /**
     * @brief Returns a readonly list of all tools
     */
    const std::vector<std::shared_ptr<Domain::Tools::Tool>> get_tools() const;

public: // Active tool
    /**
     * @brief Returns the tool selected and active.
     * If none has been selected nullptr is returned.
     */
    std::shared_ptr<Domain::Tools::Tool> get_active_tool() const;
    /**
     * @brief Sets the active tool
     *
     * @param value Tool to be selected.
     */
    void set_active_tool(std::shared_ptr<Domain::Tools::Tool> value);

private: // Data
    std::shared_ptr<Domain::Tools::Tool> active_tool = nullptr;
    std::vector<std::shared_ptr<Domain::Tools::Tool>> loaded_tools;
};

}
