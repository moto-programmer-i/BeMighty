package lwjgl.ex.vulkan;

import org.lwjgl.PointerBuffer;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkCommandBufferSubmitInfo;
import org.lwjgl.vulkan.VkQueue;
import org.lwjgl.vulkan.VkSubmitInfo2;

import static org.lwjgl.vulkan.VK14.*;

import java.util.List;

/**
 * GPUの中にあるやることリスト
 * https://chaosplant.tech/do/vulkan/2-3/
 */
public class Queue {
	private final QueueSettings settings;
	private final VkQueue vkQueue;

	public Queue(QueueSettings settings) {
		this.settings = settings;
		try (var stack = MemoryStack.stackPush()) {
			PointerBuffer queueBuffer = stack.mallocPointer(1);
			vkGetDeviceQueue(settings.getLogicalDevice().getDevice(), settings.getQueueFamilyIndex(),
					settings.getQueueIndex(), queueBuffer);
			vkQueue = new VkQueue(queueBuffer.get(0), settings.getLogicalDevice().getDevice());	
		}
	}

	public void waitIdle() {
		vkQueueWaitIdle(vkQueue);
	}

	public VkQueue getVkQueue() {
		return vkQueue;
	}
}
