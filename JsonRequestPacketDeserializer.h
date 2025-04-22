#pragma once

#include <vector>
#include <chrono>
#include "Structs.h"
#include "json.hpp"

class JsonRequestPacketDeserializer
{
public:
    static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
    static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
};