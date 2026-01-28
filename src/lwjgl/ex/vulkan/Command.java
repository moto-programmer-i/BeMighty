package lwjgl.ex.vulkan;

import org.lwjgl.system.MemoryStack;

@FunctionalInterface
public interface Command {
	public void render(MemoryStack stack, SwapChain swapChain);
	
	// Render -> FrameRender -> CommandBuffer -> Command -> swapChain
}
