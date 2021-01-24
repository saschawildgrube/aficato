<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CSiteMapModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			parent::__construct(
				$website,
				"cms/sitemap",
				$strInstanceID,
				$arrayConfig);
		}
			
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "":
					//$this->SetState("start");
				default:
					return $this->StateStart();
			}
		}
		
	
		
		function StateStart()
		{
			$navtree = $this->GetWebsite()->GetNavigationTree("sitemap");
			$navtree->FilterItemsForSitemap($this->GetWebsite());
			$navigation = new CNavigationElement($this->GetWebsite(),"sitemap");
			$this->SetOutputItem("SITEMAP",$navigation->RenderNavigationVertical($navtree));
			return "start";
		}
		
		


	}

