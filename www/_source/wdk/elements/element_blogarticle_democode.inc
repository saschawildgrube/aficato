$this->IncludeElement("article");
$this->IncludeElement("blogarticle");

$blogarticle = new CBlogArticleElement($this->GetWebsite(),"default");

$strOutput = $blogarticle->RenderBlogArticle(
	"Title",
	"Some very interesting text. Loram ispum plusquam dolor.",
	"wiki",
	"2015-06-21 12:00");