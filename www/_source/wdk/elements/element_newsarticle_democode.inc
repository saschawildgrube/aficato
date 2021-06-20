$this->IncludeElement("newsarticle");

$newsarticle = new CNewsArticleElement($this->GetWebsite(),"default");
$strOutput = $newsarticle->RenderNewsArticle(
	"Headline",
	"Loram Ipsum Plusquam Dolor Helebram Sulibrum.",
	"wiki",
	"2016-03-14 15:32:05");