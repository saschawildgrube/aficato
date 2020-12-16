$this->IncludeElement('responsive');
$responsive = new CResponsiveElement($this->GetWebsite(), 'default');

$strOutput = $responsive->RenderResponsive(
	'Extra Small',
	'Small',
	'Medium',
	'Large'); 