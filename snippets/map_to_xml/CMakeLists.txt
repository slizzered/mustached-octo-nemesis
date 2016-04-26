#-------------------------------------------------------------------------------
# Set name.
#-------------------------------------------------------------------------------
get_filename_component(FolderName ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" FolderName ${FolderName})

################################################################################
# Build.
################################################################################
file(GLOB_RECURSE _SOURCES "src" "*.cpp")
file(GLOB_RECURSE _HEADERS "src" "*.hpp")

find_package(Boost 1.60 REQUIRED COMPONENTS serialization)

# Always add all files to the target executable build call to add them to the build project.
ADD_EXECUTABLE(${FolderName} ${_SOURCES} ${_HEADERS})
TARGET_LINK_LIBRARIES(${FolderName} ${Boost_LIBRARIES})
