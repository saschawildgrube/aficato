$this->IncludeElement("downloadlink");

$downloadlink = new CDownloadLinkElement($this->GetWebsite(), "default");

$strOutput = $downloadlink->RenderDownloadLink(
	"{DOWNLOADPATH}test.pdf",
	"A pdf for download",
	"pdf");