package utils4game.lwjgl.window;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.GLFW_FALSE;
import static org.lwjgl.glfw.GLFW.GLFW_KEY_ESCAPE;
import static org.lwjgl.glfw.GLFW.GLFW_RELEASE;
import static org.lwjgl.glfw.GLFW.GLFW_RESIZABLE;
import static org.lwjgl.glfw.GLFW.GLFW_TRUE;
import static org.lwjgl.glfw.GLFW.GLFW_VISIBLE;
import static org.lwjgl.glfw.GLFW.glfwCreateWindow;
import static org.lwjgl.glfw.GLFW.glfwDefaultWindowHints;
import static org.lwjgl.glfw.GLFW.glfwDestroyWindow;
import static org.lwjgl.glfw.GLFW.glfwGetPrimaryMonitor;
import static org.lwjgl.glfw.GLFW.glfwGetVideoMode;
import static org.lwjgl.glfw.GLFW.glfwGetWindowFrameSize;
import static org.lwjgl.glfw.GLFW.glfwGetWindowSize;
import static org.lwjgl.glfw.GLFW.glfwInit;
import static org.lwjgl.glfw.GLFW.glfwMakeContextCurrent;
import static org.lwjgl.glfw.GLFW.glfwPollEvents;
import static org.lwjgl.glfw.GLFW.glfwSetErrorCallback;
import static org.lwjgl.glfw.GLFW.glfwSetKeyCallback;
import static org.lwjgl.glfw.GLFW.glfwSetWindowPos;
import static org.lwjgl.glfw.GLFW.glfwSetWindowShouldClose;
import static org.lwjgl.glfw.GLFW.glfwShowWindow;
import static org.lwjgl.glfw.GLFW.glfwSwapBuffers;
import static org.lwjgl.glfw.GLFW.glfwSwapInterval;
import static org.lwjgl.glfw.GLFW.glfwTerminate;
import static org.lwjgl.glfw.GLFW.glfwWindowHint;
import static org.lwjgl.glfw.GLFW.glfwWindowShouldClose;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.NULL;

import java.nio.IntBuffer;

import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.system.MemoryStack;


public class Window {
	/**
	 * vsyncを有効化する場合の、glfwSwapIntervalのデフォルト引数
	 */
	public static int DEFAULT_SWAP_INTERVAL = 1;
	/**
	 * 設定
	 */
	private WindowSettings settings; 

	/**
	 * ウィンドウハンドル
	 */
	private long window;
	
	/**
	 * ウィンドウを作成
	 */
	public Window(WindowSettings settings) {
		this.settings = settings;
		run();
	}
	
	// https://www.lwjgl.org/guide の内容から改変

	public void run() {
		init();
		loop();

		// Free the window callbacks and destroy the window
		glfwFreeCallbacks(window);
		glfwDestroyWindow(window);

		// Terminate GLFW and free the error callback
		glfwTerminate();
		glfwSetErrorCallback(null).free();
	}

	private void init() {
		// Setup an error callback. The default implementation
		// will print the error message in System.err.
		GLFWErrorCallback.createPrint(System.err).set();

		// Initialize GLFW. Most GLFW functions will not work before doing this.
		if ( !glfwInit() )
			throw new IllegalStateException("GLFWの初期化に失敗しました");

		// Configure GLFW
		glfwDefaultWindowHints(); // optional, the current window hints are already the default
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // the window will stay hidden after creation
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable

		// Create the window
		window = glfwCreateWindow(settings.getWidth(), settings.getHeight(), settings.getTitle(), NULL, NULL);
		if ( window == NULL )
			throw new RuntimeException("GLFWウィンドウの作成に失敗しました");

		// Setup a key callback. It will be called every time a key is pressed, repeated or released.
		glfwSetKeyCallback(window, (window, key, scancode, action, mods) -> {
			if ( key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE )
				glfwSetWindowShouldClose(window, true); // We will detect this in the rendering loop
		});

		// Get the thread stack and push a new frame
		try ( MemoryStack stack = stackPush() ) {
			IntBuffer pWidth = stack.mallocInt(1); // int*
			IntBuffer pHeight = stack.mallocInt(1); // int*

			
			// Get the window size passed to glfwCreateWindow
			glfwGetWindowSize(window, pWidth, pHeight);

			// Get the resolution of the primary monitor
			GLFWVidMode resolution = glfwGetVideoMode(glfwGetPrimaryMonitor());

			// ウィンドウの位置
			int xpos, ypos;
			switch(settings.getHorizontalPosition()) {
			case CENTER:
				xpos = (resolution.width() - pWidth.get(0)) / 2;
				break;
			case RIGHT:
				xpos = resolution.width();
				break;
			case LEFT:
			default:
				xpos = 0;
				break;
			}
			
			switch(settings.getVerticalPosition()) {
			case CENTER:
				ypos = (resolution.height() - pHeight.get(0)) / 2;
				break;
			case TOP:
				// ウィンドウの中身の左上が基準なので、最上部にウィンドウを表示する場合は、
				// タイトルバーの高さだけ、下に下げる必要がある
				// 参考 https://www.glfw.org/docs/3.3/spaces.svg
				
				// タイトルバーの領域を取得 
				IntBuffer left = stack.mallocInt(1); // int*
				IntBuffer top = stack.mallocInt(1); // int*
				IntBuffer right = stack.mallocInt(1); // int*
				IntBuffer bottom = stack.mallocInt(1); // int*
				glfwGetWindowFrameSize(window, left, top, right, bottom);
				// System.out.println("size: " + top.get(0) + " " + bottom.get(0));
				
				ypos = top.get(0) - bottom.get(0);
				break;
			case BOTTOM:
			default:
				ypos = resolution.height() - pHeight.get(0);
				break;
			}
			glfwSetWindowPos(
				window,
				xpos,
				ypos
			);
		}

		// Make the OpenGL context current
		glfwMakeContextCurrent(window);
		
		// Enable v-sync
		if (settings.isVsync()) {
			glfwSwapInterval(DEFAULT_SWAP_INTERVAL);
		}

		// Make the window visible
		glfwShowWindow(window);
	}

	private void loop() {
		// This line is critical for LWJGL's interoperation with GLFW's
		// OpenGL context, or any context that is managed externally.
		// LWJGL detects the context that is current in the current thread,
		// creates the GLCapabilities instance and makes the OpenGL
		// bindings available for use.
//		GL.createCapabilities();

		// Set the clear color
//		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

		// Run the rendering loop until the user has attempted to close
		// the window or has pressed the ESCAPE key.
		while ( !glfwWindowShouldClose(window) ) {
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the framebuffer

			glfwSwapBuffers(window); // swap the color buffers

			// Poll for window events. The key callback above will only be
			// invoked during this call.
			glfwPollEvents();
		}
	}
}
