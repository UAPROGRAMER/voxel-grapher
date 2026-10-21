#pragma once

#include <exception>
#include <string>

class VoxelGrapherError : std::exception {
 public:
  std::string msg;

  VoxelGrapherError(std::string&& msg);

  VoxelGrapherError(const std::string& msg);

  const char* what() const noexcept override;
};