$this->IncludeElement("header");

$header = new CHeaderElement($this->GetWebsite(),"default");
$strOutput = "";
for ($nLevel = 1; $nLevel <= 6; $nLevel++)
{
	$strOutput .= $header->RenderHeader("Header Level ".$nLevel,$nLevel);
}