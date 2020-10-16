$this->IncludeElement("separator");

$separator = new CSeparatorElement($this->GetWebsite(), "default");
$strOutput = "<p>Some text before the separator.</p>\n";
$strOutput .= $separator->RenderSeparator();
$strOutput .= "<p>Some text after the separator.</p>\n";