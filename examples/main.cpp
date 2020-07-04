#include <iostream>
#include "youtube.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	std::cout << "Running examples\n";
	try {
		example_youtube_custom_elements();
	}
	catch (const std::exception& ex) {
		std::cout << "Error occurred: " << ex.what() << '\n';
	}
	catch (...) {
		std::cout << "Unknown error ocurred\n";
	}
}
