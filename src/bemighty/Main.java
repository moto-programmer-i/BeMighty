package bemighty;

import java.util.Arrays;
import java.util.Set;

import org.lwjgl.vulkan.KHRSwapchain;
import static org.lwjgl.vulkan.VK14.*;

import lwjgl.ex.vulkan.LogicalDevice;
import lwjgl.ex.vulkan.LogicalDeviceSettings;
import lwjgl.ex.vulkan.PhysicalDevice;
import lwjgl.ex.vulkan.Queue;
import lwjgl.ex.vulkan.Surface;
import lwjgl.ex.vulkan.SurfaceSettings;
import lwjgl.ex.vulkan.Vulkan;
import lwjgl.ex.vulkan.VulkanSettings;
import lwjgl.ex.vulkan.Window;
import lwjgl.ex.vulkan.WindowSettings;

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
//			window.swapBuffers();
			
//			Requested layer "VK_LAYER_KHRONOS_validation" failed to load!
//			libVkLayer_khronos_validation.so: 共有オブジェクトファイルを開けません: そのようなファイルやディレクトリはありません
			
			try(var vulkan = new Vulkan(vulkanSettings)) {
				var vkPhysicalDevice = PhysicalDevice.getFirstVkPhysicalDevice(vulkan);
				var physicalDevice = new PhysicalDevice(vkPhysicalDevice);
				var logicalDeviceSettings = new LogicalDeviceSettings();
				logicalDeviceSettings.setPhysicalDevice(physicalDevice);
				try(var logicalDevice = new LogicalDevice(logicalDeviceSettings)) {
					
					var surfaceSettings = new SurfaceSettings();
					surfaceSettings.setVulkan(vulkan.getVkInstance());
					surfaceSettings.setDevice(vkPhysicalDevice);
					surfaceSettings.setWindow(window);
					
					// vulkanインスタンスclose時にまとめてcloseしていいか不明、良いならやる
					try(var surface = new Surface(surfaceSettings)) {
						
						
						Queue queue = new Queue(logicalDevice);
						
						System.out.println(queue);
						window.waitUntilClose();
					}
				}
			}
		}
	}

}
