package lwjgl.ex.vulkan;

import java.util.Set;

import org.lwjgl.vulkan.KHRSwapchain;

public class LogicalDeviceSettings {
	public static final float DEFAULT_QUEUE_PRIORITIES = 1.0f;
	public static final Set<String> DEFAULT_REQIRED_EXTENSIONS = Set.of(KHRSwapchain.VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	
	private PhysicalDevice physicalDevice;
	private Set<String> requiredExtensions = DEFAULT_REQIRED_EXTENSIONS;
	private float queuePriorities = DEFAULT_QUEUE_PRIORITIES;
	
	public PhysicalDevice getPhysicalDevice() {
		return physicalDevice;
	}
	public void setPhysicalDevice(PhysicalDevice physicalDevice) {
		this.physicalDevice = physicalDevice;
	}
	/**
	 * 初期値 {@value #DEFAULT_REQIRED_EXTENSIONS}
	 * @return
	 */
	public Set<String> getRequiredExtensions() {
		return requiredExtensions;
	}
	public void setRequiredExtensions(Set<String> requiredExtensions) {
		this.requiredExtensions = requiredExtensions;
	}
	
	/**
	 * 初期値 {@value #DEFAULT_QUEUE_PRIORITIES}
	 * @return
	 */
	public float getQueuePriorities() {
		return queuePriorities;
	}
	public void setQueuePriorities(float queuePriorities) {
		this.queuePriorities = queuePriorities;
	}
}
