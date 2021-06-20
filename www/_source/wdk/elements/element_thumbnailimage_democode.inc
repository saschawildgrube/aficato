$this->IncludeElement("thumbnailimage");

$thumbnailimage = new CThumbnailImageElement($this->GetWebsite(), "default");
$strOutput = $thumbnailimage->RenderThumbnailImage(
	"{IMAGEURL id=\"example_thumbnail\"}",
	"{IMAGEURL id=\"example\"}",
	600,
	450,
	"A nice thumb nail image");