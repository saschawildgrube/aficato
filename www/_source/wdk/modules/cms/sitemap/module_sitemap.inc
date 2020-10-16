<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CSiteMapModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/sitemap",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
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

