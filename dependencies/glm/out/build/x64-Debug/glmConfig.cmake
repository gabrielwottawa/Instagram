set(GLM_VERSION "0.9.8")
set(GLM_INCLUDE_DIRS "C:/Users/User/Documents/Visual Studio 2019/OpenGLProjects/PG2021-1/dependencies/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
