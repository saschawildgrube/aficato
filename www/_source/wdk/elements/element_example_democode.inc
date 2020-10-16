$this->IncludeElement("example");
$example = new CExampleElement($this->GetWebsite(),"default");
$strOutput = $example->RenderExample(
"\n
<h1>This is an example...</h1>\n
<p>...of the example element. And yes, this is self-referencing and hence confusing.</p>\n
");