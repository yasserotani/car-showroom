#pragma once

#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture();

    // Load image file (returns nullptr on failure)
    static Texture* LoadFromFile(const std::string& filepath);

    // Create a small solid-color texture (helper fallback)
    static Texture* CreateSolidColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

    void Bind(unsigned int unit = 0) const;
    void Destroy();

private:
    unsigned int m_id = 0;
};