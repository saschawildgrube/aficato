$this->IncludeElement("article");
$this->IncludeElement("blogarticleteaser");

$blogarticleteaser = new CBlogArticleTeaserElement($this->GetWebsite(),"default");

$strOutput = $blogarticleteaser->RenderBlogArticleTeaser(
	"Title",
	"2015-06-21 12:00",
	"#article",
	"Link to article");