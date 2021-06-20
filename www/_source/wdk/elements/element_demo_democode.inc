$this->IncludeElement("demo");
$demo = new CDemoElement($this->GetWebsite(),"default");
$strOutput = $demo->RenderDemo(
	"This is a demo text.");