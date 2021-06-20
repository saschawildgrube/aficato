$this->IncludeElement("highlight");

$highlight = new CHighlightElement($this->GetWebsite(),"default");

$strOutput = $highlight->RenderHighlight(
	"<p>This text contains a highlighted word. This is a real highlight! Highlighting is case insensitive.</p>",
	"highlight"
	);


$strOutput .= $highlight->RenderHighlight(
	u("<p>This text contains some Umlauts like Ä,Ö,Ü,ä,ö,ü.<br/>
	And the Umlaut 'Ä' is supposed to be highlighted.</p>"),
	u("Ä")
	);