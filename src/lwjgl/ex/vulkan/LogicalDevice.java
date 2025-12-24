package lwjgl.ex.vulkan;

import java.nio.Buffer;
import org.lwjgl.PointerBuffer;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkDevice;
import org.lwjgl.vulkan.VkDeviceCreateInfo;
import org.lwjgl.vulkan.VkDeviceQueueCreateInfo;

import static org.lwjgl.vulkan.VK10.vkDestroyDevice;
import static org.lwjgl.vulkan.VK13.*;
import static org.lwjgl.system.MemoryUtil.*;

import java.util.List;
import java.util.Set;

import static lwjgl.ex.vulkan.Vulkan.*;

// 参考
// https://github.com/lwjglgamedev/vulkanbook/blob/master/booksamples/chapter-03/src/main/java/org/vulkanb/eng/graph/vk/Device.java

public class LogicalDevice implements AutoCloseable {
	private VkDevice device;

    public LogicalDevice(LogicalDeviceSettings settings) {

        try (var stack = MemoryStack.stackPush()) {
        	// Set<String> -> Buffer[]
        	PointerBuffer requiredExtensionsBuffer = stack.pointers(
        			settings.getRequiredExtensions().stream().map(stack::UTF8).toArray(Buffer[]::new)
        			);
        	
        	// capacity 1 を指定しないと戻り値の型がBufferにならない
        	var queueInfoBuffer = VkDeviceQueueCreateInfo.malloc(1, stack)
                    .sType$Default()
                    .pNext(NULL)
                    .flags(0)
                    .queueFamilyIndex(settings.getPhysicalDevice().getGraphicsQueueIndex().getAsInt())
                    .pQueuePriorities(stack.floats(settings.getQueuePriorities()));
        	
        	var deviceCreateInfo = VkDeviceCreateInfo.calloc(stack)
                    .sType$Default()
                    .ppEnabledExtensionNames(requiredExtensionsBuffer)
                    .pQueueCreateInfos(queueInfoBuffer);
                    
        	PointerBuffer deviceBuffer = stack.mallocPointer(1);
        	throwExceptionIfFailed(vkCreateDevice(settings.getPhysicalDevice().getDevice(), deviceCreateInfo, null, deviceBuffer),"論理デバイスの初期化に失敗しました");
            device = new VkDevice(deviceBuffer.get(0), settings.getPhysicalDevice().getDevice(), deviceCreateInfo);
        }
    }

	public VkDevice getDevice() {
		return device;
	}
	
	@Override
	public void close() throws Exception {
		if (device != null) {
			// 論理デバイスは明示的にDestroyする必要がある
			// The Vulkan spec states: All child objects that were created with instance or with a VkPhysicalDevice retrieved from it, and that can be destroyed or freed, must have been destroyed or freed prior to destroying instance (https://vulkan.lunarg.com/doc/view/1.4.321.1/linux/antora/spec/latest/chapters/initialization.html#VUID-vkDestroyInstance-instance-00629)
            vkDestroyDevice(device, null);
            device = null;
        }
	}
    
}
