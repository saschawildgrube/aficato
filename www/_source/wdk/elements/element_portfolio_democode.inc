$this->IncludeElement("portfolio");

$arrayPortfolio = array();
$arrayPortfolio[] = array(
	"title" => "Header 1", 
	"text" => "This is some blind text about thing 1.",
	"linkurl" => $this->GetWebsite()->MakeSelfURL(),
	"linktext" => "Click here");
$arrayPortfolio[] = array(
	"title" => "Header 2",
	"text" => "This is some blind text about thing 2.",
	"linkurl" => $this->GetWebsite()->MakeSelfURL(),
	"linktext" => "Click here");
$arrayPortfolio[] = array(
	"title" => "Header 3",
	"text" => "This is some blind text about thing 3.",
	"linkurl" => $this->GetWebsite()->MakeSelfURL(),
	"linktext" => "Click here");
$arrayPortfolio[] = array(
	"title" => "Header 4 without text",
	"text" => "",
	"linkurl" => $this->GetWebsite()->MakeSelfURL(),
	"linktext" => "Click here");
$arrayPortfolio[] = array(
	"title" => "Header 5 without link",
	"text" => "This is some blind text about thing 5.",
	"linkurl" => "",
	"linktext" => "");
$portfolio = new CPortfolioElement($this->GetWebsite(),"default");
$strOutput = $portfolio->RenderPortfolio(
	$arrayPortfolio);