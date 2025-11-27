package bemighty;

import lwjgl.ex.window.Window;
import lwjgl.ex.window.WindowSettings;

public class Main {
	public static int WIDTH = 400;
	public static int HEIGHT = 400;
	public static String WINDOW_NAME = "Be Mighty";

	public static void main(String[] args) {
		var settings = new WindowSettings();
		settings.setWidth(WIDTH);
		settings.setHeight(HEIGHT);
		settings.setName(WINDOW_NAME);
		new Window(settings);
	}

}
