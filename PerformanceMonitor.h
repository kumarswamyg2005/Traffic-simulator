/**
 * @file PerformanceMonitor.h
 * @brief Real-time performance monitoring and FPS control
 * 
 * Tracks frame rates, memory usage, and provides adaptive quality control
 */

#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <chrono>
#include <deque>
#include <cstdio>

/**
 * @class PerformanceMonitor
 * @brief Monitors and controls simulation performance
 * 
 * Features:
 * - FPS tracking with rolling average
 * - Frame time measurement
 * - Adaptive quality suggestions
 * - Performance statistics
 */
class PerformanceMonitor {
public:
    static PerformanceMonitor& getInstance() {
        static PerformanceMonitor instance;
        return instance;
    }
    
    void frameStart() {
        frameStartTime_ = std::chrono::high_resolution_clock::now();
    }
    
    void frameEnd() {
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            frameEndTime - frameStartTime_).count();
        
        frameTimes_.push_back(duration);
        if (frameTimes_.size() > 60) {
            frameTimes_.pop_front();
        }
        
        frameCount_++;
        
        // Update statistics every second
        auto now = std::chrono::high_resolution_clock::now();
        auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastStatsUpdate_).count();
        
        if (timeSinceLastUpdate >= 1000) {
            updateStatistics();
            lastStatsUpdate_ = now;
        }
    }
    
    float getCurrentFPS() const { return currentFPS_; }
    float getAverageFrameTime() const { return avgFrameTimeMs_; }
    float getMinFPS() const { return minFPS_; }
    float getMaxFPS() const { return maxFPS_; }
    
    bool shouldReduceQuality() const {
        return currentFPS_ < 30.0f;
    }
    
    bool shouldIncreaseQuality() const {
        return currentFPS_ > 55.0f && avgFrameTimeMs_ < 14.0f;
    }
    
    void printStatistics() const {
        printf("[Performance] FPS: %.1f (min: %.1f, max: %.1f) | Frame: %.2fms\n",
               currentFPS_, minFPS_, maxFPS_, avgFrameTimeMs_);
    }
    
    void reset() {
        frameTimes_.clear();
        frameCount_ = 0;
        currentFPS_ = 60.0f;
        avgFrameTimeMs_ = 16.67f;
        minFPS_ = 60.0f;
        maxFPS_ = 60.0f;
    }
    
private:
    PerformanceMonitor() 
        : frameCount_(0)
        , currentFPS_(60.0f)
        , avgFrameTimeMs_(16.67f)
        , minFPS_(60.0f)
        , maxFPS_(60.0f)
    {
        lastStatsUpdate_ = std::chrono::high_resolution_clock::now();
    }
    
    void updateStatistics() {
        if (frameTimes_.empty()) return;
        
        // Calculate average frame time
        long long totalTime = 0;
        long long minTime = frameTimes_[0];
        long long maxTime = frameTimes_[0];
        
        for (long long time : frameTimes_) {
            totalTime += time;
            if (time < minTime) minTime = time;
            if (time > maxTime) maxTime = time;
        }
        
        avgFrameTimeMs_ = (totalTime / static_cast<float>(frameTimes_.size())) / 1000.0f;
        currentFPS_ = 1000000.0f / (totalTime / static_cast<float>(frameTimes_.size()));
        
        // Calculate min/max FPS from frame times
        if (maxTime > 0) minFPS_ = 1000000.0f / maxTime;
        if (minTime > 0) maxFPS_ = 1000000.0f / minTime;
    }
    
    std::chrono::high_resolution_clock::time_point frameStartTime_;
    std::chrono::high_resolution_clock::time_point lastStatsUpdate_;
    std::deque<long long> frameTimes_;
    
    long long frameCount_;
    float currentFPS_;
    float avgFrameTimeMs_;
    float minFPS_;
    float maxFPS_;
};

/**
 * @class FrameRateController
 * @brief Controls frame rate to target FPS
 */
class FrameRateController {
public:
    explicit FrameRateController(int targetFPS = 60) 
        : targetFPS_(targetFPS)
        , frameTime_(1000 / targetFPS)
    {}
    
    void setTargetFPS(int fps) {
        targetFPS_ = fps;
        frameTime_ = 1000 / fps;
    }
    
    void waitForNextFrame() {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastFrameTime_).count();
        
        if (elapsed < frameTime_) {
            // Simple busy-wait for better precision
            while (elapsed < frameTime_) {
                now = std::chrono::high_resolution_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - lastFrameTime_).count();
            }
        }
        
        lastFrameTime_ = now;
    }
    
private:
    int targetFPS_;
    int frameTime_;
    std::chrono::high_resolution_clock::time_point lastFrameTime_;
};

#endif // PERFORMANCE_MONITOR_H
