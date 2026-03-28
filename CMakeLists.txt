cmake_minimum_required(VERSION 3.10)

# Nome do seu Plugin
project(FC_B734_Clipboard)

# 1. Definições globais
set(CMAKE_CXX_STANDARD 11)
add_definitions(-DXPLM200 -DXPLM210 -DXPLM300 -DXPLM301)

# 2. Localização da SDK (Ajuste o caminho se necessário)
set(XPLANE_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDK")

# 3. Incluir Headers
include_directories(${XPLANE_SDK_DIR}/CHeaders/XPLM)

# 4. Localizar os arquivos de código
file(GLOB SOURCE_FILES "src/*.cpp")

# 5. Criar a Biblioteca (Plugin)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Configurações específicas por Sistema Operacional
if(WIN32)
    # No Windows, precisamos linkar com a XPLM_64.lib
    target_link_libraries(${PROJECT_NAME} "${XPLANE_SDK_DIR}/Libraries/Win/XPLM_64.lib")
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".xpl")
    
elseif(APPLE)
    # No Mac, o plugin é um "Bundle"
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        BUNDLE TRUE
        BUNDLE_EXTENSION "xpl"
        XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "YES"
        XCODE_ATTRIBUTE_STRUCT_ALIGNMENT "YES"
    )
    # No Mac não linkamos com .lib, o X-Plane resolve os símbolos ao carregar
    target_link_options(${PROJECT_NAME} PRIVATE "-undefined dynamic_lookup")
endif()