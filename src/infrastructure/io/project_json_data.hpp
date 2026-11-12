#pragma once
#include <cstdint>
#include <string>
#include <vector>


namespace Infrastructure::IO {

struct LayerJsonData {
    /** @brief Display name of the layer. */
    std::string name;
    /** @brief Name of the archive entry holding the layer's image. */
    std::string file;
};

struct ProjectJsonData {
    uint32_t size_width;
    uint32_t size_height;
    std::vector<LayerJsonData> layers;
};

}
