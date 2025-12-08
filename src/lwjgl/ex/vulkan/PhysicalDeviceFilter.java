package lwjgl.ex.vulkan;

import java.util.HashSet;
import java.util.Set;

import org.lwjgl.vulkan.KHRSwapchain;

public class PhysicalDeviceFilter {
	private final Set<String> extensions;
	private boolean hasGraphicsQueueFamily = true;
	// デバイスが画面に画像を表示できるかどうか

	public static final Set<String> DEFAULT_EXTENSIONS;

	static {
		DEFAULT_EXTENSIONS = new HashSet<>();
		// デバイスが画面に画像を表示できるかどうか
		DEFAULT_EXTENSIONS.add(KHRSwapchain.VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	public PhysicalDeviceFilter() {
		this(DEFAULT_EXTENSIONS);
	}

	public PhysicalDeviceFilter(Set<String> extensions) {
		this.extensions = extensions;
	}

	public boolean hasGraphicsQueueFamily() {
		return hasGraphicsQueueFamily;
	}

	public void setHasGraphicsQueueFamily(boolean hasGraphicsQueueFamily) {
		this.hasGraphicsQueueFamily = hasGraphicsQueueFamily;
	}

	public Set<String> getExtensions() {
		return extensions;
	}
}
