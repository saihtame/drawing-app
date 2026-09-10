#include "layers_panel.hpp"
#include "imgui.h"
#include <iostream>


namespace Presentation {

void LayersPanel::render() {
    ImGui::Begin("Layers");

    // Get active document
    auto doc = doc_service.get_active_document();


    // Layer line items
    if (auto doc_locked = doc.lock()) {
        // Get canvas
        if (auto canvas = doc_locked->get_canvas().lock()) {
            // Top menu
            if (ImGui::Button("New Layer")) {
                canvas->add_layer();
            }
            // Layer line items
            int layer_count = canvas->get_layer_count();
            int layer_to_remove = -1;
            for (int i = 0; i < layer_count; i++) {
                auto layer = canvas->get_layer(i);
                if (auto layer_locked = layer.lock()) {
                    // Hide/show button
                    if (layer_locked->is_visible()) {
                        if (ImGui::Button(std::string("Hide##layer_btn_" + std::to_string(i)).c_str())) {
                            layer_locked->toggle_visiblity();
                        }
                    } else {
                        if (ImGui::Button(std::string("Show##layer_btn_" + std::to_string(i)).c_str())) {
                            layer_locked->toggle_visiblity();
                        }
                    }
                    // Name label
                    ImGui::SameLine();
                    ImGui::Text("%s", layer_locked->get_name().c_str());
                    ImGui::SameLine();
                    // Delete button
                    if (ImGui::Button(std::string("X##layer_btn_" + std::to_string(i)).c_str())) {
                        layer_to_remove = i;
                    }
                }
            }
            if (layer_to_remove != -1)
                canvas->remove_layer(layer_to_remove);
        } else {
            std::cerr << "Active document has invalid canvas" << std::endl;
        }
    }

    ImGui::End();
}

}
