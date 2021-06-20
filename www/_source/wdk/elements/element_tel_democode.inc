$this->IncludeElement("tel");


$strTel = '+49 (175) 524 7272';

$tel = new CTelElement($this->GetWebsite(), 'icon');
$strOutput = $tel->RenderTel($strTel);

$strOutput .= '<br/>';

$tel = new CTelElement($this->GetWebsite(), 'default');
$strOutput .= $tel->RenderTel($strTel);

