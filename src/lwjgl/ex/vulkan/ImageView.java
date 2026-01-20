package lwjgl.ex.vulkan;

import java.nio.LongBuffer;

import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkImageViewCreateInfo;
import static org.lwjgl.vulkan.VK14.*;

// 参考
// https://github.com/lwjglgamedev/vulkanbook/blob/master/booksamples/chapter-04/src/main/java/org/vulkanb/eng/graph/vk/ImageView.java
public class ImageView implements AutoCloseable {
	private final long handler;
	private final ImageViewSettings settings;
	public ImageView(ImageViewSettings settings) {
		this.settings = settings;
		try (var stack = MemoryStack.stackPush()) {
            LongBuffer lp = stack.mallocLong(1);
            var viewCreateInfo = VkImageViewCreateInfo.calloc(stack)
                    .sType$Default()
                    .image(settings.getImageHandler())
                    .viewType(settings.getViewType())
                    // formatがimageに紐づくなら、imageクラスのほうがいい
                    .format(settings.getFormat())
                    .subresourceRange(it -> it
                            .aspectMask(settings.getAspectMask())
                            .baseMipLevel(settings.getBaseMipLevel())
                            .levelCount(settings.getLevelCount())
                            .baseArrayLayer(settings.getBaseArrayLayer())
                            .layerCount(settings.getLayerCount()));

            Vulkan.throwExceptionIfFailed(vkCreateImageView(settings.getLogicalDevice().getDevice(), viewCreateInfo, null, lp),
                    "ImageViewの作成に失敗しました");
            handler = lp.get(0);
		}
	}
	@Override
	public void close() throws Exception {
		vkDestroyImageView(settings.getLogicalDevice().getDevice(), handler, null);
	}
}
