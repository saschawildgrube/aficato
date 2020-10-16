$this->IncludeElement('image');

$link = new CImageElement($this->GetWebsite(), 'default');
$strOutput = $link->RenderImage(
	$this->GetWebsite()->GetLayoutImageURL('example'),
	'A nice image');