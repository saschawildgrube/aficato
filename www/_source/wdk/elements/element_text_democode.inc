$this->IncludeElement("text");

$text = new CTextElement($this->GetWebsite(),"default");
$strOutput = $text->RenderText(
	"Titulus Heador",
	"Loram Ipsum moro dolor. Blibb blu some blind text."
	);