$this->IncludeElement('flag');

$flag = new CFlagElement($this->GetWebsite(), 'default');

$strOutput = $flag->RenderFlag('FRA');
$strOutput .= $flag->RenderFlag('DEU');