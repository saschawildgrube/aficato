$this->IncludeElement("imageteaser");

$imageteaser = new CImageTeaserElement($this->GetWebsite(),"default");
$strOutput = $imageteaser->RenderImageTeaser(
	'{IMAGEURL id="example_thumbnail"}',
	"Titulus Heador",
	"Loram Ipsum moro dolor. Blibb blu some blind text...",
	$this->GetWebsite()->MakeSelfURL(),
	"Read more",
	true);