#include "utils/rate_limit_delegate.h"
#include <chrono>
#include <ratio>


RateLimitDelegate::RateLimitDelegate(std::chrono::duration<long long, std::micro> ttl): ttl_(ttl) {
    
}

void RateLimitDelegate::Update() {
    if (!update_delegate_) {
        return;
    }

    auto time_passed = clock_.now() - last_update_;
    if (time_passed < ttl_) {
        return;
    }

    update_delegate_();
    last_update_ = clock_.now();
}

void RateLimitDelegate::ForceUpdate() {
    if (!update_delegate_) {
        return;
    }
    update_delegate_();
}

void RateLimitDelegate::SetDelegate(std::function<void()> delegate) {
    this->update_delegate_ = delegate;
}

std::chrono::duration<long long, std::micro> RateLimitDelegate::GetTTL() const {
    return ttl_;
}

void RateLimitDelegate::SetTTL(std::chrono::duration<long long, std::micro> ttl){
    ttl_ = ttl;
}