Html Parser


Simple Html Parser, there is not javascript evaluation this is strictly for html.

Examples:

```
	std::filesystem::path file{ std::string{"BigPage.html"} };
	HtmlParser::HtmlParser hp{file};
	auto dom = hp.parse();
	auto div = dom.getElementById("primary");
	auto allLinks = div->getElementsByTagName("a");
```