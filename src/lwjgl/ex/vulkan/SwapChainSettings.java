package lwjgl.ex.vulkan;

public class SwapChainSettings {
	public static final int DEFAULT_IMAGE_ARRAY_LAYERS = 1;
	private Window window;
	private LogicalDevice logicalDevice;
	private Surface surface;
	// int requestedImages
	private int imageArrayLayers = DEFAULT_IMAGE_ARRAY_LAYERS;
	
	private boolean vsync;
	public Window getWindow() {
		return window;
	}
	public void setWindow(Window window) {
		this.window = window;
	}
	public LogicalDevice getLogicalDevice() {
		return logicalDevice;
	}
	public void setLogicalDevice(LogicalDevice logicalDevice) {
		this.logicalDevice = logicalDevice;
	}
	public Surface getSurface() {
		return surface;
	}
	public void setSurface(Surface surface) {
		this.surface = surface;
	}
	
	public int getImageArrayLayers() {
		return imageArrayLayers;
	}
	public void setImageArrayLayers(int imageArrayLayers) {
		this.imageArrayLayers = imageArrayLayers;
	}
	public boolean isVsync() {
		return vsync;
	}
	public void setVsync(boolean vsync) {
		this.vsync = vsync;
	}
}
