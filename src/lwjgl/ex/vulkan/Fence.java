package lwjgl.ex.vulkan;

import java.nio.LongBuffer;

import org.lwjgl.system.MemoryStack;
import org.lwjgl.vulkan.VkFenceCreateInfo;
import static org.lwjgl.vulkan.VK14.*;

// 参考
// https://github.com/lwjglgamedev/vulkanbook/blob/master/booksamples/chapter-05/src/main/java/org/vulkanb/eng/graph/vk/Fence.java

/**
 * CPU側の同期のための待機機構
 * https://chaosplant.tech/do/vulkan/ex2/
 */
public class Fence implements AutoCloseable {
	private final long handler;
	private final LogicalDevice logicalDevice;
	
	public Fence(LogicalDevice logicalDevice) {
		this.logicalDevice = logicalDevice;
		try (var stack = MemoryStack.stackPush()) {
            var info = VkFenceCreateInfo.calloc(stack)
                    .sType$Default();
            		// 最初からsignaled、待機終了にしたいときもあるらしいが不明なので保留
                    // .flags(signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0);

            LongBuffer lp = stack.mallocLong(1);
            Vulkan.throwExceptionIfFailed(vkCreateFence(logicalDevice.getDevice(), info, null, lp), "Fenceの作成に失敗しました");
            handler = lp.get(0);
        }
	}

// vkWaitForFencesは内部で勝手に呼ばれる？
//	/**
//	 * 同期待ちに入る（waitがObjectのメソッドであり、オーバーライドになってしまう）
//	 */
//	public void startWaiting() {
//        // 引数waitAllは、LWJGLの設計ミス。本来複数ハンドラを送る場合に、1つでも完了したら待機終了とするときに使う
//        Vulkan.throwExceptionIfFailed(vkWaitForFences(logicalDevice.getDevice(), handler, true, Long.MAX_VALUE)
//        		, "Fenceの待機開始に失敗しました");
//	}
	
	public void reset() {
		Vulkan.throwExceptionIfFailed(vkResetFences(logicalDevice.getDevice(), handler)
				, "Fenceのリセットに失敗しました");
	}

	@Override
	public void close() throws Exception {
		vkDestroyFence(logicalDevice.getDevice(), handler, null);
	}

	public long getHandler() {
		return handler;
	}
}
