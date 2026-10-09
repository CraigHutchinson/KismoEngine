/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Core\DllInclude.cpp
 * Status: superseded by kismo-v0.3/Core/DllInclude.cpp (same name, content differs)
 */
#include "Platform/Platform.h"

#include "Framework/Types.h"
#include "Framework/Common.h"
#include "Framework/Log.h"
#include "Framework/Vector.h"
#include "Framework/Window.h"
#include "Framework/Thread.h"
#include "Framework/DateTime.h"
#include "Framework/Rtti.h"
#include "Framework/String.h"
#include "Framework/Rect.h"
#include "Framework/Resource.h"
#include "Framework/ResourceManager.h"
#include "Framework/Plugin.h"
#include "Framework/Window.h"
#include "Framework/Timer.h"
#include "Framework/Matrix.h"
#include "Framework/Profiler.h"

#include "Io/DirectoryManager.h"
#include "Io/FileParser.h"

#include "gfx/Renderer.h"
#include "gfx/RenderTarget.h"
#include "gfx/WindowTarget.h"
#include "gfx/Camera.h"
#include "gfx/MeshTranslator.h"
#include "gfx/SceneTranslator.h"
#include "gfx/MeshManager.h"
#include "gfx/Buffer.h"
#include "gfx/BufferDefinition.h"
#include "gfx/BufferElement.h"
#include "gfx/Mesh.h"
#include "gfx/Node.h"
#include "gfx/Light.h"

#include "phys/Physics.h"
#include "phys/Scene.h"
#include "phys/Actor.h"
#include "phys/Body.h"
#include "phys/Shape.h"