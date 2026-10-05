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
            // Currently selected layer, used to highlight the active row and as
            // the surface that drawing tools paint on
            auto selected_layer = canvas->get_selected_layer().lock();
            for (int i = 0; i < layer_count; i++) {
                auto layer = canvas->get_layer(i);
                if (auto layer_locked = layer.lock()) {
                    ImGui::PushID(i);

                    // Hide/show button
                    if (layer_locked->is_visible()) {
                        if (ImGui::Button("Hide")) {
                            layer_locked->toggle_visiblity();
                        }
                    } else {
                        if (ImGui::Button("Show")) {
                            layer_locked->toggle_visiblity();
                        }
                    }
                    ImGui::SameLine();

                    // Name selectable spanning the rest of the row (minus the delete button)
                    const float spacing = ImGui::GetStyle().ItemSpacing.x;
                    const float delete_btn_width = ImGui::GetFrameHeight();
                    const float name_width = ImGui::GetContentRegionAvail().x - delete_btn_width - spacing;
                    bool is_selected = selected_layer != nullptr
                        && selected_layer->get_uuid() == layer_locked->get_uuid();
                    if (ImGui::Selectable(layer_locked->get_name().c_str(), is_selected,
                        ImGuiSelectableFlags_None, ImVec2(name_width, 0.0f))) {
                        canvas->set_selected_layer(layer_locked->get_uuid());
                    }

                    // Drag source: allows dragging this layer row
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                        ImGui::SetDragDropPayload(LAYER_DRAG_PAYLOAD, &i, sizeof(i));
                        ImGui::SetTooltip("Dragging %s", layer_locked->get_name().c_str());
                        ImGui::EndDragDropSource();
                    }

                    // Drop target: move the dragged layer onto this row when dropped here
                    if (ImGui::BeginDragDropTarget()) {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(LAYER_DRAG_PAYLOAD)) {
                            int from = *static_cast<const int*>(payload->Data);
                            if (from != i) {
                                canvas->move_layer(from, i);
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImGui::SameLine();

                    // Delete button
                    if (ImGui::Button("X", ImVec2(delete_btn_width, 0.0f))) {
                        layer_to_remove = i;
                    }

                    ImGui::PopID();
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
