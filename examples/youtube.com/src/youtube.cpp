#include <iostream>
#include <filesystem>
#include <HtmlParser/HtmlParser.h>

//int main(int argc, char** argv) {
int main() {
	std::filesystem::path file{ std::string{"../youtube.html"} };
	HtmlParser::HtmlParser hp{ file };
	auto dom = hp.parse();
	auto allCustomElements = dom.getElementsByTagName("ytd-rich-item-renderer"); //youtube custom elements
	std::cout << "Total: " << allCustomElements.size() << '\n';
}
