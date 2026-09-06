#pragma once


namespace Presentation::Popups {

class Popup {
public:
    virtual ~Popup() = default;

public: // Virtual methods
    virtual bool render() = 0;
    virtual const char* get_name() const = 0;
};

}
