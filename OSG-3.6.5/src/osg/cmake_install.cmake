# Install script for directory: C:/Librerias/SUMOLibraries/OSG-3.6.5/src/osg

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Librerias/SUMOLibraries/OSG-3.6.5")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osgd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/lib/osg.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osg.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgrd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraphx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Librerias/SUMOLibraries/OSG-3.6.5/bin/osg161-osgd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibopenscenegraph-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osg" TYPE FILE FILES
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/io_utils"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/os_utils"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/AlphaFunc"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/AnimationPath"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ApplicationUsage"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ArgumentParser"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Array"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/AttributeDispatchers"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/AudioStream"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/AutoTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Billboard"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BindImageTexture"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BlendColor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BlendEquation"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BlendEquationi"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BlendFunc"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BlendFunci"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BoundingBox"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BoundingSphere"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BoundsChecking"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/buffered_value"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BufferIndexBinding"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BufferObject"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/BufferTemplate"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Callback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Camera"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CameraView"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Capability"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClampColor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClearNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClipControl"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClipNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClipPlane"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ClusterCullingCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CollectOccludersVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ColorMask"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ColorMaski"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ColorMatrix"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ComputeBoundsVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/DispatchCompute"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ContextData"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ConvexPlanarOccluder"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ConvexPlanarPolygon"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CoordinateSystemNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CopyOp"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CullFace"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CullingSet"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CullSettings"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/CullStack"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/DeleteHandler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Depth"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/DepthRangeIndexed"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/DisplaySettings"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Drawable"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/DrawPixels"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Endian"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Export"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/fast_back_stack"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Fog"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/FragmentProgram"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/FrameBufferObject"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/FrameStamp"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/FrontFace"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Geode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Geometry"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GL2Extensions"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GLDefines"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GLExtensions"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GLObjects"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GLU"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GraphicsCostEstimator"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GraphicsContext"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GraphicsThread"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Group"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Hint"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Identifier"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Image"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ImageSequence"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ImageStream"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ImageUtils"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/KdTree"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Light"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LightModel"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LightSource"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LineSegment"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LineStipple"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LineWidth"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LOD"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/LogicOp"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Material"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Math"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Matrix"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Matrixd"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Matrixf"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/MatrixTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/MixinVector"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Multisample"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Node"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/NodeCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/NodeTrackerCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/NodeVisitor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Notify"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Object"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/observer_ptr"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Observer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ObserverNodePath"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/OccluderNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/OcclusionQueryNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/OperationThread"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PatchParameter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PagedLOD"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Plane"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Point"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PointSprite"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PolygonMode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PolygonOffset"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PolygonStipple"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Polytope"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PositionAttitudeTransform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PrimitiveSet"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PrimitiveSetIndirect"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/PrimitiveRestartIndex"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Program"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Projection"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ProxyNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Quat"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Referenced"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ref_ptr"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/RenderInfo"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/SampleMaski"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Sampler"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Scissor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ScissorIndexed"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ScriptEngine"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Sequence"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ShadeModel"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Shader"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ShaderAttribute"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ShaderComposer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ShadowVolumeOccluder"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Shape"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ShapeDrawable"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/State"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/StateAttribute"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/StateAttributeCallback"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/StateSet"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Stats"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Stencil"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/StencilTwoSided"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Switch"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TemplatePrimitiveFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TemplatePrimitiveIndexFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexEnv"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexEnvCombine"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexEnvFilter"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexGen"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexGenNode"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TexMat"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture1D"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture2D"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture2DMultisample"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture2DArray"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Texture3D"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TextureBuffer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TextureCubeMap"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TextureRectangle"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Timer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TransferFunction"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Transform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TriangleFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TriangleIndexFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/TriangleLinePointIndexFunctor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Types"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Uniform"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/UserDataContainer"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ValueObject"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ValueMap"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ValueStack"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2b"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2d"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2f"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2i"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2s"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2ub"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2ui"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec2us"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3b"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3d"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3f"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3i"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3s"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3ub"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3ui"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec3us"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4b"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4d"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4f"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4i"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4s"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4ub"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4ui"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Vec4us"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/VertexAttribDivisor"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/VertexArrayState"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/VertexProgram"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/View"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Viewport"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/ViewportIndexed"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Version"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/Config"
    "C:/Librerias/SUMOLibraries/OSG-3.6.5/include/osg/GL"
    )
endif()

