/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Core\DllInclude.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Platform/Platform.h"

#include "Framework/Types.h"
#include "Framework/Common.h"
#include "Framework/Log.h"
#include "Math/Vector.h"
#include "Framework/Window.h"
#include "Framework/Thread.h"
#include "Framework/DateTime.h"
#include "Framework/Rtti.h"
#include "Framework/String.h"
#include "Math/Rect.h"
#include "Framework/Resource.h"
#include "Framework/ResourceManager.h"
#include "Framework/Plugin.h"
#include "Framework/Window.h"
#include "Framework/Timer.h"
#include "Math/Matrix.h"
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