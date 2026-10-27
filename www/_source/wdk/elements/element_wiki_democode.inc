$this->IncludeElement("wiki");

$strWiki = "
=Text=
Some blind text
=Lists=
==Bullet List==
* Apples
* Oranges
* Cherries
* Pinapples
==Multi-Level Bullet List==
* Fruits
** Apples
* Veggies
** Cabbage
==Numbered List==
# Pole position
# Second best
# Still on the podest
# Four-Whom?
==Definitions==
; Term
: Definition 1
: Definition 2
=Links=
==Wiki Style Links==
===With Caption===
* [[index|Back to the home page]]
===Without Caption===
* [[index]]
==External Links==
===With Caption===
* [http://www.example.com Example.com]
===Without Caption===
* [http://www.example.com]
=Embedded HTML=
This is some text with html <span style=\"color:red\">formatting</span>.
=Preformatted Text=
<pre>
Wiki ''formatting'' and <strong>HTML</strong> remain literal here.
</pre>
=Code=
<code>
function HelloWorld(\$strName)
{
	print(\"Hello \$strName\");
}
</code>
=Syntax Highlighting=
<syntaxhighlight lang=\"php\">
function HelloWorld(\$strName)
{
	print(\"Hello \$strName\");
}
</syntaxhighlight>
=Table=
{|
! Name
! Type
|-
| Apple
| Fruit
|-
| Cabbage
| Vegetable
|}
";

$strOutput = "";

$wiki = new CWikiElement($this->GetWebsite(),"default");
$strOutput .= $wiki->RenderWiki($strWiki);
