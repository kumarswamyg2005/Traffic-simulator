/**
 * @file LODSystem.h
 * @brief Level of Detail management for performance optimization
 * 
 * Implements distance-based LOD for vehicles and pedestrians to reduce
 * rendering overhead for distant objects
 */

#ifndef LOD_SYSTEM_H
#define LOD_SYSTEM_H

#include <cmath>
#include <cstdio>

/**
 * @enum LODLevel
 * @brief Available levels of detail
 */
enum class LODLevel {
    HIGH,    // Full detail - close to camera
    MEDIUM,  // Reduced detail - medium distance
    LOW,     // Minimal detail - far from camera
    CULLED   // Too far to render
};

/**
 * @struct LODSettings
 * @brief Configuration for LOD distance thresholds
 */
struct LODSettings {
    float highDistance;    // Distance for high detail (0 - this)
    float mediumDistance;  // Distance for medium detail (highDistance - this)
    float lowDistance;     // Distance for low detail (mediumDistance - this)
    float cullDistance;    // Objects beyond this are not rendered
    
    LODSettings() 
        : highDistance(50.0f)
        , mediumDistance(100.0f)
        , lowDistance(200.0f)
        , cullDistance(300.0f)
    {}
    
    LODSettings(float high, float medium, float low, float cull)
        : highDistance(high)
        , mediumDistance(medium)
        , lowDistance(low)
        , cullDistance(cull)
    {}
};

/**
 * @class LODSystem
 * @brief Manages level of detail for all objects
 */
class LODSystem {
public:
    static LODSystem& getInstance() {
        static LODSystem instance;
        return instance;
    }
    
    // Configure LOD distances for different object types
    void setVehicleLODSettings(const LODSettings& settings) {
        vehicleSettings_ = settings;
    }
    
    void setPedestrianLODSettings(const LODSettings& settings) {
        pedestrianSettings_ = settings;
    }
    
    void setBuildingLODSettings(const LODSettings& settings) {
        buildingSettings_ = settings;
    }
    
    // Get current LOD level based on distance
    LODLevel getVehicleLOD(float distanceSquared) const {
        return calculateLOD(distanceSquared, vehicleSettings_);
    }
    
    LODLevel getPedestrianLOD(float distanceSquared) const {
        return calculateLOD(distanceSquared, pedestrianSettings_);
    }
    
    LODLevel getBuildingLOD(float distanceSquared) const {
        return calculateLOD(distanceSquared, buildingSettings_);
    }
    
    // Helper to calculate distance squared (avoids sqrt)
    static float distanceSquared(float x1, float y1, float z1,
                                 float x2, float y2, float z2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        float dz = z2 - z1;
        return dx*dx + dy*dy + dz*dz;
    }
    
    // Get statistics
    void resetStats() {
        highCount_ = mediumCount_ = lowCount_ = culledCount_ = 0;
    }
    
    void recordLOD(LODLevel level) {
        switch (level) {
            case LODLevel::HIGH:   highCount_++; break;
            case LODLevel::MEDIUM: mediumCount_++; break;
            case LODLevel::LOW:    lowCount_++; break;
            case LODLevel::CULLED: culledCount_++; break;
        }
    }
    
    void printStats() const {
        int total = highCount_ + mediumCount_ + lowCount_ + culledCount_;
        if (total == 0) return;
        
        printf("[LOD] High: %d (%.1f%%) | Med: %d (%.1f%%) | Low: %d (%.1f%%) | Culled: %d (%.1f%%)\n",
               highCount_, 100.0f * highCount_ / total,
               mediumCount_, 100.0f * mediumCount_ / total,
               lowCount_, 100.0f * lowCount_ / total,
               culledCount_, 100.0f * culledCount_ / total);
    }
    
    // Adaptive LOD based on performance
    void adjustForPerformance(float currentFPS, float targetFPS = 60.0f) {
        if (currentFPS < targetFPS * 0.8f) {
            // Performance is poor, reduce LOD distances
            vehicleSettings_.highDistance *= 0.9f;
            vehicleSettings_.mediumDistance *= 0.9f;
            pedestrianSettings_.highDistance *= 0.9f;
            pedestrianSettings_.mediumDistance *= 0.9f;
        } else if (currentFPS > targetFPS * 0.95f) {
            // Performance is good, increase LOD distances
            vehicleSettings_.highDistance *= 1.05f;
            vehicleSettings_.mediumDistance *= 1.05f;
            pedestrianSettings_.highDistance *= 1.05f;
            pedestrianSettings_.mediumDistance *= 1.05f;
            
            // Cap at reasonable limits
            if (vehicleSettings_.highDistance > 80.0f) vehicleSettings_.highDistance = 80.0f;
            if (vehicleSettings_.mediumDistance > 150.0f) vehicleSettings_.mediumDistance = 150.0f;
            if (pedestrianSettings_.highDistance > 60.0f) pedestrianSettings_.highDistance = 60.0f;
            if (pedestrianSettings_.mediumDistance > 120.0f) pedestrianSettings_.mediumDistance = 120.0f;
        }
    }
    
private:
    LODSystem() {
        // Default vehicle LOD: high detail up to 50m, medium to 100m, low to 200m
        vehicleSettings_ = LODSettings(50.0f, 100.0f, 200.0f, 300.0f);
        
        // Pedestrians need less detail at distance
        pedestrianSettings_ = LODSettings(30.0f, 60.0f, 120.0f, 200.0f);
        
        // Buildings can maintain detail longer
        buildingSettings_ = LODSettings(100.0f, 200.0f, 400.0f, 600.0f);
    }
    
    LODLevel calculateLOD(float distSquared, const LODSettings& settings) const {
        // Compare squared distances to avoid sqrt
        float highDistSq = settings.highDistance * settings.highDistance;
        float mediumDistSq = settings.mediumDistance * settings.mediumDistance;
        float lowDistSq = settings.lowDistance * settings.lowDistance;
        float cullDistSq = settings.cullDistance * settings.cullDistance;
        
        if (distSquared < highDistSq) return LODLevel::HIGH;
        if (distSquared < mediumDistSq) return LODLevel::MEDIUM;
        if (distSquared < lowDistSq) return LODLevel::LOW;
        if (distSquared < cullDistSq) return LODLevel::LOW;  // Still render at lowest detail
        return LODLevel::CULLED;
    }
    
    LODSettings vehicleSettings_;
    LODSettings pedestrianSettings_;
    LODSettings buildingSettings_;
    
    // Statistics
    mutable int highCount_;
    mutable int mediumCount_;
    mutable int lowCount_;
    mutable int culledCount_;
};

/**
 * @class FrustumCuller
 * @brief Simple frustum culling for off-screen objects
 */
class FrustumCuller {
public:
    void updateViewFrustum(float camX, float camY, float camZ,
                          float viewDirX, float viewDirY, float viewDirZ,
                          float fov, float aspect, float nearPlane, float farPlane) {
        camX_ = camX;
        camY_ = camY;
        camZ_ = camZ;
        viewDirX_ = viewDirX;
        viewDirY_ = viewDirY;
        viewDirZ_ = viewDirZ;
        fov_ = fov;
        aspect_ = aspect;
        nearPlane_ = nearPlane;
        farPlane_ = farPlane;
        
        // Simple sphere-based culling radius
        cullingRadius_ = farPlane * 1.5f;
    }
    
    bool isVisible(float x, float y, float z, float radius = 5.0f) const {
        // Simple distance-based culling
        float dx = x - camX_;
        float dy = y - camY_;
        float dz = z - camZ_;
        float distSquared = dx*dx + dy*dy + dz*dz;
        
        float maxDist = cullingRadius_ + radius;
        return distSquared < (maxDist * maxDist);
    }
    
    void resetStats() {
        visibleCount_ = 0;
        culledCount_ = 0;
    }
    
    void recordVisible() { visibleCount_++; }
    void recordCulled() { culledCount_++; }
    
    void printStats() const {
        int total = visibleCount_ + culledCount_;
        if (total == 0) return;
        
        printf("[Culling] Visible: %d (%.1f%%) | Culled: %d (%.1f%%)\n",
               visibleCount_, 100.0f * visibleCount_ / total,
               culledCount_, 100.0f * culledCount_ / total);
    }
    
private:
    float camX_, camY_, camZ_;
    float viewDirX_, viewDirY_, viewDirZ_;
    float fov_, aspect_, nearPlane_, farPlane_;
    float cullingRadius_;
    
    mutable int visibleCount_;
    mutable int culledCount_;
};

#endif // LOD_SYSTEM_H
