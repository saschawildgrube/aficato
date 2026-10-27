<?php

	class CTableOfContentsElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'tableofcontents',
				$strStyleID,
				false,
				false);
		}
		
		function RenderTableOfContents()
		{
			$this->AddJavaScript('wdk-maketableofcontents');
			$this->AddJavaScript('wdk-element-tableofcontents');
			
			/*$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			$arrayItems["BEGIN"] = $this->GetLayoutDef("LAYOUT_BEGIN");
			$arrayItems["END"] = $this->GetLayoutDef("LAYOUT_END");
			
			//$strOutput = ReplaceTagsWDK($strOutput, $arrayItems);
			*/
			$strOutput = '<div id="toc"></div>';
			
			return $strOutput;
		}
	}

