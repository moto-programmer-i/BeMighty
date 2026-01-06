package lwjgl.ex.vulkan;

import org.lwjgl.PointerBuffer;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkQueue;
import static org.lwjgl.vulkan.VK14.*;

public class Queue {
	// キューの適切な初期値不明、一旦0にしておく
	public static final int DEFAULT_QUEUE_INDEX = 0;
	private VkQueue vkQueue;
	public Queue(LogicalDevice device) {
		this(device, device.getGraphicsQueueIndex().getAsInt(), DEFAULT_QUEUE_INDEX);
	}
	public Queue(LogicalDevice device, int queueFamilyIndex, int queueIndex) {
		try (var stack = MemoryStack.stackPush()) {
            PointerBuffer queueBuffer = stack.mallocPointer(1);
            vkGetDeviceQueue(device.getDevice(), queueFamilyIndex, queueIndex, queueBuffer);
            vkQueue = new VkQueue(queueBuffer.get(0), device.getDevice());
        }
	}
	
	public VkQueue getVkQueue() {
		return vkQueue;
	}
}
