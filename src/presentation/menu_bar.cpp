#include "menu_bar.hpp"
#include "imgui.h"
#include "presentation/popups/new_file_popup.hpp"
#include <nfd.h>


namespace  Presentation {

MenuBar::MenuBar(CanvasPanels::CanvasPanelService& canvas_panel_service,
                 Application::Documents::DocumentService& doc_service,
                 Popups::PopupManager& popup_manager)
 : canvas_panel_service(canvas_panel_service), doc_service(doc_service),
   popup_manager(popup_manager) {
}

void MenuBar::render() {
    auto active_document = doc_service.get_active_document();
    bool has_active_document = !active_document.expired();

    if(ImGui::BeginMainMenuBar()) {
        /* File menu */
        if (ImGui::BeginMenu("File", true)) {
            // New File
            if (ImGui::MenuItem("New file"))
                new_file();
            // Load File
            if (ImGui::MenuItem("Load file"))
                load_file();
            // Save File
            if (ImGui::MenuItem("Save file", "", false, has_active_document))
                save_file();
            // Save File As
            if (ImGui::MenuItem("Save file as", "", false, has_active_document))
                save_file_as();
            // Export As PNG
            if (ImGui::MenuItem("Export as PNG...", "", false, has_active_document))
                export_file();
            ImGui::EndMenu();
        }
        /* View menu */
        if (ImGui::BeginMenu("View", true)) {
            // Fit to screen
            if (ImGui::MenuItem("Fit to screen", "", false, has_active_document))
                fit_to_screen();
            // Zoom menu
            if (ImGui::BeginMenu("Zoom", has_active_document)) {
                if (ImGui::MenuItem("200%", "", false))
                    set_zoom(2.0);
                if (ImGui::MenuItem("150%", "", false))
                    set_zoom(1.5);
                if (ImGui::MenuItem("100%", "", false))
                    set_zoom(1.0);
                if (ImGui::MenuItem("50%", "", false))
                    set_zoom(0.5);
                if (ImGui::MenuItem("25%", "", false))
                    set_zoom(0.25);
                ImGui::EndMenu();
            }
            // Center canvas
            if (ImGui::MenuItem("Center canvas", "", false, has_active_document))
                center_canvas();
            ImGui::EndMenu();
        }
        /* View menu */
        ImGui::EndMainMenuBar();
    }
}

inline void MenuBar::new_file() {
    auto popup = std::make_shared<Popups::NewFilePopup>(doc_service);
    popup_manager.push_popup(popup);
}

inline void MenuBar::load_file() {
    std::cout << "Getting load file path from dialog" << std::endl;
    std::filesystem::path load_path;
    NFD_Init();
    nfdu8char_t *out_path;
    nfdu8filteritem_t filters[1] = { { "Project File", "dproj" } };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);
    if (result == NFD_OKAY)
    {
        if (out_path == nullptr)
        {
            std::cerr << "Load file dialog did not return a file path" << std::endl;
            NFD_Quit();
            return;
        }
        load_path = out_path;
        std::cout << "Got path: " << load_path << std::endl;
        NFD_FreePathU8(out_path);
    }
    else if (result == NFD_CANCEL)
    {
        std::cout << "User cancelled dialog" << std::endl;
        NFD_Quit();
        return;
    }
    else 
    {
        std::cerr << "Load file dialog error: " << NFD_GetError() << std::endl;
        NFD_Quit();
        return;
    }
    NFD_Quit();

    doc_service.load_document(load_path);
}

inline void MenuBar::save_file() {
    // Get the active document
    auto doc = doc_service.get_active_document();
    auto doc_locked = doc.lock();
    if (doc_locked == nullptr) return; // Do nothing, if we have no active document

    // Check if we have a save path associated with the document
    auto save_path = doc_locked->get_save_path();
    if (!save_path.has_value()) {
        save_file_as(); // Do with a save file popup, to get a path.
        return;
    }

    // Save document
    doc_locked->set_save_path(save_path.value());
    doc_service.save_document(save_path.value(), *doc_locked);
}

inline void MenuBar::save_file_as() {
    // Get the active document
    auto doc = doc_service.get_active_document();
    auto doc_locked = doc.lock();
    if (doc_locked == nullptr) return; // Do nothing, if we have no active document

    // Get save path from OS dialog
    std::cout << "Getting save file path from dialog..." << std::endl;
    std::filesystem::path save_path;
    NFD_Init();
    nfdu8char_t *out_path;
    nfdu8filteritem_t filters[1] = { { "Project File", "dproj" } };
    nfdsavedialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_SaveDialogU8_With(&out_path, &args);
    if (result == NFD_OKAY) {
        if (out_path == nullptr) {
            std::cerr << "Save file dialog did not return a file path" << std::endl;
            NFD_Quit();
            return;
        }
        save_path = out_path;
        std::cout << "Got path: " << save_path << std::endl;
        NFD_FreePathU8(out_path);
    }
    else if (result == NFD_CANCEL) {
        std::cout << "User cancelled dialog" << std::endl;
        NFD_Quit();
        return;
    }
    else {
        std::cerr << "Save file dialog error: " << NFD_GetError() << std::endl;
        NFD_Quit();
        return;
    }
    NFD_Quit();

    // Save document
    doc_locked->set_save_path(save_path);
    doc_service.save_document(save_path, *doc_locked);
}

inline void MenuBar::export_file() {
    // Get the active document
    auto doc = doc_service.get_active_document();
    auto doc_locked = doc.lock();
    if (doc_locked == nullptr) return; // Do nothing, if we have no active document

    // Get export path from OS dialog
    std::cout << "Getting export file path from dialog..." << std::endl;
    std::filesystem::path export_path;

    // Start in the document's folder (or cwd) so the chooser always has a real
    // directory selected; virtual locations can yield no local file path.
    std::filesystem::path default_dir = std::filesystem::current_path();
    if (auto document_save_path = doc_locked->get_save_path();
        document_save_path.has_value() && !document_save_path->parent_path().empty()) {
        default_dir = document_save_path->parent_path();
    }
    std::error_code dir_error;
    if (!std::filesystem::is_directory(default_dir, dir_error))
        default_dir = std::filesystem::current_path();
    const std::string default_dir_string = default_dir.string();

    NFD_Init();
    nfdu8char_t *out_path;
    nfdu8filteritem_t filters[1] = { { "PNG Image", "png" } };
    nfdsavedialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    args.defaultPath = default_dir_string.c_str();
    args.defaultName = "untitled.png";
    nfdresult_t result = NFD_SaveDialogU8_With(&out_path, &args);
    if (result == NFD_OKAY) {
        if (out_path == nullptr) {
            std::cerr << "Export dialog did not return a file path" << std::endl;
            NFD_Quit();
            return;
        }
        export_path = out_path;
        std::cout << "Got path: " << export_path << std::endl;
        NFD_FreePathU8(out_path);
    }
    else if (result == NFD_CANCEL) {
        std::cout << "User cancelled dialog" << std::endl;
        NFD_Quit();
        return;
    }
    else {
        std::cerr << "Export file dialog error: " << NFD_GetError() << std::endl;
        NFD_Quit();
        return;
    }
    NFD_Quit();

    // PNG is the only supported format
    if (export_path.extension() != ".png")
        export_path.replace_extension(".png");

    // Export document
    doc_service.export_png(export_path, *doc_locked);
}

inline void MenuBar::fit_to_screen() {
    auto active_document = doc_service.get_active_document();
    if (auto document_locked = active_document.lock()) {
        auto panel = canvas_panel_service.get_canvas_panel(document_locked->get_uuid());
        if (auto panel_locked = panel.lock()) {
            panel_locked->request_fit_to_screen();
        }
    }
}

inline void MenuBar::set_zoom(float value) {
    auto active_document = doc_service.get_active_document();
    if (auto document_locked = active_document.lock()) {
        auto& canvas = document_locked->get_canvas();
        canvas.set_zoom(value);
    }
}

inline void MenuBar::center_canvas() {
    auto active_document = doc_service.get_active_document();
    if (auto document_locked = active_document.lock()) {
        auto& canvas = document_locked->get_canvas();
        canvas.set_rendering_offset({0.0, 0.0});
    }
}

}

