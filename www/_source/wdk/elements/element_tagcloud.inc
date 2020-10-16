<?php

	require_once(GetWDKDir()."wdk_list.inc");

	class CTagCloudDefinition
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();	
			$this->m_arrayData["tags"] = array();
		}
		
		function GetData()
		{
			return $this->m_arrayData;	
		}
		
		function AddTag($strTag,$fWeight,$strLink,$strFormatStyle)
		{
			$arrayTag = array();
			$arrayTag["tag"] = $strTag;
			$arrayTag["weight"] = $fWeight;
			$arrayTag["link"] = $strLink;
			$arrayTag["formatstyle"] = $strFormatStyle;
			array_push($this->m_arrayData["tags"],$arrayTag);
		}
		
		
		
		function IsValid()
		{
			$arrayTags = ArrayGetValue($this->m_arrayData,"tags");
			if (!is_array($arrayTags))
			{
				return false;	
			}
		
			foreach ($arrayTags as $arrayTag)
			{
				$strTag = ArrayGetValue($arrayTag,"link");
/*				if ($strLink == "")
				{
					return false;	
				}
				*/
				$strTag = ArrayGetValue($arrayTag,"tag");
				if ($strTag == "")
				{
					return false;	
				}
				$fWeight = ArrayGetValue($arrayTag,"weight");
				if ($fWeight < 0.0)
				{
					return false;	
				}
			}
			return true;
		}
		
	}
	
	class CTagCloudElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"tagcloud",
				$strStyleID,
				true,
				false);
				
		}
		

		function RenderTagCloud($tagcloudDef)
		{
			$this->Trace("RenderTagCloud");
			
			if (!is_object($tagcloudDef))
			{
				$this->Error("tagcloudDef is not an object.");	
			}
			
			if ($tagcloudDef->IsValid() != true)
			{
				$this->Error("tagcloudDef->IsValid != true.");	
			}
			
			$strOutput = "";
			$strOutput .= $this->GetLayoutDef("LAYOUT_TAGCLOUD_BEGIN") . "\n";

			$arrayTags = ArrayGetValue($tagcloudDef->GetData(),"tags");
			$listTags = new CList();
			$listTags->SetListArray($arrayTags);
			$listTags->Sort(array("tag" => "asc"));
				
			
			
			$arrayTags = $listTags->GetListArray();
			$this->Trace($arrayTags);


			// Getting the highest and lowest weight
			$fWeightLow = null;
			$fWeightHigh = 0;
			foreach ($arrayTags as $arrayTag)
			{
				$fWeight = ArrayGetValue($arrayTag,"weight");
				if ($fWeightLow == null)
				{
					$fWeightLow = $fWeight;
				}
				else
				{
					$fWeightLow = min($fWeight,$fWeightLow);
				}
				$fWeightHigh = max($fWeight,$fWeightHigh);
			}
			
			$this->Trace("WeightLow : $fWeightLow");
			$this->Trace("WeightHigh: $fWeightHigh");
			
			
			/*
				min = 5
				max = 15
				span = max-min = 10
				weight = 10
				(weight - min) / span
			*/
			
			// Normalizing the weight
			$fWeightSpan = $fWeightHigh - $fWeightLow;
			foreach ($arrayTags as &$arrayTagRef)
			{
				$fWeight = floatval(ArrayGetValue($arrayTagRef,"weight"));
				if ($fWeightSpan == 0)
				{
					$fWeight = 0.5;
				}
				else
				{
					$fWeight = ($fWeight - $fWeightLow) / $fWeightSpan;
				}
				ArraySetValue($arrayTagRef,$fWeight,"weight");
			}
			
			$this->Trace($arrayTags);


				
			$nCount = ArrayCount($arrayTags);
			$nCurrent = 0;
			
			$fFontSizeFactor = floatval($this->GetLayoutDef("LAYOUT_TAGCLOUD_FONTSIZEFACTOR"));
			if ($fFontSizeFactor == 0)
			{
				$fFontSizeFactor = 1; 
			}
			
			foreach ($arrayTags as $arrayTag)
			{
				$nCurrent++;
				$strOutput .= $this->GetLayoutDef("LAYOUT_TAGCLOUD_TAG_BEGIN") . "\n";
				
				$strTag = ArrayGetValue($arrayTag,"tag");
				$strLink = ArrayGetValue($arrayTag,"link");
				$strFormatStyle = ArrayGetValue($arrayTag,"formatstyle");
				$fWeight = floatval(ArrayGetValue($arrayTag,"weight"));

				$strPayload = $strTag;
				
				// Define actual text Size
				$nFontWeight = round(($fWeight * 800)+100);
				$fFontSize = (($fWeight * $fFontSizeFactor)+1) * 100;
				
				$this->Trace("$strTag: fFontSize = $fFontSize, nFontWeight = $nFontWeight");
				
				$strPayload = "<span style=\"font-size: {$fFontSize}%; font-weight: {$nFontWeight};\">$strTag</span>";
				
				// Define defined format style
				if ($strFormatStyle != "")
				{
					$this->IncludeElement("format");
					$format = new CFormatElement($this->GetWebsite(),$strFormatStyle);
					$strPayload = $format->RenderFormat($strPayload);
				}

				// create link wrapper
				if ($strLink != "")
				{
					$link = new CLinkElement(
						$this->GetWebsite(),
						$this->GetLayoutDef("LAYOUT_TAGCLOUD_LINK_STYLE"));
					$strPayload = $link->RenderLink($strLink,$strPayload);
				}
				
				$strOutput .= $strPayload;
				
				$strOutput .= $this->GetLayoutDef("LAYOUT_TAGCLOUD_TAG_END") . "\n";
				
				if ($nCurrent < $nCount)
				{
					$strOutput .= $this->GetLayoutDef("LAYOUT_TAGCLOUD_TAG_SEPARATOR") . "\n";
				}
			}
			
			$strOutput .= $this->GetLayoutDef("LAYOUT_TAGCLOUD_END") . "\n";
			
			return $strOutput;
		}
		
	}



