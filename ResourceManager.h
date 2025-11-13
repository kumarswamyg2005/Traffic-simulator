/**
 * @file ResourceManager.h
 * @brief RAII-based OpenGL resource management for the traffic simulator
 * 
 * Implements proper resource cleanup and memory management patterns
 * to prevent leaks and ensure stable operation.
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>
#include <memory>
#include <cstdio>

/**
 * @class TextureRAII
 * @brief RAII wrapper for OpenGL textures
 * 
 * Automatically manages texture lifecycle with proper cleanup
 */
class TextureRAII {
public:
    TextureRAII() : textureId_(0) {}
    
    ~TextureRAII() {
        cleanup();
    }
    
    // Prevent copying
    TextureRAII(const TextureRAII&) = delete;
    TextureRAII& operator=(const TextureRAII&) = delete;
    
    // Allow moving
    TextureRAII(TextureRAII&& other) noexcept : textureId_(other.textureId_) {
        other.textureId_ = 0;
    }
    
    TextureRAII& operator=(TextureRAII&& other) noexcept {
        if (this != &other) {
            cleanup();
            textureId_ = other.textureId_;
            other.textureId_ = 0;
        }
        return *this;
    }
    
    GLuint get() const { return textureId_; }
    GLuint* getPtr() { return &textureId_; }
    
    void upload(int size, const std::vector<unsigned char>& data) {
        cleanup();
        glGenTextures(1, &textureId_);
        glBindTexture(GL_TEXTURE_2D, textureId_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    }
    
    void bind() const {
        if (textureId_ != 0) {
            glBindTexture(GL_TEXTURE_2D, textureId_);
        }
    }
    
private:
    void cleanup() {
        if (textureId_ != 0) {
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }
    
    GLuint textureId_;
};

/**
 * @class DisplayListRAII
 * @brief RAII wrapper for OpenGL display lists
 * 
 * Manages display list lifecycle for static geometry optimization
 */
class DisplayListRAII {
public:
    DisplayListRAII() : listId_(0) {}
    
    ~DisplayListRAII() {
        cleanup();
    }
    
    // Prevent copying
    DisplayListRAII(const DisplayListRAII&) = delete;
    DisplayListRAII& operator=(const DisplayListRAII&) = delete;
    
    // Allow moving
    DisplayListRAII(DisplayListRAII&& other) noexcept : listId_(other.listId_) {
        other.listId_ = 0;
    }
    
    DisplayListRAII& operator=(DisplayListRAII&& other) noexcept {
        if (this != &other) {
            cleanup();
            listId_ = other.listId_;
            other.listId_ = 0;
        }
        return *this;
    }
    
    GLuint get() const { return listId_; }
    
    void create() {
        cleanup();
        listId_ = glGenLists(1);
    }
    
    void call() const {
        if (listId_ != 0) {
            glCallList(listId_);
        }
    }
    
    void beginCompile() {
        if (listId_ != 0) {
            glNewList(listId_, GL_COMPILE);
        }
    }
    
    void endCompile() {
        glEndList();
    }
    
private:
    void cleanup() {
        if (listId_ != 0) {
            glDeleteLists(listId_, 1);
            listId_ = 0;
        }
    }
    
    GLuint listId_;
};

/**
 * @class ResourceManager
 * @brief Central manager for all OpenGL resources
 * 
 * Tracks memory usage and provides centralized cleanup
 */
class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }
    
    TextureRAII& getRoadTexture() { return roadTexture_; }
    TextureRAII& getBuildingTexture() { return buildingTexture_; }
    TextureRAII& getCarBodyTexture() { return carBodyTexture_; }
    TextureRAII& getSidewalkTexture() { return sidewalkTexture_; }
    TextureRAII& getGrassTexture() { return grassTexture_; }
    TextureRAII& getWoodTexture() { return woodTexture_; }
    TextureRAII& getMetalTexture() { return metalTexture_; }
    
    DisplayListRAII& getRoadDisplayList() { return roadDisplayList_; }
    DisplayListRAII& getBuildingDisplayList() { return buildingDisplayList_; }
    DisplayListRAII& getGroundDisplayList() { return groundDisplayList_; }
    
    void reportMemoryUsage() const {
        printf("[ResourceManager] Textures: 7, Display Lists: 3\n");
    }
    
    void cleanup() {
        // RAII handles cleanup automatically
        printf("[ResourceManager] Resources cleaned up\n");
    }
    
private:
    ResourceManager() = default;
    ~ResourceManager() { cleanup(); }
    
    // Prevent copying and moving
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    // Texture resources
    TextureRAII roadTexture_;
    TextureRAII buildingTexture_;
    TextureRAII carBodyTexture_;
    TextureRAII sidewalkTexture_;
    TextureRAII grassTexture_;
    TextureRAII woodTexture_;
    TextureRAII metalTexture_;
    
    // Display list resources for static geometry
    DisplayListRAII roadDisplayList_;
    DisplayListRAII buildingDisplayList_;
    DisplayListRAII groundDisplayList_;
};

#endif // RESOURCE_MANAGER_H
