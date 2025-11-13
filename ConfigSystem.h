/**
 * @file ConfigSystem.h
 * @brief Configuration management system
 * 
 * Provides a simple key-value configuration system for simulation parameters
 * Supports loading from files and runtime parameter adjustment
 */

#ifndef CONFIG_SYSTEM_H
#define CONFIG_SYSTEM_H

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

/**
 * @class ConfigSystem
 * @brief Manages simulation configuration parameters
 * 
 * Simple text-based config file format:
 * key=value
 * # comments start with #
 */
class ConfigSystem {
public:
    static ConfigSystem& getInstance() {
        static ConfigSystem instance;
        return instance;
    }
    
    // Load configuration from file
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            printf("[Config] Warning: Could not open config file '%s', using defaults\n", 
                   filename.c_str());
            return false;
        }
        
        std::string line;
        int lineNum = 0;
        while (std::getline(file, line)) {
            lineNum++;
            
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;
            
            // Find the = separator
            size_t equalsPos = line.find('=');
            if (equalsPos == std::string::npos) {
                printf("[Config] Warning: Invalid line %d: '%s'\n", lineNum, line.c_str());
                continue;
            }
            
            // Extract key and value
            std::string key = trim(line.substr(0, equalsPos));
            std::string value = trim(line.substr(equalsPos + 1));
            
            if (!key.empty()) {
                config_[key] = value;
            }
        }
        
        file.close();
        printf("[Config] Loaded %zu parameters from '%s'\n", config_.size(), filename.c_str());
        return true;
    }
    
    // Save configuration to file
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            printf("[Config] Error: Could not write config file '%s'\n", filename.c_str());
            return false;
        }
        
        file << "# Traffic Simulator Configuration\n";
        file << "# Generated configuration file\n\n";
        
        for (const auto& pair : config_) {
            file << pair.first << "=" << pair.second << "\n";
        }
        
        file.close();
        printf("[Config] Saved %zu parameters to '%s'\n", config_.size(), filename.c_str());
        return true;
    }
    
    // Set parameter
    void set(const std::string& key, const std::string& value) {
        config_[key] = value;
    }
    
    void setInt(const std::string& key, int value) {
        config_[key] = std::to_string(value);
    }
    
    void setFloat(const std::string& key, float value) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.3f", value);
        config_[key] = buf;
    }
    
    void setBool(const std::string& key, bool value) {
        config_[key] = value ? "true" : "false";
    }
    
    // Get parameter with default
    std::string getString(const std::string& key, const std::string& defaultValue = "") const {
        auto it = config_.find(key);
        return (it != config_.end()) ? it->second : defaultValue;
    }
    
    int getInt(const std::string& key, int defaultValue = 0) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            return std::atoi(it->second.c_str());
        }
        return defaultValue;
    }
    
    float getFloat(const std::string& key, float defaultValue = 0.0f) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            return static_cast<float>(std::atof(it->second.c_str()));
        }
        return defaultValue;
    }
    
    bool getBool(const std::string& key, bool defaultValue = false) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            const std::string& val = it->second;
            return (val == "true" || val == "1" || val == "yes" || val == "on");
        }
        return defaultValue;
    }
    
    // Check if key exists
    bool hasKey(const std::string& key) const {
        return config_.find(key) != config_.end();
    }
    
    // Print all configuration
    void printAll() const {
        printf("[Config] Current configuration (%zu parameters):\n", config_.size());
        for (const auto& pair : config_) {
            printf("  %s = %s\n", pair.first.c_str(), pair.second.c_str());
        }
    }
    
    // Initialize with default simulation parameters
    void setDefaults() {
        // Performance settings
        setInt("target_fps", 60);
        setBool("enable_vsync", true);
        setBool("enable_lod", true);
        setBool("enable_frustum_culling", true);
        
        // LOD distances
        setFloat("vehicle_lod_high", 50.0f);
        setFloat("vehicle_lod_medium", 100.0f);
        setFloat("vehicle_lod_low", 200.0f);
        setFloat("pedestrian_lod_high", 30.0f);
        setFloat("pedestrian_lod_medium", 60.0f);
        setFloat("pedestrian_lod_low", 120.0f);
        
        // Simulation parameters
        setInt("max_vehicles", 100);
        setInt("max_pedestrians", 200);
        setFloat("vehicle_spawn_rate", 0.5f);
        setFloat("pedestrian_spawn_rate", 0.3f);
        
        // Graphics settings
        setBool("enable_lighting", true);
        setBool("enable_textures", true);
        setBool("enable_shadows", false);
        setBool("enable_fog", true);
        setInt("window_width", 1280);
        setInt("window_height", 720);
        
    // Camera settings
    setFloat("camera_speed", 20.0f);
    setFloat("camera_sensitivity", 0.002f);
    // Adjusted field of view and steeper top-down default
    setFloat("camera_fov", 65.0f);
    // Default camera clipping planes
    setFloat("camera_near_plane", 0.1f);
    setFloat("camera_far_plane", 500.0f);
    // Default start position / orientation (steeper top-down view)
    setFloat("camera_start_x", 0.0f);
    setFloat("camera_start_y", 180.0f);
    setFloat("camera_start_z", 220.0f);
    setFloat("camera_start_yaw", 0.0f);
    setFloat("camera_start_pitch", -60.0f);
        
        // Debug settings
        setBool("show_fps", true);
        setBool("show_stats", false);
        setInt("stats_update_interval", 60);
        
        printf("[Config] Initialized with default parameters\n");
    }
    
    // Clear all configuration
    void clear() {
        config_.clear();
    }
    
private:
    ConfigSystem() {}
    
    // Helper to trim whitespace
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }
    
    std::map<std::string, std::string> config_;
};

/**
 * @brief Helper macro to get config values with default
 */
#define CONFIG_INT(key, def) ConfigSystem::getInstance().getInt(key, def)
#define CONFIG_FLOAT(key, def) ConfigSystem::getInstance().getFloat(key, def)
#define CONFIG_BOOL(key, def) ConfigSystem::getInstance().getBool(key, def)
#define CONFIG_STR(key, def) ConfigSystem::getInstance().getString(key, def)

#endif // CONFIG_SYSTEM_H
