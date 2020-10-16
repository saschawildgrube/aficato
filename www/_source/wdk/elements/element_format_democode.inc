$this->IncludeElement("format");
$this->IncludeElement("text");

$format_red = new CFormatElement($this->GetWebsite(),"red");
$format_strong = new CFormatElement($this->GetWebsite(),"strong");
$text = new CTextElement($this->GetWebsite(),"default");
$strOutput = $text->RenderText(
	"A Header with some ".$format_red->RenderFormat("red colored")." words",
	"And this is some text containing ".$format_strong->RenderFormat("significantly emphasized")." words."
	);