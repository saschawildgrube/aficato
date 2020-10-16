$this->IncludeElement("tel");

$tel = new CTelElement($this->GetWebsite(), "default");
$strTel = "+491755247272";
$strText = "+49 (175) 524 7272";
$strOutput = $tel->RenderTel($strTel, $strText);