# CMake编写的部分规则

首先安装cmake，并添加到环境变量中，然后就可以开始使用了。

调用方式:

```sh
mkdir build   #新建build文件夹
cd build      #进入到该文件下
cmake ../src  #编译CMakeList.txt所在目录，编译文件会生成在当前目录下，即build目录下
```

`cmake` 可以接受部分参数，也可以自定义一些参数，在命令行下输入`cmake --help`可查看，部分参数如下：

```sh
-G "编译平台"
    -G "Visual Studio 9 2008"
    -G "Visual Studio 12 2013 Win64"
    -G "MinGW Makefiles"
-D 定义变量或者设置值
    -DENABLE_NORMAL_UNITTEST:BOOL=OFF
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DCMAKE_INSTALL_PREFIX:PATH=e:\dist
```

下面是一个可用的`src/CMakeLists.txt`脚本解释：

```cmake
# 至少要求 cmake 3.14 及以上
cmake_mininum_required(VERSION 3.14)

# 项目名称
project(test C CXX)

# 添加自定义宏
add_definitions(-DTEST_STATIC_LIB)

# Debug 版本库添加后缀 d
set(CMAKE_DEBUG_POSTFIX "d")

# 统一输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

# VS 中默认是 /MDd 的，如果需要可设置 /MTd
if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG "CMAKE_C_FLAGS_DEBUG /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "CMAKE_CXX_FLAGS_DEBUG /MTd")
endif()

# 在C/C++ 附加包含目录中添加目录
set(CMAKE_INCLUDE_CURRENT_DIR ON) #cmake时的目录
include_directories(${PROJECT_SOURCE_DIR}) #当前CMakeLists的目录

# 判断是32位还是64位
if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(ARCH "32bit")
else()
    set(ARCH "64bit")
endif()

#启用文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    # 禁用UTF-8警告，无返回值视为错误
    add_compile_options(/wd4819 /wd4828 /we4715 /we4716 /MP /Zi)
    # 设置头文件路径
    set(THIRDPARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/3rdparty/vs/include)
    # 不同版本可以加载不同的库文件
    if (MSVC_VERSION EQUAL 1500)
        set(THIRDPARTY_LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/vs/2008/lib)
    elseif (MSVC10)
        set(THIRDPARTY_LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/vs/2010/lib)
    elseif (MSVC12)
        set(THIRDPARTY_LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/vs/2013/lib)
    else()
        message(FATAL_ERROR "\n unsupport vc verison: ${MSVC_VERSION}")
    endif()
elseif (MINGW)
    set(THIRDPARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/3rdparty/qt/include)
    set(THIRDPARTY_LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/qt/lib)
elseif (UNIX)
    set(THIRDPARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/3rdparty/linux/include)
    set(THIRDPARTY_LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/linux/lib)
else()
    message(FATAL_ERROR "\n build tool not support!")
endif()

# debug 版本库后面要加 d
set(THIRDPARTY_LIBS 3rdlib$<$<CONFIG:Debug>:d>)

if (WIN32) # windows 下要加网络库
    set(THIRDPARTY_LIBS ${THIRDPARTY_LIBS} Ws2_32 iphlpapi Winnm)
elseif (UNIX)
    set(THIRDPARTY_LIBS ${THIRDPARTY_LIBS} pthread)
endif()

# 类似C++的include，加入cmake的查找目录
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# 引入头文件 tools.cmake，tools.cmake 见下文
include(tools)

# 判断是否编译动态库
if (BUILD_SHARED_LIBS)
    add_definitions(-DTEST_LIBRARY)
else()
    add_definitions(-DTEST_STATIC_LIB)
endif()

#这里有两种方案，每个文件生成一份，或者查找所有.cpp文件放在一个项目中
if ()
add_subdirectory(common) # 生成common_objs
add_subdirectory(base) # 生成 base_objs

# 编译生成一个大 lib
add_library(${PROJECT_NAME} ${TARGET_OBJECTS:common_objs}
                            ${TARGET_OBJECTS:base_objs})
else()
file(GLOB_RECURSE ALL_SRCS *.h *.hpp *.c *.cpp *.cxx)
add_library(${PROJECT_NAME} ${ALL_SRCS})
endif()

TargetUser3rd(${PROJECT_NAME})

# 下面install会生成 INSTALL 项目
# 安装位置由开始定义的 CMAKE_INSTALL_PREFIX 决定

#拷贝所有头文件
file(GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}\*.h")
install(FILES ${LIB_HEADERS} DESTINATION include/test)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

# 打开自动测试功能
enable_testing()

# 测试入口（可不需要这样）
set(UNITTEST_MAIN "${PROJECT_SOURCE_DIR}/test/test_main.cpp")

# 生成单元测试
if (ENABLE_NORMAL_UNITTEST)
    aux_source_directory(./test UNITTEST_DIR)
    add_excutable(test_unittest ${UNITTEST_MAIN} ${UNITTEST_DIR})
    # 单元测试使用 gmock 和上面生成 test 库
    target_link_libraries(test_unittest PRIVATE test gmock)
    # 这里如果需要头文件和链接库目录的话还需要加上哦
    include_directories(${THIRDPARTY_INCLUDE_DIR})
    target_link_directories(test_unittest PRIVATE ${THIRDPARTY_LIBS_DIR}
    install(TARGETS test_unittest DESTINATION bin)
    add_test(test_test test_unittest)
endif()

```

`src/cmake/tool.cmake`文件内容如下：

```cmake
# 在Target中使用第三方库
macro(TargetUser3rd TargetName)
    target_include_deirectories(${TargetName} PRIVATE ${THIRDPARTY_INCLUDE_DIR})
    target_link_directories(${TargetName} PRIVATE ${THIRDPARTY_LIBS_DIR}
    target_link_libraries(${TargetName} PRIVATE ${THIRDPARTY_LIBS})
endmacro()
```

以上，即可以构建一个VS或者QT项目，构建完成后可编译打包，以下为命令行命令：

```cmake
# msbuild 一般需要执行 vcvarsall.bat 脚本设置环境变量才能使用
msbuild test.sln /t:Rebuild /p:Configuration=Debug(Release)

# INSTALL 和 RUN_TESTS 为cmake自动生成的
# install() 对应生成 INSTALL，enable_test() 对应会有 RUN_TESTS 项目，一个是编译完后安装脚本，一个是执行单元测试内容
msbuild INSTALL.vcxproj /t:Build /p:Configuration=Debug(Release)
msbuild RUN_TESTS.vcxproj /t:Build /pConfiguration=Debug(Release)
```
