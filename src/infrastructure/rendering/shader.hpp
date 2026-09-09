#pragma once
#include <filesystem>
#include <string>
#include <glad/gl.h>


namespace Infrastructure::Rendering {

class Shader final {
public:
    Shader(std::filesystem::path path, GLenum shader_type);
    ~Shader();
    GLuint get_id();

private:
    GLuint shader_id = 0;

private:
    inline std::string read_file(std::string path);
};

}
