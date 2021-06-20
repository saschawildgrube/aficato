$this->IncludeElement("paragraph");

$paragraph = new CParagraphElement($this->GetWebsite(),"default");
$strOutput = $paragraph->RenderParagraph(
	"Loram Ipsum moro dolor. Blibb blu some blind text."
	);