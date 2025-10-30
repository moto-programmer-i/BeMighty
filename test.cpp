import std;
import Files;

int main()
{
	constexpr std::string_view filename = "/run/media/user/linux-data/repositories/BeMighty/shaders/slang.spv";
	for (auto& line: Files::readAllLines(filename)) {
		std::cout << line << std::endl;
	}
}
