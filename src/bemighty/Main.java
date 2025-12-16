package bemighty;

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

				var list = physicalDevice.getQueueFamilyPropertiesList();
				for(int i = 0; i < list.size(); ++i) {
					System.out.println("ーーーーーーーーーーーーーーーーーーー");
					System.out.println("キュー " + i);
					var queue = list.get(i);
					System.out.println("queueCount " + queue.getQueueCount());
				}
			}
			
			window.waitUntilClose();
		}
	}

}
