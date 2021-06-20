<?php

	class CPortfolioElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			parent::__construct(
				$website,
				"portfolio",
				$strStyleID,
				true,
				true);
		}
		
		function RenderPortfolio(
			$arrayPortfolio)
		{
			if (!is_array($arrayPortfolio))
			{
				return "";	
			}
			if (ArrayCount($arrayPortfolio) == 0)
			{
				return "";				
			}
			$strOutput = $this->GetLayoutHTML(); 
			
			$strStyle = $this->GetLayoutDef("LAYOUT_PORTFOLIO_TEASER_STYLE");
			if ($strStyle == "")
			{
				$strStyle = "default";	
			}
			$this->IncludeElement("teaser");
			$teaser = new CTeaserElement($this->GetWebsite(),$strStyle);

			$strStyle = $this->GetLayoutDef("LAYOUT_PORTFOLIO_IMAGETEASER_STYLE");
			if ($strStyle == "")
			{
				$strStyle = "default";	
			}
			$this->IncludeElement("imageteaser");
			$imageteaser = new CImageTeaserElement($this->GetWebsite(),$strStyle);

			
			$strOutputPortfolio = "";
			
			$strTeaserBegin = $this->GetLayoutDef("LAYOUT_PORTFOLIO_TEASER_BEGIN");
			$strTeaserEnd = $this->GetLayoutDef("LAYOUT_PORTFOLIO_TEASER_END");
			$strTeaserSeparator = $this->GetLayoutDef("LAYOUT_PORTFOLIO_TEASER_SEPARATOR");
			
			$nCount = 0;
			foreach ($arrayPortfolio as $arrayTeaser)
			{
				if (!is_array($arrayTeaser))
				{
					continue;
				}
				if ($nCount > 0)
				{
					$strOutputPortfolio .= $strTeaserSeparator; 
				}
				$nCount++;
				$strOutputPortfolio .= $strTeaserBegin;
				$strPayload = ArrayGetValue($arrayTeaser,"PAYLOAD");
				if ($strPayload != "")
				{
					$strOutputPortfolio .= $strPayload;
				}
				else
				{
					$strImageURL = ArrayGetValue($arrayTeaser,"imageurl");
					if ($strImageURL != "")
					{
						$strOutputPortfolio .= $imageteaser->RenderImageTeaser(
							$strImageURL,
							ArrayGetValue($arrayTeaser,"title"),
							ArrayGetValue($arrayTeaser,"text"),
							ArrayGetValue($arrayTeaser,"linkurl"),
							ArrayGetValue($arrayTeaser,"linktext"),
							ArrayGetValue($arrayTeaser,"external"));
					}
					else
					{
						$strOutputPortfolio .= $teaser->RenderTeaser(
							ArrayGetValue($arrayTeaser,"title"),
							ArrayGetValue($arrayTeaser,"text"),
							ArrayGetValue($arrayTeaser,"linkurl"),
							ArrayGetValue($arrayTeaser,"linktext"),
							ArrayGetValue($arrayTeaser,"external"));
					}
				}
				$strOutputPortfolio .= $strTeaserEnd;
			}
			$arrayItems["PORTFOLIO"] = $strOutputPortfolio;
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

