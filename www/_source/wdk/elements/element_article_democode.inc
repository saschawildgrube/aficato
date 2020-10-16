$this->IncludeElement("article");

$strOutput = "";
$article = new CArticleElement($this->GetWebsite(),"default");

$strArticle = "=Wiki=
Some text
* A Bullet
==Subtitle==
A [http://www.example.com link]";
$strOutput .= $article->RenderArticle($strArticle,"wiki");

$strArticle = "<h1>HTML</h1>
<p>Some text</p>
<ul>
<li>A Bullet</li>
</ul>
<h2>Subtitle</h2>
<p>A <a href=\"http://www.example.com\">link</a></p>";
$strOutput .= $article->RenderArticle($strArticle,"html");

$strArticle = "[size=16][b]BBCode[/b][/size][br]
Some text[br]
[list][*]A Bullet
[/list]
[size=14]Subtitle[/size][br]
A [url=http://www.example.com]link[/url]";
$strOutput .= $article->RenderArticle($strArticle,"bbcode");
$strOutput .= "<br/><br/>";
$strOutput .= $article->RenderArticle($strArticle,"raw");