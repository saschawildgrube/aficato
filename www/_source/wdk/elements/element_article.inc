<?php

	class CArticleElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"article",
				$strStyleID,
				true,
				false);
		}
		
		function RenderArticle($strArticle,$strFormat)
		{
			$strOutput = "";
			$strFormat = StringLowerCase($strFormat);
			switch ($strFormat)
			{
			case "html":
				$strOutput = HtmlDecode($strArticle);
				break;
			case "wiki":
				$strOutput = HtmlDecode($strArticle);
				$this->IncludeElement("wiki");
				$wiki = new CWikiElement($this->GetWebsite(),$this->GetLayoutDef("LAYOUT_WIKI_STYLE"));
				$strOutput = $wiki->RenderWiki($strOutput);
				break;
			case "bbcode":
				require_once(GetWDKDir()."wdk_bbcode.inc");
				$strOutput = StringBBCodeToHtml($strArticle);
				break;
			case "plain":
				$strOutput = nl2br($strArticle);
				break;
			case "raw":
			default:
				$this->IncludeElement("console");
   			$console = new CConsoleElement($this->GetWebsite(),$this->GetLayoutDef("LAYOUT_CONSOLE_STYLE"));
   			$strOutput = $console->RenderConsole($strArticle);
				break;	
			}
			return $strOutput;
		}
	}

