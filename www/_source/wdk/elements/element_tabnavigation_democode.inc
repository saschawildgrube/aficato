$this->IncludeElement("tabnavigation");

$tabnavigationDef = new CTabNavigationDefinition();
	
for ($nIndex = 1; $nIndex <= 5; $nIndex++)
{
	$strText = "Tab $nIndex";
	if ($nIndex==4)
	{
		$strText .= " with more text";
	}
	$arrayParams = array();
	$arrayParams["tab"] = "tab_$nIndex";
	$tabnavigationDef->AddTab(
		$arrayParams["tab"],
		$strText,
		$this->GetWebsite()->MakeURL($arrayParams));
}


$tabnavigation = new CTabNavigationElement(
	$this->GetWebsite(),
	"default");
$strOutput = $tabnavigation->RenderTabNavigation(
	$tabnavigationDef,
	$this->GetWebsite()->GetVar("tab"));