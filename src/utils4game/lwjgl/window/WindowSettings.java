package utils4game.lwjgl.window;

import utils4game.lwjgl.*;

public class WindowSettings {
	private int width;
	private int height;
	private String title;
	private VerticalPosition verticalPosition = VerticalPosition.CENTER;
	private HorizontalPosition horizontalPosition = HorizontalPosition.CENTER;
	private boolean vsync = true;
	
	/**
	 * インスタンスを作成
	 * @param width
	 * @param height
	 * @param title
	 * @return
	 */
	public static WindowSettings create(int width, int height, String title) {
		return new WindowSettings(width, height, title);
	}
	
	private WindowSettings(int width, int height, String title) {
		this.width = width;
		this.height = height;
		this.title  = title;
	}
	
	public int getWidth() {
		return width;
	}
	
	public String getTitle() {
		return title;
	}

	//	public void setWidth(int width) {
//		this.width = width;
//	}
	public int getHeight() {
		return height;
	}
//	public void setHeight(int height) {
//		this.height = height;
//	}

	public WindowSettings setVerticalPosition(VerticalPosition position) {
		this.verticalPosition = position;
		return this;
	}

	public VerticalPosition getVerticalPosition() {
		return verticalPosition;
	}

	public HorizontalPosition getHorizontalPosition() {
		return horizontalPosition;
	}

	public WindowSettings setHorizontalPosition(HorizontalPosition horizontalPosition) {
		this.horizontalPosition = horizontalPosition;
		return this;
	}

	public boolean isVsync() {
		return vsync;
	}

	public WindowSettings setVsync(boolean vsync) {
		this.vsync = vsync;
		return this;
	}
}
