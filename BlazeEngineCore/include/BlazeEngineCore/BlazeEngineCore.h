#pragma once
#define BLAZE_ENGINE_CORE

#pragma region PCH
#include <concepts>
#include <xutility>
#include <xmemory>
#include <array>
#include <functional>

#include "BlazeEngineCore/BlazeEngineCoreDefines.h"

#include "BlazeEngineCore/Memory/Allocator.h"

#include "BlazeEngineCore/DataStructures/String.h"
#include "BlazeEngineCore/DataStructures/StringView.h"

#include "BlazeEngineCore/Utilities/Time.h"

#include "BlazeEngineCore/File/Path.h"

#include "BlazeEngineCore/Debug/Callstack.h"
#include "BlazeEngineCore/Debug/Log.h"
#include "BlazeEngineCore/Debug/Result.h"
#include "BlazeEngineCore/Debug/Logger.h"

#include "BlazeEngineCore/DataStructures/Hash.h"
#include "BlazeEngineCore/DataStructures/List.h"
#include "BlazeEngineCore/DataStructures/ListImpl.h"
#include "BlazeEngineCore/DataStructures/Map.h"
#include "BlazeEngineCore/DataStructures/MapImpl.h"

#pragma endregion

#include "BlazeEngineCore/BlazeEngineCoreDefines.h"

#include "BlazeEngineCore/DataStructures/Array.h"
#include "BlazeEngineCore/DataStructures/ArrayImpl.h"
#include "BlazeEngineCore/DataStructures/ArrayView.h"
#include "BlazeEngineCore/DataStructures/Buffer.h"
#include "BlazeEngineCore/DataStructures/Color.h"
#include "BlazeEngineCore/DataStructures/Constexpr.h"
#include "BlazeEngineCore/DataStructures/DualList.h"
#include "BlazeEngineCore/DataStructures/DualListImpl.h"
#include "BlazeEngineCore/DataStructures/Handle.h"
#include "BlazeEngineCore/DataStructures/HandleImpl.h"
#include "BlazeEngineCore/DataStructures/Hash.h"
#include "BlazeEngineCore/DataStructures/HybridArray.h"
#include "BlazeEngineCore/DataStructures/List.h"
#include "BlazeEngineCore/DataStructures/ListImpl.h"
#include "BlazeEngineCore/DataStructures/Map.h"
#include "BlazeEngineCore/DataStructures/MapImpl.h"
#include "BlazeEngineCore/DataStructures/Rect.h"
#include "BlazeEngineCore/DataStructures/Set.h"
#include "BlazeEngineCore/DataStructures/SetImpl.h"
#include "BlazeEngineCore/DataStructures/String.h"
#include "BlazeEngineCore/DataStructures/StringHash.h"
#include "BlazeEngineCore/DataStructures/StringUTF8.h"
#include "BlazeEngineCore/DataStructures/StringView.h"
#include "BlazeEngineCore/DataStructures/StringViewUTF8.h"
#include "BlazeEngineCore/DataStructures/Template.h"
#include "BlazeEngineCore/DataStructures/TemplateGroup.h"
#include "BlazeEngineCore/DataStructures/Tuple.h"
#include "BlazeEngineCore/DataStructures/UnicodeChar.h"
#include "BlazeEngineCore/DataStructures/UnorderedArray.h"
#include "BlazeEngineCore/DataStructures/UnorderedArrayImpl.h"
#include "BlazeEngineCore/DataStructures/VirtualList.h"
#include "BlazeEngineCore/DataStructures/VirtualListImpl.h"
#include "BlazeEngineCore/DataStructures/VirtualMap.h"
#include "BlazeEngineCore/DataStructures/VirtualMapImpl.h"

#include "BlazeEngineCore/Debug/Breakpoint.h"
#include "BlazeEngineCore/Debug/LoggerListener.h"

#include "BlazeEngineCore/File/Path.h"
#include "BlazeEngineCore/File/File.h"
#include "BlazeEngineCore/File/FileSystem.h"
#include "BlazeEngineCore/File/Stream/BufferStream.h"
#include "BlazeEngineCore/File/Stream/FileStream.h"
#include "BlazeEngineCore/File/Stream/Stream.h"
#include "BlazeEngineCore/File/Stream/SubStream.h"

#include "BlazeEngineCore/Math/Quaternion.h"
#include "BlazeEngineCore/Math/Matrix.h"
#include "BlazeEngineCore/Math/Math.h"
#include "BlazeEngineCore/Math/Random.h"
#include "BlazeEngineCore/Math/Shapes.h"

#include "BlazeEngineCore/Utilities/Stopwatch.h"
#include "BlazeEngineCore/Utilities/Timing.h"
#include "BlazeEngineCore/Utilities/StringParsing.h"

#include "BlazeEngineCore/Threading/Thread.h"

#include "BlazeEngineCore/Memory/Creator.h"
#include "BlazeEngineCore/Memory/Allocator.h"
#include "BlazeEngineCore/Memory/MallocAllocator.h"
#include "BlazeEngineCore/Memory/MemoryListener.h"
#include "BlazeEngineCore/Memory/MemoryManager.h"
#include "BlazeEngineCore/Memory/VirtualAllocator.h"