<?php

	class CCarouselElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			parent::__construct(
				$website,
				"carousel",
				$strStyleID,
				true,
				true);
		}
		
		function RenderCarousel(
			$arraySlides)
		{
			if (ArrayCount($arraySlides) == 0)
			{
				return $strOutput;	
			}
			$strOutput = $this->GetLayoutHTML();
			
			$strCarouselID = "carousel-".MakeNumericToken(5);
			
			$strImageTeaserStyle = $this->GetLayoutDef("LAYOUT_IMAGETEASER_STYLE");
			if ($strImageTeaserStyle == "")
			{
				$strImageTeaserStyle = "carousel";	
			}
			$this->IncludeElement("imageteaser");
			$imageteaser = new CImageTeaserElement($this->GetWebsite(),$strImageTeaserStyle);
 	
  		$strAdditionalIndicator = $this->GetLayoutDef("LAYOUT_ADDITIONAL_INDICATOR");
  		$strAdditionalIndicator = ReplaceString($strAdditionalIndicator,"{CAROUSELID}",$strCarouselID); 
  		$strSlideBeginActive = $this->GetLayoutDef("LAYOUT_SLIDE_BEGIN_ACTIVE");
  		$strSlideBegin = $this->GetLayoutDef("LAYOUT_SLIDE_BEGIN");
  		$strSlideEnd = $this->GetLayoutDef("LAYOUT_SLIDE_END");
			
			$strAdditionalIndicators = "";
			$strSlides = "";
			
			$nCount = 0;
			foreach ($arraySlides as $arraySlide)
			{
				if (ArrayCount($arraySlide) == 0)
				{
					continue;
				}

				if ($nCount > 0)
				{
					$strAdditionalIndicators .= ReplaceString($strAdditionalIndicator,"{SLIDE_INDEX}",$nCount)."\n";
					//$strOutputCarousel .= $strSlideSeparator;
				}

				//$strLink = ArrayGetValue($arrayItem,"link");
				/*if ($strLink == "")
				{
						
				}*/
				if ($nCount == 0)
				{
					$strSlides .= $strSlideBeginActive."\n";
				}
				else
				{
					$strSlides .= $strSlideBegin."\n";
				}
				$strPayload = ArrayGetValue($arraySlide,"payload");
				if ($strPayload != "")
				{
					$strSlides .= $strPayload;
				}
				else
				{
					$strSlides .= $imageteaser->RenderImageTeaser(
						ArrayGetValue($arraySlide,"imageurl"),
						ArrayGetValue($arraySlide,"title"),
						ArrayGetValue($arraySlide,"text"),
						ArrayGetValue($arraySlide,"linkurl"),
						ArrayGetValue($arraySlide,"linktext"),
						GetBoolValue(ArrayGetValue($arraySlide,"external")));
				}
								
				$strSlides .= $strSlideEnd."\n";
				
				$nCount++;
			}
			$arrayItems = array();
			$arrayItems["SLIDES"] = $strSlides;
			$arrayItems["CAROUSELID"] = $strCarouselID;
			$arrayItems["ADDITIONAL_INDICATORS"] = $strAdditionalIndicators;
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

