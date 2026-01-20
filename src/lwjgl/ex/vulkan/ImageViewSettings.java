package lwjgl.ex.vulkan;

import static org.lwjgl.vulkan.VK14.*;

// 参考
// https://github.com/lwjglgamedev/vulkanbook/blob/master/booksamples/chapter-04/src/main/java/org/vulkanb/eng/graph/vk/ImageView.java

public class ImageViewSettings {
	public static final int DEFAULT_BASE_ARRAY_LAYER = 0;
	public static final int DEFAULT_LAYER_COUNT = 1;
	public static final int DEFAULT_BASE_MIP_LEVEL = 0;
	public static final int DEFAULT_LEVEL_COUNT = 1;
	public static final int DEFAULT_VIEW_TYPE = VK_IMAGE_VIEW_TYPE_2D;

	private int aspectMask;
	private int baseArrayLayer = DEFAULT_BASE_ARRAY_LAYER;
	/**
	 * 本来はenumにするべきだが、LWJGLがこうなってしまっているのでしょうがない
	 */
	private int format;
	private int layerCount = DEFAULT_LAYER_COUNT;
	private int levelCount = DEFAULT_LEVEL_COUNT;
	private int baseMipLevel = DEFAULT_BASE_MIP_LEVEL;
	private int viewType = DEFAULT_VIEW_TYPE;

	private long imageHandler;
	
	private LogicalDevice logicalDevice;

	public int getAspectMask() {
		return aspectMask;
	}

	public void setAspectMask(int aspectMask) {
		this.aspectMask = aspectMask;
	}

	public int getBaseArrayLayer() {
		return baseArrayLayer;
	}

	public void setBaseArrayLayer(int baseArrayLayer) {
		this.baseArrayLayer = baseArrayLayer;
	}

	public int getFormat() {
		return format;
	}

	public void setFormat(int format) {
		this.format = format;
	}

	public int getLayerCount() {
		return layerCount;
	}

	public void setLayerCount(int layerCount) {
		this.layerCount = layerCount;
	}

	public int getLevelCount() {
		return levelCount;
	}

	public void setLevelCount(int levelCount) {
		this.levelCount = levelCount;
	}

	public int getBaseMipLevel() {
		return baseMipLevel;
	}

	public void setBaseMipLevel(int baseMipLevel) {
		this.baseMipLevel = baseMipLevel;
	}

	public int getViewType() {
		return viewType;
	}

	public void setViewType(int viewType) {
		this.viewType = viewType;
	}

	public long getImageHandler() {
		return imageHandler;
	}

	public void setImageHandler(long imageHandler) {
		this.imageHandler = imageHandler;
	}

	public LogicalDevice getLogicalDevice() {
		return logicalDevice;
	}

	public void setLogicalDevice(LogicalDevice logicalDevice) {
		this.logicalDevice = logicalDevice;
	}
}
