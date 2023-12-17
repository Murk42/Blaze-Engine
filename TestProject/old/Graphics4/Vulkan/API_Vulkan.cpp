#include "pch.h"
#define VMA_IMPLEMENTATION
#include "API_Vulkan.h"
#include "VulkanInstance.h"
#include <utility>

namespace Graphics3::Vulkan
{
	std::unique_ptr<VulkanInstance> vulkanInstance;

	struct QueueFamilyIndices {		
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.Ptr());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{			
			if (surface != VK_NULL_HANDLE)
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

				if (presentSupport) {
					indices.presentFamily = i;					
				}
			}
						
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				indices.graphicsFamily = i;				
			

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	static bool CheckDeviceExtensionSupport(VkPhysicalDevice device, Set<String> deviceExtensions)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		Array<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.Ptr());

		Set<StringView> requiredExtensions;

		for (auto& ext : deviceExtensions)
			requiredExtensions.Insert((StringView)ext);

		for (const auto& extension : availableExtensions)
			requiredExtensions.Erase(extension.extensionName);		

		return requiredExtensions.Empty();
	}	

	BlazeVMAData::BlazeVMAData(GraphicsContext_Vulkan& graphicsContext)
	{
		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

		VmaAllocatorCreateInfo createInfo{ };
		createInfo.device = graphicsContext.GetDevice();
		createInfo.physicalDevice = graphicsContext.GetPhysicalDevice();
		createInfo.instance = vulkanInstance->GetHandle();
		createInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		createInfo.flags = 0;
		createInfo.pVulkanFunctions = &vulkanFunctions;

		VkResult result = vmaCreateAllocator(&createInfo, &allocator);
		if (result != VK_SUCCESS)
			Debug::Logger::LogFatal("Blaze graphics API", "Failed to create a VMA allocator");
	}
	BlazeVMAData::BlazeVMAData(BlazeVMAData&& other) noexcept		
	{
		allocator = other.allocator;
		other.allocator = VMA_NULL;
	}
	BlazeVMAData::~BlazeVMAData()
	{
		Clear();
	}

	void BlazeVMAData::Clear()
	{
		vmaDestroyAllocator(allocator);
		allocator = VMA_NULL;
	}

	BlazeVMAData& BlazeVMAData::operator=(BlazeVMAData&& other) noexcept
	{
		Clear();

		allocator = other.allocator;
		other.allocator = VMA_NULL;

		return *this;
	}

	BlazeVMAData* GetBlazeVMAData(GraphicsContext_Vulkan& graphicsContext)
	{
		return graphicsContext.dataMap.Find("BlazeVMAData").GetValue<BlazeVMAData>();			
	}

	GraphicsContext_Vulkan::GraphicsContext_Vulkan(ImplementationInfo* info)
		: GraphicsContext_Vulkan(Set<String>())
	{
	}

	GraphicsContext_Vulkan::GraphicsContext_Vulkan(Set<String>&& extensions)
		: device(NULL), physicalDevice(NULL), graphicsQueueFamily(-1), presentQueueFamily(-1), initSurface(NULL), commandPool(VK_NULL_HANDLE), graphicsQueue(VK_NULL_HANDLE)
	{			
		if (vulkanInstance == nullptr)		
			vulkanInstance = std::make_unique<VulkanInstance>();				
		
		initWindow = vulkanInstance->CreateWindow("", Vec2i(INT_MAX), Vec2i(0, 0));

		if (SDL_Vulkan_CreateSurface((SDL_Window*)initWindow.GetHandle(), vulkanInstance->GetHandle(), &initSurface) == SDL_FALSE)
			Debug::Logger::LogFatal("Blaze Engine", "SDL failed to create vulkan surface: \"" + (String)SDL_GetError() + "\"\n");		    		

		//Choosing a physical device
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstance->GetHandle(), &deviceCount, nullptr);

		if (deviceCount == 0)		
			Debug::Logger::LogFatal("Blaze Engine", "Failed to find GPUs with Vulkan support!");					

		Array<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance->GetHandle(), &deviceCount, devices.Ptr());

		extensions.Insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);		

		QueueFamilyIndices indices;
		for (const auto& device : devices)
		{
			indices = FindQueueFamilies(device, initSurface);

			if (!indices.isComplete())
				continue;

			if (!CheckDeviceExtensionSupport(device, extensions))
				continue;

			physicalDevice = device;
			break;
		}

		if (physicalDevice == VK_NULL_HANDLE)
		{
			Debug::Logger::LogFatal("Blaze Engine", "failed to find a suitable GPU!\n");
			return;
		}

		enabledExtensions = std::move(extensions);

		graphicsQueueFamily = indices.graphicsFamily.value();
		presentQueueFamily = indices.presentFamily.value();

		//Creating a logical device    
		Array<VkDeviceQueueCreateInfo> queueCreateInfos;
		Set<uint32_t> uniqueQueueFamilies = { graphicsQueueFamily, presentQueueFamily };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.AddBack(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		Array<const char*> _enabledExtensions { enabledExtensions.Count() };

		{
			auto src = enabledExtensions.FirstIterator();
			auto dst = _enabledExtensions.FirstIterator();
			auto end = enabledExtensions.BehindIterator();

			while (src != end)
			{
				*dst = src->Ptr();
				++src;
				++dst;
			}
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.Ptr();
		createInfo.queueCreateInfoCount = queueCreateInfos.Count();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = _enabledExtensions.Count();
		createInfo.ppEnabledExtensionNames = _enabledExtensions.Ptr();
		createInfo.enabledLayerCount = 0;
#ifdef BLAZE_VULKAN_VALIDATION_LAYERS
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.Count());
		createInfo.ppEnabledLayerNames = validationLayers.Ptr();
#endif        

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
			Debug::Logger::LogFatal("Blaze Engine", "failed to create logical device!\n");		

		dataMap.Insert("BlazeVMAData", BlazeVMAData(*this));

		vkGetDeviceQueue(device, graphicsQueueFamily, 0, &graphicsQueue);
		//Creating command pool
		{
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = 0;
			poolInfo.queueFamilyIndex = graphicsQueueFamily;

			if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
				Debug::Logger::LogError("BlazeEngine", "Failed to create command pool!\n");
		}
	}	
	GraphicsContext_Vulkan::GraphicsContext_Vulkan(GraphicsContext_Vulkan&& other) noexcept
	{
		device = other.device;
		physicalDevice = other.physicalDevice;
		graphicsQueueFamily = other.graphicsQueueFamily;
		presentQueueFamily = other.presentQueueFamily;
		initWindow = std::move(other.initWindow);		
		initSurface = other.initSurface;		
		commandPool = other.commandPool;
		graphicsQueue = other.graphicsQueue;
		dataMap = std::move(other.dataMap);

		other.device = NULL;
		other.physicalDevice = NULL;
		other.graphicsQueueFamily = -1;
		other.presentQueueFamily = -1;		
		other.initSurface = NULL;		
		other.commandPool = VK_NULL_HANDLE;
		other.graphicsQueue = VK_NULL_HANDLE;
	}

	GraphicsContext_Vulkan::~GraphicsContext_Vulkan()
	{
		Destroy();
	}

	void GraphicsContext_Vulkan::Destroy()
	{

		if (device == NULL)
			return;		

		dataMap.Clear();

		vkDestroyCommandPool(device, commandPool, nullptr);

		vkDestroyDevice(device, nullptr);		

		vkDestroySurfaceKHR(vulkanInstance->GetHandle(), initSurface, nullptr);

		vulkanInstance->FreeWindow(initWindow);		

		device = VK_NULL_HANDLE;
		physicalDevice = VK_NULL_HANDLE;
		graphicsQueueFamily = -1;
		presentQueueFamily = -1;
		initSurface = VK_NULL_HANDLE;		
		commandPool = VK_NULL_HANDLE;
		graphicsQueue = VK_NULL_HANDLE;
	}

	void GraphicsContext_Vulkan::WaitForIdle() const
	{
		vkDeviceWaitIdle(device);
	}

	void GraphicsContext_Vulkan::CreateWindowAndSurface(WindowSDL& window, VkSurfaceKHR& surface, StringView title, Vec2i pos, Vec2i size)
	{		
		if (initWindow.IsNullWindow())
		{
			window = std::move(initWindow);
			surface = initSurface;									
		}
		else
		{
			window = vulkanInstance->CreateWindow(title, pos, size);
			if (SDL_Vulkan_CreateSurface((SDL_Window*)window.GetHandle(), vulkanInstance->GetHandle(), &surface) == SDL_FALSE)
				Debug::Logger::LogFatal("Blaze Engine", "SDL failed to create vulkan surface: \"" + (String)SDL_GetError() + "\"\n");
		}		
	}

	void GraphicsContext_Vulkan::DestroyWindowAndSurface(WindowSDL& window, VkSurfaceKHR surface)
	{
		vulkanInstance->FreeWindow(window);
		vkDestroySurfaceKHR(vulkanInstance->GetHandle(), surface, nullptr);
	}

	void GraphicsContext_Vulkan::RunSingleTimeCommand(const std::function<void(VkCommandBuffer)>& func)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		func(commandBuffer);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

	//VkFormat GraphicsContext_Vulkan::ChooseFormatForLinearTilingImage(std::initializer_list<VkFormat> formats, VkFormatFeatureFlags features)
	//{
	//	for (auto& format : formats)
	//	{
	//		VkFormatProperties2 properties{ };
	//		properties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
	//
	//		vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, &properties);
	//
	//		if ((properties.formatProperties.linearTilingFeatures & features) == features)
	//			return format;
	//	}
	//
	//	return VK_FORMAT_UNDEFINED;
	//}
	//VkFormat GraphicsContext_Vulkan::ChooseFormatForOptimalTilingImage(std::initializer_list<VkFormat> formats, VkFormatFeatureFlags features)
	//{
	//	for (auto& format : formats)
	//	{
	//		VkFormatProperties2 properties{ };
	//		properties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
	//
	//		vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, &properties);
	//
	//		if ((properties.formatProperties.optimalTilingFeatures & features) == features)
	//			return format;
	//	}
	//
	//	return VK_FORMAT_UNDEFINED;
	//}	

	void* GraphicsContext_Vulkan::CreateStagingBuffer(uint size, VkBuffer& buffer, VmaAllocation& allocation)
	{				
		BlazeVMAData* vmaData = GetBlazeVMAData(*this);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocationInfo{};
		allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VmaAllocationInfo info;

		if (vmaCreateBuffer(vmaData->GetAllocator(), &bufferInfo, &allocationInfo, &buffer, &allocation, &info) < 0)
			Debug::Logger::LogFatal("Blaze graphics API", "VMA failed to create an buffer");

		return info.pMappedData;
	}

	void GraphicsContext_Vulkan::CreateImage2D(Vec2i size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageLayout layout, VkImage& image, VmaAllocation& allocation)
	{
		BlazeVMAData* vmaData = GetBlazeVMAData(*this);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(size.x);
		imageInfo.extent.height = static_cast<uint32_t>(size.y);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = layout;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		VmaAllocationCreateInfo allocationInfo{};
		allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;

		if (vmaCreateImage(vmaData->GetAllocator(), &imageInfo, &allocationInfo, &image, &allocation, nullptr) < 0)
			Debug::Logger::LogFatal("Blaze graphics API", "VMA failed to create an image");
	}

	void GraphicsContext_Vulkan::CopyBufferToImage(VkBuffer buffer, VkImage image, uint bufferOffset, uint bufferBitmapStride, Vec2i imageOffset, Vec2i imageSize, VkImageLayout layout)
	{
		RunSingleTimeCommand([&](VkCommandBuffer commandBuffer) {

			VkBufferImageCopy region{};
			region.bufferOffset = bufferOffset;
			region.bufferRowLength = bufferBitmapStride;
			region.bufferImageHeight = 0;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;
			region.imageOffset = { imageOffset.x, imageOffset.y, 0 };
			region.imageExtent = { (uint)imageSize.x, (uint)imageSize.y, 1 };

			vkCmdCopyBufferToImage(
				commandBuffer,
				buffer,
				image,
				layout,
				1,
				&region
			);

			});
	}

	void GraphicsContext_Vulkan::TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcAccess, VkAccessFlags dstAccess)
	{
		RunSingleTimeCommand([&](VkCommandBuffer commandBuffer) {
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = srcAccess; // TODO
			barrier.dstAccessMask = dstAccess; // TODO

			vkCmdPipelineBarrier(
				commandBuffer,
				srcStage, dstStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);
			});
	}

	GraphicsContext_Vulkan& GraphicsContext_Vulkan::operator=(GraphicsContext_Vulkan&& other) noexcept
	{
		Destroy();

		device = other.device;
		physicalDevice = other.physicalDevice;
		graphicsQueueFamily = other.graphicsQueueFamily;
		presentQueueFamily = other.presentQueueFamily;
		initWindow = std::move(other.initWindow);
		initSurface = other.initSurface;		
		commandPool = other.commandPool;
		graphicsQueue = other.graphicsQueue;
		dataMap = std::move(other.dataMap);

		other.device = NULL;
		other.physicalDevice = NULL;
		other.graphicsQueueFamily = -1;
		other.presentQueueFamily = -1;	
		other.initSurface = NULL;		
		other.commandPool = VK_NULL_HANDLE;
		other.graphicsQueue = VK_NULL_HANDLE;

		return *this;
	}

	Semaphore_Vulkan::Semaphore_Vulkan(GraphicsContext_Vulkan& graphicsContext)
		: handle(nullptr), graphicsContext(&graphicsContext)
	{
		if (!this->graphicsContext)
			return;

		VkSemaphoreCreateInfo createInfo{ };
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		vkCreateSemaphore(graphicsContext.GetDevice(), &createInfo, nullptr, &handle);
	}
	Semaphore_Vulkan::Semaphore_Vulkan(Semaphore_Vulkan&& other) noexcept
	{
		handle = other.handle;
		graphicsContext = other.graphicsContext;
		other.handle = nullptr;
		other.graphicsContext = nullptr;
	}
	Semaphore_Vulkan::~Semaphore_Vulkan()
	{
		Destroy();
	}
	void Semaphore_Vulkan::Destroy()
	{
		if (graphicsContext == nullptr)
			return;

		vkDestroySemaphore(graphicsContext->GetDevice(), handle, nullptr);

		handle = nullptr;
		graphicsContext = nullptr;
	}
	Semaphore_Vulkan& Semaphore_Vulkan::operator=(Semaphore_Vulkan&& other) noexcept
	{
		Destroy();
		graphicsContext = other.graphicsContext;
		handle = other.handle;
		other.handle = nullptr;
		other.graphicsContext = nullptr;

		return *this; 
	}

	RenderContext_Vulkan::RenderContext_Vulkan(GraphicsContext_Vulkan* graphicsContext, VkQueue queue)
		: graphicsContext(graphicsContext), queue(queue)
	{
	}

	RenderContext_Vulkan::RenderContext_Vulkan(RenderContext_Vulkan&& other) noexcept		
	{
		graphicsContext = other.graphicsContext;
		queue = other.queue;

		other.graphicsContext = nullptr;
		other.queue = VK_NULL_HANDLE;
	}

	RenderContext_Vulkan::~RenderContext_Vulkan()
	{
		Clear();
	}

	void RenderContext_Vulkan::WaitForIdle() const
	{
		vkQueueWaitIdle(queue);		
	}

	void RenderContext_Vulkan::Clear()
	{
		graphicsContext = nullptr;
		queue = VK_NULL_HANDLE;
	}

	RenderContext_Vulkan& RenderContext_Vulkan::operator=(RenderContext_Vulkan&& other) noexcept
	{
		Clear();

		graphicsContext = other.graphicsContext;
		queue = other.queue;

		other.graphicsContext = nullptr;
		other.queue = VK_NULL_HANDLE;

		return *this;
	}

	Framebuffer_Vulkan::Framebuffer_Vulkan()	
		: handle(VK_NULL_HANDLE), image(VK_NULL_HANDLE)
	{
	}
	Framebuffer_Vulkan::Framebuffer_Vulkan(Framebuffer_Vulkan&& other) noexcept		
	{		
		handle = other.handle;
		image = other.image;

		other.handle = VK_NULL_HANDLE;
		other.image = VK_NULL_HANDLE;
	}
	Framebuffer_Vulkan::~Framebuffer_Vulkan()
	{
		Clear();
	}
	void Framebuffer_Vulkan::Clear()
	{		
		//TODO
	}

	Framebuffer_Vulkan& Framebuffer_Vulkan::operator=(Framebuffer_Vulkan&& other) noexcept
	{	
		Clear();

		handle = other.handle;
		image = other.image;
		other.handle = VK_NULL_HANDLE;
		other.image = VK_NULL_HANDLE;

		return *this;
	}
	
	GraphicsContext_Vulkan CreateGraphicsContext(ImplementationInfo* implementationInfo)
	{
		if (implementationInfo->api != ImplementationAPI::Vulkan)
			Debug::Logger::LogWarning("Blaze graphics API", "Passing non Vulkan implementation info to a static Vulkan funtion.");

		return GraphicsContext_Vulkan({ 					
			});
	}
	Semaphore_Vulkan CreateSemaphore(GraphicsContext_Vulkan& graphicsContext)
	{
		return Semaphore_Vulkan(graphicsContext);
	}
	RenderContext_Vulkan CreateGraphicsRenderContext(GraphicsContext_Vulkan& graphicsContext)
	{
		VkQueue queue;
		vkGetDeviceQueue(graphicsContext.GetDevice(), graphicsContext.GetGraphicsQueueFamily(), 0, &queue);
		return RenderContext_Vulkan(&graphicsContext, queue);
	} 
	RenderContext_Vulkan CreatePresentRenderContext(GraphicsContext_Vulkan& graphicsContext)
	{
		VkQueue queue;
		vkGetDeviceQueue(graphicsContext.GetDevice(), graphicsContext.GetPresentQueueFamily(), 0, &queue);
		return RenderContext_Vulkan(&graphicsContext, queue);
	}
	RenderWindow_Vulkan CreateRenderWindow(GraphicsContext_Vulkan& graphicsContext, String title, Vec2i pos, Vec2i size)
	{
		return RenderWindow_Vulkan(&graphicsContext, title, pos, size);
	}
	RenderWindowFramebuffer_Vulkan& AcquireNextFramebuffer(RenderWindow_Vulkan& renderWindow, Semaphore_Vulkan& signalSemaphore)
	{
		return renderWindow.AcquireNextFramebuffer(signalSemaphore);
	} 	
}