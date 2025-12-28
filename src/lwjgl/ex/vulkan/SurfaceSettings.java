package lwjgl.ex.vulkan;

import org.lwjgl.vulkan.VkInstance;
import org.lwjgl.vulkan.VkPhysicalDevice;

public class SurfaceSettings {
	private VkInstance vulkan;
	private VkPhysicalDevice device;
	private Window window;
	
	public VkInstance getVulkan() {
		return vulkan;
	}
	public void setVulkan(VkInstance vulkan) {
		this.vulkan = vulkan;
	}
	public VkPhysicalDevice getDevice() {
		return device;
	}
	public void setDevice(VkPhysicalDevice device) {
		this.device = device;
	}
	public Window getWindow() {
		return window;
	}
	public void setWindow(Window window) {
		this.window = window;
	}
}
