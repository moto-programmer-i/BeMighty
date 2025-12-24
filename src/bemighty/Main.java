package bemighty;

import java.util.Arrays;
import java.util.Set;

import org.lwjgl.vulkan.KHRSwapchain;
import static org.lwjgl.vulkan.VK14.*;

import lwjgl.ex.vulkan.LogicalDevice;
import lwjgl.ex.vulkan.LogicalDeviceSettings;
import lwjgl.ex.vulkan.PhysicalDevice;
import lwjgl.ex.vulkan.Vulkan;
import lwjgl.ex.vulkan.VulkanSettings;
import lwjgl.ex.window.Window;
import lwjgl.ex.window.WindowSettings;

public class Main {
	public static int WIDTH = 400;
	public static int HEIGHT = 400;
	public static String WINDOW_NAME = "Be Mighty";

	public static void main(String[] args) throws Exception {
		var vulkanSettings = new VulkanSettings();
		vulkanSettings.setName(WINDOW_NAME);

		var windowSettings = new WindowSettings();
		windowSettings.setWidth(WIDTH);
		windowSettings.setHeight(HEIGHT);
		windowSettings.setName(WINDOW_NAME);
		try(var window = new Window(windowSettings)) {
			window.swapBuffers();
			
//			Requested layer "VK_LAYER_KHRONOS_validation" failed to load!
//			libVkLayer_khronos_validation.so: 共有オブジェクトファイルを開けません: そのようなファイルやディレクトリはありません
			
			try(var vulkan = new Vulkan(vulkanSettings)) {
				var vkPhysicalDevices = PhysicalDevice.getAllVkPhysicalDevice(vulkan);
				var physicalDevice = new PhysicalDevice(vkPhysicalDevices.getFirst());
				var logicalDeviceSettings = new LogicalDeviceSettings();
				logicalDeviceSettings.setPhysicalDevice(physicalDevice);
				try(var logicalDevice = new LogicalDevice(logicalDeviceSettings)) {
					System.out.println(logicalDevice.getDevice().address());
					
					window.waitUntilClose();
				}
			}
		}
	}

}
