#include "youtube.h"
#include <iostream>
#include <filesystem>
#include <HtmlParser/HtmlParser.h>

void example_youtube_custom_elements() {
	std::filesystem::path file{ std::string{"youtube.html"} };
	HtmlParser::HtmlParser hp{ file };
	auto dom = hp.parse();
	auto allRichThubmnails = dom.getElementsByClassName("rich-thumbnail");
	std::cout << "Total Rich thumbnails: " << allRichThubmnails.size() << '\n';
}
