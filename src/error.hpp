#pragma once

#include <string>
#include <exception>

class VoxelGrapherError : std::exception {
public:
  std::string msg;

  VoxelGrapherError(std::string&& msg);

  VoxelGrapherError(const std::string& msg);

  const char* what() const noexcept override;
};