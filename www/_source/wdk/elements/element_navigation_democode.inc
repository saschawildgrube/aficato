$this->IncludeElement("text");

$text = new CTextElement($this->GetWebsite(),"default");

$strCurrentContextID = $this->GetModuleVar("currentcontextid");

$navtree = new CNavigationTree();
$navtree->LoadNavigationTree(
	$this->GetWebsite(),
	"demo");

$navtree_attach = new CNavigationTree();
$navtree_attach->LoadNavigationTree(
	$this->GetWebsite(),
	"demoattached");
$navtree->AttachNavigationTree(
	$navtree_attach,
	"22", // attach after node named "22"
	1);   // attach as a sub node of "22"
 
$navtree->SetCurrentContext($strCurrentContextID);
$navtree->SetContextParameterName($this->GetModuleVarName("currentcontextid"));

/*
	Please note: A website's main navigation structure is typically used to
	navigate content pages.
	Within the CWebsite::OnRenderNavigation function it looks like this:
	
	$navtree->SetCurrentContext($this->GetContent());
	$navtree->SetContextParameter("content");
*/



$strOutput = "";

$navigation = new CNavigationElement(
	$this->GetWebsite(),
	"horizontalpath");	
$strOutput .= $text->RenderText(
	"RenderNavigationHorizontalPath",
	$navigation->RenderNavigationHorizontalPath($navtree)
	);	
	
$strOutput .= "<p>Please note: the default implementation attaches the horizontal path navigation to the top edge of the screen. You may not see the actual navigation but the example version now.</p>";

$navigation = new CNavigationElement(
	$this->GetWebsite(),
	"currentpath");
$strOutput .= $text->RenderText(
	"RenderNavigationCurrentPath (aka \"Breadcrumb trail\")",
	$navigation->RenderNavigationCurrentPath($navtree)
	);


$navigation = new CNavigationElement(
	$this->GetWebsite(),
	"verticalpath");
$strOutput .= $text->RenderText(
	"RenderNavigationVerticalPath",
	$navigation->RenderNavigationVerticalPath(
		$navtree)
	); 
$strOutput .= $text->RenderText(
	"RenderNavigationVertical",
	$navigation->RenderNavigationVertical(
		$navtree)
	);
$strOutput .= $text->RenderText(
	"RenderNavigationVerticalCurrentPath",
	$navigation->RenderNavigationVerticalCurrentPath(
		$navtree)
	);