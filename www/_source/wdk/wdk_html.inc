<?php

	require_once("wdk_random.inc");

	function HtmlEncode($str)
	{
		$str = StringEncodeUTF8($str);
		$str = htmlentities($str,ENT_QUOTES,"UTF-8");
		return $str;
	}
	
	function HtmlDecode($str)
	{
		return html_entity_decode($str,ENT_QUOTES,"UTF-8");	
	}
	
	function HtmlConvertLineBreaks($str)
	{ 
		$str = ReplaceString($str,"\r\n","\n");
		$str = ReplaceString($str,"\r","\n");
		$str = ReplaceString($str,"\n","<br/>");
		return $str;
		
	}
	
	function MakeDocType($strDocType)
	{
		$strEncoding = "utf-8";
		switch ($strDocType)	
		{
			case "html401-transitional":
				$strOutput = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">";
				break;
			case "xhtml1-transitional":
				$strOutput = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n\n";
				$strOutput .= "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">";
				break;
			case "xhtml11":
				$strOutput = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n\n";
				$strOutput .= "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";
				break;
			case "html5":
			default:
				$strOutput = "<!DOCTYPE html>";
				break;
		}
		return $strOutput;
	}


	function StringHTMLtoXMLEntities($strInput)
	{ 
		$arrayHtmlTranslation = get_html_translation_table(HTML_ENTITIES, ENT_NOQUOTES);
		$strXMLEncoded = u($strInput);
		foreach ($arrayHtmlTranslation as $strChar => $strHtmlEntity)
		{
			if ($strChar == "&")
			{
				continue;
			}
			$strXMLEncoded = ReplaceString($strXMLEncoded,$strHtmlEntity,StringXMLEntities(u($strChar))); 
		}
		return StringXMLEntities(u($strXMLEncoded));
	}

	function StringHTMLAddEmbracingTags(
		$strHTML,
		$strToken,
		$strTagStart,
		$strTagEnd,
		$bIgnoreCase,
		$arrayTagWhitelist,
		$arrayTagBlacklist)
	{
		if ($strToken == "")
		{
			return $strHTML;
		}
		
		$str_ROOT = MakeUniqueToken($strToken.$strHTML,array($strToken));
		$str_BR = MakeUniqueToken($strToken.$strHTML.$str_ROOT,array($strToken));
		$str_br = MakeUniqueToken($strToken.$strHTML.$str_ROOT.$str_BR,array($strToken));

		$strXML = StringHTMLtoXMLEntities($strHTML);
		$strToken = StringHTMLtoXMLEntities($strToken);
		
		$strXML_tweak = "<ROOT_".$str_ROOT.">".$strXML."</ROOT_".$str_ROOT.">";
		$strXML_tweak = ReplaceString($strXML_tweak,"<br/>",$str_br);
		$strXML_tweak = ReplaceString($strXML_tweak,"<BR/>",$str_BR);
		
		$strOutput = StringXMLAddEmbracingTags(
			$strXML_tweak,
			$strToken,
			$strTagStart,
			$strTagEnd,
			$bIgnoreCase,
			$arrayTagWhitelist,
			$arrayTagBlacklist);
		
		if ($strOutput != false)
		{
			$strOutput = ReplaceString($strOutput,$str_br,"<br/>");
			$strOutput = ReplaceString($strOutput,$str_BR,"<BR/>");
			$strOutput = ReplaceString($strOutput,"<ROOT_".$str_ROOT.">","");
			$strOutput = ReplaceString($strOutput,"</ROOT_".$str_ROOT.">","");
			return $strOutput;	
		}
		if ($bIgnoreCase == true)
		{
			return StringAddEmbracingTagsIgnoreCase(
				$strXML,
				$strToken,
				$strTagStart,
				$strTagEnd);	
		}
		else
		{
			return StringAddEmbracingTags(
				$strXML,
				$strToken,
				$strTagStart,
				$strTagEnd);	
		}
	}


