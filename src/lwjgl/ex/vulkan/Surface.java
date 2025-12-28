package lwjgl.ex.vulkan;

import java.nio.LongBuffer;

import org.lwjgl.glfw.GLFWVulkan;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkSurfaceCapabilitiesKHR;
import org.lwjgl.vulkan.VkSurfaceFormatKHR;
import org.lwjgl.vulkan.KHRSurface;

public class Surface implements AutoCloseable {
	final long handler;
	final SurfaceSettings settings;

	public Surface(SurfaceSettings settings) {
		this.settings = settings;
		try (MemoryStack stack = MemoryStack.stackPush()) {
            handler = settings.getWindow().createWindowSurfaceHandler(stack, settings.getVulkan());
			
            // 絶対必要だが、一旦このまま進めたらどうなるのか確かめる
//            VkSurfaceCapabilitiesKHR surfaceProperties = VkSurfaceCapabilitiesKHR.calloc(stack);
//            throwExceptionIfFailed(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(settings.getDevice().getDevice(), context.surface, surfaceProperties));
//
//            VkSurfaceFormatKHR format = VkSurfaceFormatKHR.malloc(stack);
		}
	}

	@Override
	public void close() throws Exception {
		KHRSurface.vkDestroySurfaceKHR(settings.getVulkan(), handler, null);
	}

}
