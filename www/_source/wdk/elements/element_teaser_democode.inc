$this->IncludeElement("teaser");

$teaser = new CTeaserElement($this->GetWebsite(),"default");
$strOutput = $teaser->RenderTeaser(
	"Titulus Heador",
	"Loram Ipsum moro dolor. Blibb blu some blind text...",
	$this->GetWebsite()->MakeSelfURL(),
	"Read more",
	true);