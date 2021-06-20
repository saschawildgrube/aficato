<?php

	class CCollectionElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"collection",
				$strStyleID,
				true,
				false);  
		}
		
		function RenderCollection($arrayCollection,$nHeaderLevelBase = 1)
		{
			if (!is_array($arrayCollection))
			{
				return "";	
			}
			$nHeaderLevelBase = intval($nHeaderLevelBase);
			if ($nHeaderLevelBase < 1)
			{
				$nHeaderLevelBase = 1;
			}
			if ($nHeaderLevelBase > 6)
			{
				$nHeaderLevelBase = 6;
			}
			$strOutput = "";
			$header = null;
			$paragraph = null;
			$text = null;
			$table = null;
			$collection = null;
			$separator = null;
			$console = null;
			foreach ($arrayCollection as $arrayElement)
			{
				$strElement = ArrayGetValue($arrayElement,"element");
				switch ($strElement)
				{
				case "raw":
					$strOutput .= ArrayGetValue($arrayElement,"data");
					break;

				case "separator":
					if ($separator == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_SEPARATOR_STYLE");
						if ($strStyle == "")
						{
							$strStyle = "default";
						}
						$this->IncludeElement("separator");
						$separator = new CSeparatorElement($this->GetWebsite(),$strStyle);
					}
					$strOutput .= $separator->RenderSeparator();
					break;


				case "header":
					if ($header == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_HEADER_STYLE");
						if ($strStyle == "")
						{
							$strStyle = "default";
						}
						$this->IncludeElement("header");
						$header = new CHeaderElement($this->GetWebsite(),$strStyle);
					}
					$nHeaderLevel = min(6,max(0,intval(ArrayGetValue($arrayElement,"headerlevel")))) - 1;
					$strHeader = ArrayGetValue($arrayElement,"header");
					$strOutput .= $header->RenderHeader($strHeader,$nHeaderLevelBase + $nHeaderLevel);
					break;
					
				case "paragraph":
					if ($paragraph == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_PARAGRAPH_STYLE");  
						if ($strStyle == "")
						{
							$strStyle = "default";
						}
						$this->IncludeElement("paragraph");
						$paragraph = new CParagraphElement($this->GetWebsite(),$strStyle);
					}
					$strParagraph = ArrayGetValue($arrayElement,"paragraph");
					$strOutput .= $paragraph->RenderParagraph($strParagraph);
				
					break;

				case "text":
					if ($text == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_TEXT_STYLE");
						if ($strStyle == "")
						{
							$strStyle = "default";
						}
						$this->IncludeElement("text");
						$text = new CTextElement($this->GetWebsite(),$strStyle);
					}
					$strHeadline = ArrayGetValue($arrayElement,"headline");
					$strText = ArrayGetValue($arrayElement,"text");
					$strOutput .= $text->RenderText($strHeadline,$strText);
					break;

					
				case "table":
					if ($table == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_TABLE_STYLE");   
						if ($strStyle == "")
						{
							$strStyle = "default";
						}
						$table = new CTableElement($this->GetWebsite(),$strStyle);
					}
					$arrayData = ArrayGetValue($arrayElement,"data");
					$arrayHeaders = ArrayGetValue($arrayElement,"headers");
					if (is_array($arrayHeaders))
					{
						$strOutput .= $table->RenderTableEx(
							$arrayData,
							$arrayHeaders);  						
					}
					else
					{
						$strOutput .= $table->RenderTable(
							$arrayData,
							ArrayGetValue($arrayElement,"headerfirstrow"),
							ArrayGetValue($arrayElement,"headerfirstcol"));  
					}
					break;

				case "console":
					if ($console == null)
					{
						$strStyle = $this->GetLayoutDef("LAYOUT_CONSOLE_STYLE");
						if ($strStyle == "")
						{
							$strStyle = "default";
						} 
						$this->IncludeElement("console");
						$console = new CConsoleElement($this->GetWebsite(),$strStyle);
					}
					$strData = ArrayGetValue($arrayElement,"data");
					$strOutput .= $console->RenderConsole($strData);
					break;

					
				case "collection":
					if ($collection == null)
					{
						$collection = new CCollectionElement($this->GetWebsite(),$this->GetStyle());
					}
					$arrayCollection2 = ArrayGetValue($arrayElement,"collection");
					$strOutput .= $collection->RenderCollection(
						$arrayCollection2,
						$nHeaderLevelBase+1);  
					break;
				
					
				default:
					FatalError("CCollectionElement::RenderCollection(): unsupported element \"$strElement\"");
				}
				$strOutput .= "\n";
			}
		
			return $strOutput;			
		}


	}

