#pragma once

#include <chrono>
#include <functional>
#include <ratio>

class RateLimitDelegate {
public:
    RateLimitDelegate(std::chrono::duration<long long, std::micro> ttl);
    RateLimitDelegate(const RateLimitDelegate&) = delete;
    void Update();
    void ForceUpdate();
    void SetDelegate(std::function<void()> delegate);
    void SetTTL(std::chrono::duration<long long, std::micro> ttl);
    std::chrono::duration<long long, std::micro> GetTTL() const;
private:
    std::chrono::duration<long long, std::micro> ttl_{std::chrono::milliseconds(1)};
    std::function<void()> update_delegate_;
    std::chrono::high_resolution_clock clock_{};
    std::chrono::high_resolution_clock::time_point last_update_{};
};