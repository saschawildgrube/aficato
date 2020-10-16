$this->IncludeElement("pagenavigation");

$pagenavigation = new CPageNavigationElement($this->GetWebsite(),"default");
$strOutput = $pagenavigation->RenderPageNavigation(
	$this->GetModuleVar("pagenav_offset"),
	150,
	$this->GetModuleVarName("pagenav_offset"),
	10);