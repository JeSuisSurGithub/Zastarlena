set_xmakever("2.2.5")

set_project("YuZhou")
set_version("1.2.0")
set_languages("cxx17")

add_requires("opengl", "glfw", "glslang", {configs = {binaryonly = true}})

-- Source files
target("yuzhou")
    set_kind("binary")
    add_files(
        "src/glad.c",
        "src/main.cpp",
        "src/yzcontrol.cpp",
        "src/yzctx.cpp",
        "src/yzframebuffer.cpp",
        "src/yzmodel.cpp",
        "src/yzrendergroup.cpp",
        "src/rendergroups/maingroup.cpp",
        "src/rendergroups/no_light.cpp",
        "src/yzshader.cpp",
        "src/yztexture.cpp",
        "src/yzwindow.cpp")

    -- Libraries
    if is_host("windows") then
        add_links("opengl32")
    else
        add_links("GL")
    end
    add_includedirs("include")

    -- MSYS2 calls it glfw3
    if (is_subhost("msys", "cygwin")) then
        add_links("glfw3")
    else
        add_links("glfw")
    end

    -- Compiler options
    set_warnings("everything")
    if is_mode("release") then
        add_cxxflags("-O3", "-DNDEBUG")
    elseif is_mode("debug") then
        add_cxxflags("-Og", "-ggdb")
    end

    -- Shaders
    -- add_rules("utils.glsl2spv", {outputdir = "build/shaders", targetenv = "opengl"})
    -- add_files(
    --     "shaders/blur.vert",
    --     "shaders/blur.frag",
    --     "shaders/combine.vert",
    --     "shaders/combine.frag",
    --     "shaders/main.vert",
    --     "shaders/main.frag",
    --     "shaders/no_lighting.vert",
    --     "shaders/no_lighting.frag")

    -- Assets
    after_build(function (target)
        os.cp(target:targetfile(), "build")
        os.cp("models", "build")
        os.cp("textures", "build")
        os.cp("shaders", "build")
    end)
target_end()