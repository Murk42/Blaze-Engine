#pragma once
#include "BlazeEngine/Core/EngineCore.h"
#include "BlazeEngine/Memory/MallocAllocator.h"
#include "BlazeEngine/DataStructures/List.h"
#include "BlazeEngine/Core/Result.h"

namespace Blaze
{
	enum class MemoryEventType
	{
		Allocation,
		Deallocation
	};
	struct MemoryEvent
	{
		MemoryEventType type;
		void* ptr;
		size_t size;
	};

	class BLAZE_API MemoryListener
	{
	public:
		using ContainerType = List<MemoryEvent, MallocAllocator<ListNode<MemoryEvent>>>;
	private:
		bool listening;
		ContainerType events;
	public:
		MemoryListener();
		MemoryListener(const MemoryListener&) = delete;
		MemoryListener(MemoryListener&&) noexcept;
		~MemoryListener();

		void AddMemoryEvent(const MemoryEvent&);

		Result StartListening();
		Result StopListening();

		void ClearEvents();

		bool IsListening() const { return listening; }

		const ContainerType& GetEvents() const { return events; }

		MemoryListener& operator=(const MemoryListener&) = delete;
		MemoryListener& operator=(MemoryListener&&) noexcept;
	};
}