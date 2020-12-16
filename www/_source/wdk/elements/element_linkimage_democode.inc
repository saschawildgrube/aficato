$this->IncludeElement('linkimage');
$linkimage = new CLinkImageElement($this->GetWebsite(), 'default');
$strOutput = $linkimage->RenderLinkImage(
	'http://www.example.com',
	$this->GetWebsite()->GetLayoutImageURL('profile_male'),
	'A man',
	true);
$strOutput .= $linkimage->RenderLinkImage(
	'http://www.example.com',
	$this->GetWebsite()->GetLayoutImageURL('profile_female'),
	'A woman',
	true);
