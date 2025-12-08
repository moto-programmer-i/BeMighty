package lwjgl.ex.vulkan;

// 参考
// https://github.com/lwjglgamedev/vulkanbook/blob/master/booksamples/chapter-03/src/main/java/org/vulkanb/eng/graph/vk/PhysDevice.java

import org.lwjgl.PointerBuffer;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.*;

import java.nio.IntBuffer;
import java.util.*;

import static org.lwjgl.vulkan.VK13.*;

public class PhysicalDevice {

	public PhysicalDevice(Vulkan instance) {

		

	}

	public static List<VkPhysicalDevice> getAllVkPhysicalDevice(Vulkan vulkan) {
		return getAllVkPhysicalDevice(vulkan, new PhysicalDeviceFilter());
	}
	
	public static List<VkPhysicalDevice> getAllVkPhysicalDevice(Vulkan vulkan, PhysicalDeviceFilter filter) {
		var devices = new ArrayList<VkPhysicalDevice>();

		try (var stack = MemoryStack.stackPush()) {
			// 物理デバイスの数を取得
			PointerBuffer pPhysicalDevices;
			IntBuffer numDevicesBuffer = stack.mallocInt(1);
			Vulkan.throwExceptionIfFailed(vkEnumeratePhysicalDevices(vulkan.getVkInstance(), numDevicesBuffer, null),
					"物理デバイスの数の取得に失敗しました");
			int numDevices = numDevicesBuffer.get(0);
			pPhysicalDevices = stack.mallocPointer(numDevices);
			Vulkan.throwExceptionIfFailed(
					vkEnumeratePhysicalDevices(vulkan.getVkInstance(), numDevicesBuffer, pPhysicalDevices),
					"物理デバイスの取得に失敗しました");
			
			int capacity = pPhysicalDevices.capacity();
			for (int i = 0; i < capacity; ++i) {
				var device = new VkPhysicalDevice(pPhysicalDevices.get(i), vulkan.getVkInstance());
				
				// Extensionで絞り込み
				// Featuresも似たようなことをしないといけない？
				var supportedExtensions = getExtensions(device);
				if(!supportedExtensions.containsAll(filter.getExtensions())) {
					continue;
				}
				
				devices.add(device);
			}
		}

		return devices;
	}
	
	public static Set<String> getExtensions(VkPhysicalDevice device) {
		try (var stack = MemoryStack.stackPush()) {
			var vkDeviceExtensionsBuffer = stack.mallocInt(1);
			// Get device extensions
	        Vulkan.throwExceptionIfFailed(vkEnumerateDeviceExtensionProperties(device, (String) null, vkDeviceExtensionsBuffer, null),
	                "デバイスのVulkan拡張機能の数の取得に失敗しました");
	        var vkDeviceExtensions = VkExtensionProperties.calloc(vkDeviceExtensionsBuffer.get(0));
	        Vulkan.throwExceptionIfFailed(vkEnumerateDeviceExtensionProperties(device, (String) null, vkDeviceExtensionsBuffer, vkDeviceExtensions),
	                "デバイスのVulkan拡張機能の取得に失敗しました");
	        
	        int numExtensions = vkDeviceExtensionsBuffer.get();
	        var extensions = new HashSet<String>(numExtensions);
	        for (int i = 0; i < numExtensions; i++) {
	        	extensions.add(vkDeviceExtensions.get(i).extensionNameString());
	        }
	        return extensions;
		}
    }

}
