$toolbarDef = new CToolbarDefinition();

$toolbarDef->AddTool("http://www.example.com", "link", "Example.com");
$toolbarDef->AddTool("http://www.example.com", "link", "");
$toolbarDef->AddTool("http://www.example.com", "", "Example.com");

$toolbar = new CToolbarElement($this->GetWebsite(),"default");
$strOutput = $toolbar->RenderToolbar($toolbarDef);