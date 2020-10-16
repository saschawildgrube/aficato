$this->IncludeElement("icon");

$icon = new CIconElement($this->GetWebsite(), "default");

$strOutput = $icon->RenderIcon("link","An icon without a link");
$strOutput .= $icon->RenderIcon("mail","A mail icon");