package lwjgl.ex.vulkan;

public class VulkanSettings {
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getApplicationVersion() {
		return applicationVersion;
	}
	public void setApplicationVersion(int applicationVersion) {
		this.applicationVersion = applicationVersion;
	}
	public int getEngineVersion() {
		return engineVersion;
	}
	public void setEngineVersion(int engineVersion) {
		this.engineVersion = engineVersion;
	}
	public boolean isValidate() {
		return validate;
	}
	public void setValidate(boolean validate) {
		this.validate = validate;
	}
	
	

	private String name;
	private int applicationVersion = 1;
	private int engineVersion = 1;
	boolean validate = true;
}
