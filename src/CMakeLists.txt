# Create an executable target and specify the source files
add_executable(source_files
    "main.cpp"
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET source_files PROPERTY CXX_STANDARD 20)
endif()