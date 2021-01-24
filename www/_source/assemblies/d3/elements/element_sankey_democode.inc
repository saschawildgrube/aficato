$this->IncludeElement('sankey');

$sankey = new CSankeyElement($this->GetWebsite(),'default');

$arrayData = FileReadCSV($this->GetWebsite()->GetDataFilePath('sankey.csv'));

$strOutput = $sankey->RenderSankey($arrayData,'TWh');