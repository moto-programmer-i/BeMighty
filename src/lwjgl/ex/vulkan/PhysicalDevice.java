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
	private final VkPhysicalDevice device;

	/**
	 * getFirstPhysicalDeviceから初期化
	 * @param vulkan
	 */
	public PhysicalDevice(Vulkan vulkan) {
		this(getFirstPhysicalDevice(vulkan));
	}
	
	public PhysicalDevice(VkPhysicalDevice device) {
		this.device = device;
	}
	
	public static VkPhysicalDevice getFirstPhysicalDevice(Vulkan vulkan) {
		return getAllVkPhysicalDevice(vulkan).get(0);
	}
	
	public VkPhysicalDevice getDevice() {
		return device;
	}

	/**
	 * 条件に合う最初のデバイスを返す
	 * @param vulkan
	 * @param filter
	 * @return
	 */
	public static VkPhysicalDevice getFirstPhysicalDevice(Vulkan vulkan, PhysicalDeviceFilter filter) {
		return getAllVkPhysicalDevice(vulkan, filter).get(0);
	}

	public static List<VkPhysicalDevice> getAllVkPhysicalDevice(Vulkan vulkan) {
		return getAllVkPhysicalDevice(vulkan, new PhysicalDeviceFilter());
	}

	public static List<VkPhysicalDevice> getAllVkPhysicalDevice(Vulkan vulkan, PhysicalDeviceFilter filter) {
		var devices = new ArrayList<VkPhysicalDevice>();

		try (var stack = MemoryStack.stackPush()) {
			// 物理デバイスの数を取得
			PointerBuffer pPhysicalDevices;
			IntBuffer deviceCountBuffer = stack.mallocInt(1);
			Vulkan.throwExceptionIfFailed(vkEnumeratePhysicalDevices(vulkan.getVkInstance(), deviceCountBuffer, null),
					"物理デバイスの数の取得に失敗しました");
			int deviceCount = deviceCountBuffer.get(0);
			pPhysicalDevices = stack.mallocPointer(deviceCount);
			Vulkan.throwExceptionIfFailed(
					vkEnumeratePhysicalDevices(vulkan.getVkInstance(), deviceCountBuffer, pPhysicalDevices),
					"物理デバイスの取得に失敗しました");

			int capacity = pPhysicalDevices.capacity();
			for (int i = 0; i < capacity; ++i) {
				var device = new VkPhysicalDevice(pPhysicalDevices.get(i), vulkan.getVkInstance());

				// Extensionで絞り込み
				// Featuresも似たようなことをしないといけない？
				var supportedExtensions = getExtensions(device);
				if (!supportedExtensions.containsAll(filter.getExtensions())) {
					continue;
				}

				// その他で絞り込み
				if (filter.hasGraphicsQueueFamily()) {
					if(!hasGraphicsQueueFamily(getQueueFamilyProperties(device))) {
						continue;
					}
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
			Vulkan.throwExceptionIfFailed(
					vkEnumerateDeviceExtensionProperties(device, (String) null, vkDeviceExtensionsBuffer, null),
					"デバイスのVulkan拡張機能の数の取得に失敗しました");
			var vkDeviceExtensions = VkExtensionProperties.calloc(vkDeviceExtensionsBuffer.get(0));
			Vulkan.throwExceptionIfFailed(vkEnumerateDeviceExtensionProperties(device, (String) null,
					vkDeviceExtensionsBuffer, vkDeviceExtensions), "デバイスのVulkan拡張機能の取得に失敗しました");

			int extensionCount = vkDeviceExtensionsBuffer.get();
			var extensions = new HashSet<String>(extensionCount);
			for (int i = 0; i < extensionCount; i++) {
				extensions.add(vkDeviceExtensions.get(i).extensionNameString());
			}
			return extensions;
		}
	}

	public static List<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device) {
		var list = new ArrayList<VkQueueFamilyProperties>();
		try (var stack = MemoryStack.stackPush()) {
			IntBuffer queueFamilyPropertiesBuffer = stack.mallocInt(1);
			vkGetPhysicalDeviceQueueFamilyProperties(device, queueFamilyPropertiesBuffer, null);
			int queueFamilyProperyCount = queueFamilyPropertiesBuffer.get(0);
			var queueFamilyProperties = VkQueueFamilyProperties.calloc(queueFamilyProperyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, queueFamilyPropertiesBuffer, queueFamilyProperties);
			for (int i = 0; i < queueFamilyProperyCount; ++i) {
				list.add(queueFamilyProperties.get(i));
			}
			return list;
		}
	}
	
	public static boolean hasGraphicsQueueFamily(List<VkQueueFamilyProperties> properties) {
		for(var property: properties) {
			if ((property.queueFlags() & VK_QUEUE_GRAPHICS_BIT) != 0) {
                return true;
            }
		}
		return false;
	}
}
