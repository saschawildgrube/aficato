$this->IncludeElement("iconteaser");

$iconteaser = new CIconTeaserElement($this->GetWebsite(),"default");
$strOutput = $iconteaser->RenderIconTeaser(
	'diamond',
	"Titulus Heador",
	"Loram Ipsum moro dolor. Blibb blu some blind text...",
	$this->GetWebsite()->MakeSelfURL(),
	"Read more",
	true);