/**
 * nativesライブラリはコンパイルエラーがでないので忘れないこと
 */
module BeMighty {
	requires org.lwjgl;
	requires org.lwjgl.natives;
	
	requires org.lwjgl.glfw;
	requires org.lwjgl.glfw.natives;
	
	requires org.lwjgl.assimp.natives;
	
	requires org.lwjgl.openal.natives;
	
	requires org.lwjgl.stb.natives;
	
	requires org.lwjgl.vulkan;
	
	requires org.joml;
}