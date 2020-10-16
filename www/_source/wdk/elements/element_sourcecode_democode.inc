$this->IncludeElement("sourcecode");

$sourcecode = new CSourceCodeElement($this->GetWebsite(),"default");
$strOutput = $sourcecode->RenderSourceCode("
function HelloWorld(\$strName)
{
	print(\"Hello World says \$strName\");
}
",
"php"
);