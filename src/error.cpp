#include "error.hpp"

VoxelGrapherError::VoxelGrapherError(std::string&& msg) : msg(std::move(msg)) {}

VoxelGrapherError::VoxelGrapherError(const std::string& msg) : msg(msg) {}

const char* VoxelGrapherError::what() const noexcept { return msg.c_str(); }