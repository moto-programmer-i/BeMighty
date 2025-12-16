package lwjgl.ex.vulkan;

/**
 * キューの機能（VkQueueFamilyPropertiesと、その他の情報）
 * https://javadoc.lwjgl.org/org/lwjgl/vulkan/VkQueueFamilyProperties.html
 */
public class QueueFamilyProperties {
	// 必要な情報のみ定義していく
	private int queueCount;

	public int getQueueCount() {
		return queueCount;
	}

	public void setQueueCount(int queueCount) {
		this.queueCount = queueCount;
	}
}
