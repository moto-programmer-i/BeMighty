package be_mighty;

import utils4game.lwjgl.VerticalPosition;
import utils4game.lwjgl.window.Window;
import utils4game.lwjgl.window.WindowSettings;

public class Test {
	public static final int width = 400;
	public static final int height = 400;
	public static final String title = "test";

	public static void main(String[] args) {
		new Window(
				WindowSettings.create(width, height, title)
				.setVerticalPosition(VerticalPosition.TOP)
				);

	}

}
