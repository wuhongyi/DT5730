#CMakeLists.txt --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 日 4月 24 15:24:50 2016 (+0800)
# Last-Updated: 五 1月 18 15:53:40 2019 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 39
# URL: http://wuhongyi.cn 

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#SET(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#enable_language(Fortran)
set(PROJ_NAME pkuDigitizer)
project(${PROJ_NAME})

#SET(CMAKE_BUILD_TYPE Debug)

##C99  ROOT不支持C++11采用以下两行
# set(CMAKE_CXX_FLAGS " -fPIC -W -Wall -s")#
# set(CMAKE_C_FLAGS " -fPIC -W -Wall -s")#
##C++11  ROOT支持C++11采用以下两行
set(CMAKE_CXX_FLAGS "-std=c++11  -fPIC -W -Wall -s")#
set(CMAKE_C_FLAGS "-std=c++11  -fPIC   -W -Wall -s")#

#----------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#find_package(ROOT REQUIRED)
#COMPONENTS可选 ，REQUIRED COMPONENTS必须有
find_package(ROOT COMPONENTS FFTW Gui GuiBld Hist MathMore Minuit Spectrum)#ASImage EG FFTW FitPanel Ged Geom Gui GuiBld GuiHtml GX11 GX11TTF Hbook Hist Html MathMore Minuit Minuit2 MLP Proof RGL RooFit RooFitCore Smatrix Spectrum Table TMVA TreePlayer TreeViewer 

#----------------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/include  ${ROOT_INCLUDE_DIRS})#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*)
#file(GLOB libs ${PROJECT_SOURCE_DIR}/lib/*)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#----------------------------------------------------------------------------

add_executable(${PROJ_NAME} main.cc ${sources} ${headers})
target_link_libraries(${PROJ_NAME}  -pthread CAENDigitizer ${ROOT_LIBRARIES}  m dl -rdynamic ncurses c) #${libs}  

#----------------------------------------------------------------------------

set(CopyFile
    BoardParameters.txt
    GlobalParameters.txt
  )

foreach(copyfile ${CopyFile})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${copyfile}
    ${PROJECT_SOURCE_DIR}/bin/${copyfile}
    COPYONLY
    )
endforeach()

###################################
